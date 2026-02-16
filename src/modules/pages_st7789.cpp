#if defined(ESP32_S3_LCD)
#include "display_interface.h"
#include "pages_st7789.h"
#include "st7789_display.h"
#include <algorithm>
#include <cctype>
#include <float.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
#include "../utils/logs.h"

// Fonctions utilitaires LCD (ex: formatage, couleurs, etc.)
// À compléter selon besoins graphiques LCD

std::string formatFloatLCD(float value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

std::string getHeaderLCD(const std::string& title, int pageIndex, int pageCount) {
    struct tm timeinfo;
    char timeStr[6];
    if (getLocalTime(&timeinfo)) {
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    } else {
        strcpy(timeStr, "--:--");
    }
    return title + " " + std::string(timeStr) + " (" + std::to_string(pageIndex) + "/" + std::to_string(pageCount) + ")";
}

// Exemples de pages LCD (à enrichir pour tirer parti de la couleur, de la résolution, etc.)
void pageNetwork_st7789(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount) {
    d.clear();
    // Fond dégradé bleu
    for (int y = 0; y < 240; y += 8) {
        uint16_t c = 0x1F1F + (y * 0x0200);
        d.bar(0, y, 240, 8, 8, 8);
    }
    // En-tête avec icône WiFi
    d.text(10, 10, getHeaderLCD("\xF0\x9F\x93\xB6 Réseau", pageIndex, pageCount));
    // SSID
    d.text(20, 50, "SSID : " + std::string(wifi.ssid()));
    // IP
    d.text(20, 90, "IP   : " + std::string(wifi.ip()));
    // RSSI avec couleur selon force
    int rssi = wifi.rssi();
    uint16_t rssiColor = (rssi > -60) ? 0x07E0 : (rssi > -80 ? 0xFFE0 : 0xF800);
    d.text(20, 130, "RSSI : " + std::to_string(rssi) + " dBm");
    // Icône signal
    if (rssi > -60) d.text(200, 130, "\xF0\x9F\x9B\xB2"); // Antenne verte
    else if (rssi > -80) d.text(200, 130, "\xF0\x9F\x9B\xB3"); // Antenne jaune
    else d.text(200, 130, "\xF0\x9F\x9B\xB1"); // Antenne rouge
    d.show();
}

void pageSystem_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    SystemInfo s = getSystemInfo();
    d.clear();
    // Fond violet
    for (int y = 0; y < 240; y += 8) {
        uint16_t c = 0x780F + (y * 0x0010);
        d.bar(0, y, 240, 8, 8, 8);
    }
    d.text(10, 10, getHeaderLCD("\xF0\x9F\x94\xA7 Système", pageIndex, pageCount));
    d.text(20, 50, "Heap   : " + std::to_string(s.heapFree / 1024) + " KB");
    d.text(20, 80, "PSRAM  : " + std::to_string(s.psramFree / 1024) + " KB");
    d.text(20, 110, "Flash  : " + std::to_string(s.flashSize / 1024 / 1024) + " MB");
    d.text(20, 140, "Version: " + std::string(PROJECT_VERSION));
    d.text(20, 180, "\xF0\x9F\x9A\x80 MeteoHub S3");
    d.show();
}

void pageLogs_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    d.clear();
    // Fond gris
    for (int y = 0; y < 240; y += 8) {
        d.bar(0, y, 240, 8, 8, 8);
    }
    d.text(10, 10, getHeaderLCD("\xF0\x9F\x93\x84 Logs", pageIndex, pageCount));
    int y = 40;
    for (int i = 0; i < getLogCount(); i++) {
        d.text(10, y, getLog(i));
        y += 24;
        if (y > 220) break;
    }
    d.show();
}

void pageWeather_st7789(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount) {
    SensorData data = sensors.read();
    d.clear();

    // Fond dégradé bleu/vert selon température
    uint16_t colorTop = 0x1F1F; // Bleu clair
    uint16_t colorBottom = 0x07E0; // Vert
    for (int y = 0; y < 240; y += 8) {
        uint16_t c = (y < 120) ? colorTop : colorBottom;
        d.bar(0, y, 240, 8, 8, 8); // Utilise la barre comme rectangle plein
    }

    // En-tête dynamique
    d.text(10, 10, getHeaderLCD("\xF0\x9F\x8C\xA1 Weather", pageIndex, pageCount)); // Icône soleil unicode

    if (data.valid) {
        // Température en grand, couleur dynamique
        uint16_t tempColor = (data.temperature > 28) ? 0xF800 : (data.temperature < 10 ? 0x001F : 0xFFE0);
        d.text(30, 60, "Température");
        d.text(30, 90, formatFloatLCD(data.temperature, 1) + " °C");

        // Humidité
        d.text(140, 60, "Humidité");
        d.text(140, 90, formatFloatLCD(data.humidity, 0) + " %");

        // Pression
        d.text(30, 140, "Pression");
        d.text(30, 170, formatFloatLCD(data.pressure, 0) + " hPa");

        // Icône météo simple (ex : goutte si humidité > 80)
        if (data.humidity > 80) d.text(200, 60, "\xF0\x9F\x92\xA7"); // Goutte
        if (data.temperature > 28) d.text(200, 90, "\xF0\x9F\x94\xA5"); // Flamme
        if (data.temperature < 5) d.text(200, 120, "\xF0\x9F\x8C\xA4"); // Flocon
    } else {
        d.center(120, "AHT20 / BMP280");
        d.center(150, "Non détecté");
    }
    d.show();
}

void pageGraph_st7789(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
    d.clear();
    for (int y = 0; y < 240; y += 8) d.bar(0, y, 240, 8, 8, 8);
    d.drawLine(30, 200, 210, 200);
    d.drawLine(30, 40, 30, 200);
    std::string title = "Graphique ";
    if (type == 0) title += "Température";
    else if (type == 1) title += "Humidité";
    else title += "Pression";
    d.text(40, 10, title);
    int n = history.getCount();
    if (n == 0) { d.center(120, "Aucune donnée"); d.show(); return; }
    HistoryManager::Record buffer[HISTORY_SIZE];
    history.getData(buffer, HISTORY_SIZE);
    float min = 9999, max = -9999;
    for (int i = 0; i < n; ++i) {
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        if (v < min) min = v;
        if (v > max) max = v;
    }
    float yscale = (max - min) > 0.1 ? 160.0f / (max - min) : 1.0f;
    int prevx = 30, prevy = 200 - (int)(((type == 0 ? buffer[0].t : (type == 1 ? buffer[0].h : buffer[0].p)) - min) * yscale);
    uint16_t color = (type == 0) ? 0xF800 : (type == 1 ? 0x07E0 : 0x001F);
    for (int i = 1; i < n; ++i) {
        int x = 30 + (180 * i) / (n - 1);
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        int y = 200 - (int)((v - min) * yscale);
        auto* lcd = dynamic_cast<St7789Display*>(&d);
        if (lcd) lcd->drawLine(prevx, prevy, x, y, color);
        else d.drawLine(prevx, prevy, x, y);
        prevx = x;
        prevy = y;
    }
    d.show();
}

void pageForecast_st7789(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
    d.clear();
    // Fond bleu ciel
    for (int y = 0; y < 240; y += 8) {
        d.bar(0, y, 240, 8, 8, 8);
    }
    d.text(10, 10, getHeaderLCD("\xF0\x9F\x8C\x9E Prévisions", pageIndex, pageCount));
    d.text(20, 50, "Aujourd'hui : " + forecast.today.description);
    d.text(20, 80, "Temp : " + std::to_string((int)forecast.today.temp_day) + "°C");
    d.text(20, 110, "Demain : " + forecast.tomorrow.description);
    d.text(20, 140, "Temp : " + std::to_string((int)forecast.tomorrow.temp_day) + "°C");
    d.text(200, 50, "\xF0\x9F\x8C\xA1");
    d.text(200, 110, "\xF0\x9F\x8C\xA7");
    d.show();
}
#endif

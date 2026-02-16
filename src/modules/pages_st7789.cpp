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
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);
    // En-tête avec icône WiFi
    static_cast<St7789Display&>(d).text(10, 20, getHeaderLCD("Réseau", pageIndex, pageCount), ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 50, "SSID : " + std::string(wifi.ssid()), ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 70, "IP   : " + std::string(wifi.ip()), ST77XX_WHITE, 1);
    int rssi = wifi.rssi();
    uint16_t rssiColor = (rssi > -60) ? 0x07E0 : (rssi > -80 ? 0xFFE0 : 0xF800);
    static_cast<St7789Display&>(d).text(20, 90, "RSSI : " + std::to_string(rssi) + " dBm", rssiColor, 1);
}

void pageSystem_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    SystemInfo s = getSystemInfo();
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);
    static_cast<St7789Display&>(d).text(10, 20, getHeaderLCD("Système", pageIndex, pageCount), ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 50, "Heap   : " + std::to_string(s.heapFree / 1024) + " KB", ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 70, "PSRAM  : " + std::to_string(s.psramFree / 1024) + " KB", ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 90, "Flash  : " + std::to_string(s.flashSize / 1024 / 1024) + " MB", ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 110, "Version: " + std::string(PROJECT_VERSION), ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(20, 140, "MeteoHub S3", ST77XX_WHITE, 1);
}

void pageLogs_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);
    d.text(10, 20, getHeaderLCD("Logs", pageIndex, pageCount));
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
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);

    // En-tête dynamique
    static_cast<St7789Display&>(d).text(10, 20, getHeaderLCD("Weather", pageIndex, pageCount), ST77XX_WHITE, 1);
    if (data.valid) {
        uint16_t tempColor = (data.temperature > 28) ? 0xF800 : (data.temperature < 10 ? 0x001F : 0xFFE0);
        static_cast<St7789Display&>(d).text(20, 50, "Température", tempColor, 1);
        static_cast<St7789Display&>(d).text(20, 70, formatFloatLCD(data.temperature, 1) + " °C", tempColor, 1);
        static_cast<St7789Display&>(d).text(20, 100, "Humidité", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 120, formatFloatLCD(data.humidity, 0) + " %", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 150, "Pression", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 170, formatFloatLCD(data.pressure, 0) + " hPa", ST77XX_WHITE, 1);
    } else {
        static_cast<St7789Display&>(d).center(100, "AHT20 / BMP280", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).center(120, "Non détecté", ST77XX_WHITE, 1);
    }
}

void pageGraph_st7789(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);
    // Axes
    int graph_x0 = 50; // Décale l'axe Y et laisse la place à l'échelle
    int graph_x1 = 220;
    static_cast<St7789Display&>(d).drawLine(graph_x0, 40, graph_x0, 210, ST77XX_WHITE); // Y
    static_cast<St7789Display&>(d).drawLine(graph_x0, 210, graph_x1, 210, ST77XX_WHITE); // X
    std::string title = (type == 0) ? "Temperature" : (type == 1 ? "Humidite" : "Pression");
    static_cast<St7789Display&>(d).text(graph_x0 + 10, 20, title, ST77XX_WHITE, 1);
    int n = history.getCount();
    if (n == 0) { static_cast<St7789Display&>(d).center(120, "Aucune donnée", ST77XX_WHITE, 2); return; }
    HistoryManager::Record buffer[HISTORY_SIZE];
    history.getData(buffer, HISTORY_SIZE);
    float min = 9999, max = -9999;
    for (int i = 0; i < n; ++i) {
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        if (v < min) min = v;
        if (v > max) max = v;
    }
    float yscale = (max - min) > 0.1 ? 160.0f / (max - min) : 1.0f;
    // Graduation Y (valeurs)
    for (int i = 0; i <= 4; ++i) {
        float v = min + (max - min) * (4 - i) / 4.0f;
        int y = 50 + i * 40;
        static_cast<St7789Display&>(d).text(5, y - 8, formatFloatLCD(v, 1), ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).drawLine(graph_x0 - 2, y, graph_x0 + 2, y, ST77XX_WHITE);
    }
    // Graduation X (temps)
    static_cast<St7789Display&>(d).text(40, 220, "-2h", ST77XX_WHITE, 1);
    static_cast<St7789Display&>(d).text(200, 220, "now", ST77XX_WHITE, 1);
    // Courbe
    int prevx = graph_x0, prevy = 210 - (int)(((type == 0 ? buffer[0].t : (type == 1 ? buffer[0].h : buffer[0].p)) - min) * yscale);
    uint16_t color = (type == 0) ? 0xF800 : (type == 1 ? 0x07E0 : 0x001F);
    for (int i = 1; i < n; ++i) {
        int x = graph_x0 + ((graph_x1 - graph_x0) * i) / (n - 1);
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        int y = 210 - (int)((v - min) * yscale);
        static_cast<St7789Display&>(d).drawLine(prevx, prevy, x, y, color);
        prevx = x;
        prevy = y;
    }
}

void pageForecast_st7789(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
    // Fond noir uniforme
    static_cast<St7789Display&>(d).bar(0, 0, 240, 240, 240, 240, ST77XX_BLACK);
    static_cast<St7789Display&>(d).text(10, 20, getHeaderLCD("Prévisions", pageIndex, pageCount), ST77XX_WHITE, 1);
    if (view == 0) {
        static_cast<St7789Display&>(d).text(20, 50, "Aujourd'hui : " + forecast.today.description, ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 70, "Temp : " + formatFloatLCD(forecast.today.temp_day, 1) + "°C", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 90, "Min : " + formatFloatLCD(forecast.today.temp_min, 1) + "°C", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 110, "Max : " + formatFloatLCD(forecast.today.temp_max, 1) + "°C", ST77XX_WHITE, 1);
    } else if (view == 1) {
        static_cast<St7789Display&>(d).text(20, 50, "Demain : " + forecast.tomorrow.description, ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 70, "Temp : " + formatFloatLCD(forecast.tomorrow.temp_day, 1) + "°C", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 90, "Min : " + formatFloatLCD(forecast.tomorrow.temp_min, 1) + "°C", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 110, "Max : " + formatFloatLCD(forecast.tomorrow.temp_max, 1) + "°C", ST77XX_WHITE, 1);
    } else if (view == 2 && forecast.alert_active) {
        static_cast<St7789Display&>(d).text(20, 50, "Alerte météo !", ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 70, "Type : " + forecast.alert.event, ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 90, "Gravité : " + std::to_string(forecast.alert.severity), ST77XX_WHITE, 1);
        static_cast<St7789Display&>(d).text(20, 110, "Source : " + forecast.alert.sender, ST77XX_WHITE, 1);
    } else {
        static_cast<St7789Display&>(d).text(20, 50, "Pas d'alerte météo", ST77XX_WHITE, 1);
    }
}
#endif

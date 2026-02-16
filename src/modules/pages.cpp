#include <algorithm>
#include <cctype>
#include <float.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>

#include "pages.h"
#include "../utils/logs.h"

std::string formatFloat(float value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

std::string getHeader(const std::string& title, int pageIndex, int pageCount) {
    struct tm timeinfo;
    char timeStr[6];
    if (getLocalTime(&timeinfo)) {
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    } else {
        strcpy(timeStr, "--:--");
    }
    return title + " " + std::string(timeStr) + " (" + std::to_string(pageIndex) + "/" + std::to_string(pageCount) + ")";
}

std::string translateAlert(const std::string& event) {
    std::string lowerEvent = event;
    std::transform(lowerEvent.begin(), lowerEvent.end(), lowerEvent.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });

    if (lowerEvent.find("crue") != std::string::npos) return "Vigilance Crues";
    if (lowerEvent.find("flood") != std::string::npos || lowerEvent.find("inond") != std::string::npos) return "Vigilance Inondation";
    if (lowerEvent.find("rain") != std::string::npos || lowerEvent.find("pluie") != std::string::npos) return "Vigilance Pluie";
    if (lowerEvent.find("wind") != std::string::npos || lowerEvent.find("vent") != std::string::npos) return "Vigilance Vent";
    if (lowerEvent.find("thunderstorm") != std::string::npos) return "Vigilance Orages";
    if (lowerEvent.find("snow") != std::string::npos) return "Vigilance Neige";
    if (lowerEvent.find("heat") != std::string::npos) return "Vigilance Canicule";
    if (lowerEvent.find("cold") != std::string::npos) return "Vigilance Grand Froid";
    if (lowerEvent.find("ice") != std::string::npos) return "Vigilance Verglas";
    return event; // Fallback si aucune traduction
}

void pageNetwork(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount) {
    d.clear();
    d.text(0, 0, getHeader("Net.", pageIndex, pageCount));

    d.text(0, 16, std::string("SSID: ") + wifi.ssid());
    d.text(0, 28, std::string("IP:   ") + wifi.ip());
    d.text(0, 40, std::string("RSSI: ") + std::to_string(wifi.rssi()) + " dBm");

    d.show();
}

void pageSystem(DisplayInterface& d, int pageIndex, int pageCount) {
    SystemInfo s = getSystemInfo();

    d.clear();
    d.text(0, 0, getHeader("Sys.", pageIndex, pageCount));

    d.text(0, 16, std::string("Heap:  ") + std::to_string(s.heapFree / 1024) + " KB");
    d.text(0, 28, std::string("PSRAM: ") + std::to_string(s.psramFree / 1024) + " KB");
    d.text(0, 40, std::string("Flash: ") + std::to_string(s.flashSize / 1024 / 1024) + " MB");
    d.text(0, 52, std::string("Ver:   ") + std::string(PROJECT_VERSION));

    d.show();
}

void pageLogs(DisplayInterface& d, int pageIndex, int pageCount) {
    d.clear();
    d.text(0, 0, getHeader("Logs", pageIndex, pageCount));

    int y = 12;
    for (int i = 0; i < getLogCount(); i++) {
        d.text(0, y, getLog(i));
        y += 10;
        if (y > 54) break;
    }

    d.show();
}

void pageWeather(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount) {
    SensorData data = sensors.read();
    d.clear();
    d.text(0, 0, getHeader("Meteo", pageIndex, pageCount));

    if (data.valid) {
        d.text(0, 16, std::string("Temp: ") + formatFloat(data.temperature, 1) + " C");
        d.text(0, 28, std::string("Hum:  ") + formatFloat(data.humidity, 0) + " %");
        d.text(0, 40, std::string("Pres: ") + formatFloat(data.pressure, 0) + " hPa");
    } else {
        d.center(30, "AHT20 / BMP280");
        d.center(42, "Non detecte");
    }
    d.show();
}

void pageGraph(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
    d.clear();
    
    std::string title;
    if (type == 0) title = "Temp. (C)";
    else if (type == 1) title = "Hum. (%)";
    else title = "Pres. (hPa)";
    
    d.text(0, 0, getHeader(title, pageIndex, pageCount));

    HistoryManager::Record records[HISTORY_SIZE];
    history.getData(records, HISTORY_SIZE);
    
    int count = history.getCount();
    if (count == 0) {
        d.center(30, "Attente donnees...");
        d.show();
        return;
    }

    // Layout : Graphique a gauche, Valeurs a droite, Temps en bas
    int graphX = 0;
    int graphY = 14;
    int graphW = 90; // Largeur reduite pour laisser place au texte a droite
    int graphH = 38; // Hauteur reduite pour laisser place au temps en bas
    int bottomY = graphY + graphH;

    // N'utiliser que la fenetre affichee (points les plus recents)
    int visibleCount = count;
    if (visibleCount > graphW + 1) {
        visibleCount = graphW + 1;
    }
    int startIndex = count - visibleCount;

    // Calculate min/max from displayed data window
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;
    for (int i = startIndex; i < count; i++) {
        float val;
        if (type == 0) val = records[i].t;
        else if (type == 1) val = records[i].h;
        else val = records[i].p;
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
    }
    if (minVal == maxVal) {
        float margin = (type == 2) ? 0.5f : 0.3f;
        minVal -= margin;
        maxVal += margin;
    }

    int labelPrecision = 0;
    if (type == 0) {
        labelPrecision = 1;
    } else if (type == 2) {
        labelPrecision = 1;
    }

    std::string maxLabel = formatFloat(maxVal, labelPrecision);
    std::string minLabel = formatFloat(minVal, labelPrecision);
    if (maxLabel == minLabel && labelPrecision < 2) {
        labelPrecision++;
        maxLabel = formatFloat(maxVal, labelPrecision);
        minLabel = formatFloat(minVal, labelPrecision);
    }

    // Affichage Echelle Valeurs (Droite)
    d.text(92, graphY, maxLabel);
    d.text(92, bottomY - 8, minLabel);

    // Affichage Echelle Temps (Bas)
    d.text(0, 54, "-2h");
    d.text(70, 54, "now");
    
    // Drawing loop with gap detection
    for (int i = startIndex + 1; i < count; i++) {
        // If gap is > 90 seconds, don't draw the connecting line
        if (records[i].timestamp - records[i-1].timestamp > 90) {
            continue;
        }

        float val1, val2;
        if (type == 0) { val1 = records[i-1].t; val2 = records[i].t; }
        else if (type == 1) { val1 = records[i-1].h; val2 = records[i].h; }
        else { val1 = records[i-1].p; val2 = records[i].p; }

        int localIndex1 = (i - 1) - startIndex;
        int localIndex2 = i - startIndex;
        int x1 = map(localIndex1, 0, visibleCount - 1, graphX, graphX + graphW);
        int x2 = map(localIndex2, 0, visibleCount - 1, graphX, graphX + graphW);
        int y1 = bottomY - (int)(((val1 - minVal) / (maxVal - minVal)) * graphH);
        int y2 = bottomY - (int)(((val2 - minVal) / (maxVal - minVal)) * graphH);
        d.drawLine(x1, y1, x2, y2);
    }
    d.show();
}

void pageForecast(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
    d.clear();
    d.text(0, 0, getHeader("Prev.", pageIndex, pageCount));

    if (view == 0) { // Vue "Aujourd'hui"
        d.center(14, "Aujourd'hui");
        d.text(0, 28, std::string("Min: ") + formatFloat(forecast.today.temp_min, 0) + "C / Max: " + formatFloat(forecast.today.temp_max, 0) + "C");
        d.center(42, forecast.today.description);

    } else if (view == 1) { // Vue "Demain"
        d.center(14, "Demain");
        d.text(0, 28, std::string("Min: ") + formatFloat(forecast.tomorrow.temp_min, 0) + "C / Max: " + formatFloat(forecast.tomorrow.temp_max, 0) + "C");
        d.center(42, forecast.tomorrow.description);

    } else { // Vue "Alertes"
        d.center(14, "Alertes Météo");
        if (forecast.alert_active) {
            d.center(28, forecast.alert.sender);
            
            // Traduction et affichage de l'alerte
            std::string event = translateAlert(forecast.alert.event);
            if (event.size() > 20) {
                size_t split = event.rfind(' ', 20);
                if (split == std::string::npos) split = 20;
                d.center(40, event.substr(0, split));
                if (split + 1 < event.size()) {
                    d.center(50, event.substr(split + 1));
                }
            } else {
                d.center(42, event);
            }
        } else {
            d.center(35, "Aucune alerte");
        }
    }
    d.show();
}

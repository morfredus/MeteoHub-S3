#include "pages.h"
#include "logs.h"
#include <float.h>
#include <time.h>

// Helper pour afficher l'heure dans le titre
String getHeader(const String& title, int pageIndex, int pageCount) {
    struct tm timeinfo;
    char timeStr[6];
    if(getLocalTime(&timeinfo)){
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    } else {
        strcpy(timeStr, "--:--");
    }
    return title + " " + String(timeStr) + " (" + String(pageIndex) + "/" + String(pageCount) + ")";
}

// Helper pour traduire les alertes en français
String translateAlert(const String& event) {
    String lowerEvent = event;
    lowerEvent.toLowerCase();

    if (lowerEvent.indexOf("wind") != -1) return "Vigilance Vent";
    if (lowerEvent.indexOf("rain") != -1) return "Vigilance Pluie";
    if (lowerEvent.indexOf("thunderstorm") != -1) return "Vigilance Orages";
    if (lowerEvent.indexOf("snow") != -1) return "Vigilance Neige";
    if (lowerEvent.indexOf("flood") != -1) return "Vigilance Inondation";
    if (lowerEvent.indexOf("heat") != -1) return "Vigilance Canicule";
    if (lowerEvent.indexOf("cold") != -1) return "Vigilance Grand Froid";
    if (lowerEvent.indexOf("ice") != -1) return "Vigilance Verglas";
    return event; // Fallback si aucune traduction
}

void pageNetwork(Sh1106Display& d, WifiManager& wifi, int pageIndex, int pageCount) {
    d.clear();
    d.text(0, 0, getHeader("Net.", pageIndex, pageCount));

    d.text(0, 16, "SSID: " + wifi.ssid());
    d.text(0, 28, "IP:   " + wifi.ip());
    d.text(0, 40, "RSSI: " + String(wifi.rssi()) + " dBm");

    d.show();
}

void pageSystem(Sh1106Display& d, int pageIndex, int pageCount) {
    SystemInfo s = getSystemInfo();

    d.clear();
    d.text(0, 0, getHeader("Sys.", pageIndex, pageCount));

    d.text(0, 16, "Heap:  " + String(s.heapFree / 1024) + " KB");
    d.text(0, 28, "PSRAM: " + String(s.psramFree / 1024) + " KB");
    d.text(0, 40, "Flash: " + String(s.flashSize / 1024 / 1024) + " MB");
    d.text(0, 52, "Ver:   " + String(PROJECT_VERSION));

    d.show();
}

void pageLogs(Sh1106Display& d, int pageIndex, int pageCount) {
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

void pageWeather(Sh1106Display& d, SensorManager& sensors, int pageIndex, int pageCount) {
    SensorData data = sensors.read();
    d.clear();
    d.text(0, 0, getHeader("Meteo", pageIndex, pageCount));

    if (data.valid) {
        d.text(0, 16, "Temp: " + String(data.temperature, 1) + " C");
        d.text(0, 28, "Hum:  " + String(data.humidity, 0) + " %");
        d.text(0, 40, "Pres: " + String(data.pressure, 0) + " hPa");
    } else {
        d.center(30, "AHT20 / BMP280");
        d.center(42, "Non detecte");
    }
    d.show();
}

void pageGraph(Sh1106Display& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
    d.clear();
    
    String title;
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

    // Calculate min/max from the data
    float minVal = FLT_MAX;
    float maxVal = -FLT_MAX;
    for (int i = 0; i < count; i++) {
        float val;
        if (type == 0) val = records[i].t;
        else if (type == 1) val = records[i].h;
        else val = records[i].p;
        if (val < minVal) minVal = val;
        if (val > maxVal) maxVal = val;
    }
    if (minVal == maxVal) { minVal -= 1; maxVal += 1; }

    // Layout : Graphique a gauche, Valeurs a droite, Temps en bas
    int graphX = 0;
    int graphY = 14;
    int graphW = 90; // Largeur reduite pour laisser place au texte a droite
    int graphH = 38; // Hauteur reduite pour laisser place au temps en bas
    int bottomY = graphY + graphH;

    // Affichage Echelle Valeurs (Droite)
    d.text(92, graphY, String(maxVal, 0));
    d.text(92, bottomY - 8, String(minVal, 0));

    // Affichage Echelle Temps (Bas)
    d.text(0, 54, "-2h");
    d.text(70, 54, "now");
    
    // Drawing loop with gap detection
    for (int i = 1; i < count; i++) {
        // If gap is > 90 seconds, don't draw the connecting line
        if (records[i].timestamp - records[i-1].timestamp > 90) {
            continue;
        }

        float val1, val2;
        if (type == 0) { val1 = records[i-1].t; val2 = records[i].t; }
        else if (type == 1) { val1 = records[i-1].h; val2 = records[i].h; }
        else { val1 = records[i-1].p; val2 = records[i].p; }

        int x1 = map(i - 1, 0, count - 1, graphX, graphX + graphW);
        int x2 = map(i, 0, count - 1, graphX, graphX + graphW);
        int y1 = bottomY - (int)(((val1 - minVal) / (maxVal - minVal)) * graphH);
        int y2 = bottomY - (int)(((val2 - minVal) / (maxVal - minVal)) * graphH);
        d.drawLine(x1, y1, x2, y2);
    }
    d.show();
}

void pageForecast(Sh1106Display& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
    d.clear();
    d.text(0, 0, getHeader("Prev.", pageIndex, pageCount));

    if (view == 0) { // Vue "Aujourd'hui"
        d.center(14, "Aujourd'hui");
        d.text(0, 28, "Min: " + String(forecast.today.temp_min, 0) + "C / Max: " + String(forecast.today.temp_max, 0) + "C");
        d.center(42, forecast.today.description);

    } else if (view == 1) { // Vue "Demain"
        d.center(14, "Demain");
        d.text(0, 28, "Min: " + String(forecast.tomorrow.temp_min, 0) + "C / Max: " + String(forecast.tomorrow.temp_max, 0) + "C");
        d.center(42, forecast.tomorrow.description);

    } else { // Vue "Alertes"
        d.center(14, "Alertes Météo");
        if (forecast.alert_active) {
            d.center(28, forecast.alert.sender);
            
            // Traduction et affichage de l'alerte
            String event = translateAlert(forecast.alert.event);
            if (event.length() > 20) {
                int split = event.lastIndexOf(' ', 20);
                if (split == -1) split = 20;
                d.center(40, event.substring(0, split));
                d.center(50, event.substring(split + 1));
            } else {
                d.center(42, event);
            }
        } else {
            d.center(35, "Aucune alerte");
        }
    }
    d.show();
}

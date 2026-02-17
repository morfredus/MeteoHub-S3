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
#include "../utils/system.h"
#include "config.h"
#include <Arduino.h> // Pour delay()

// Fonctions utilitaires LCD (ex: formatage, couleurs, etc.)
// À compléter selon besoins graphiques LCD

std::string formatFloatLCD(float value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

std::string getTimeStr() {
    struct tm timeinfo;
    char timeStr[6];
    if (getLocalTime(&timeinfo)) {
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    } else {
        strcpy(timeStr, "--:--");
    }
    return std::string(timeStr);
}

std::string getPageStr(int pageIndex, int pageCount) {
    return std::to_string(pageIndex) + "/" + std::to_string(pageCount);
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

// Exemples de pages LCD (à enrichir pour tirer parti de la couleur, de la résolution, etc.)
void pageNetwork_st7789(DisplayInterface& d, WifiManager& wifi, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    tft.clear();
    tft.drawHeader("Réseau", getPageStr(pageIndex, pageCount));

    int y = (LCD_HEIGHT == 320) ? 80 : 60;
    int dy = (LCD_HEIGHT == 320) ? 40 : 30;

    tft.text(20, y, "SSID", C_GREY, 1);
    tft.text(80, y, wifi.ssid(), C_WHITE, 1);
    
    y += dy;
    tft.text(20, y, "IP", C_GREY, 1);
    tft.text(80, y, wifi.ip(), C_CYAN, 1);

    y += dy;
    int rssi = wifi.rssi();
    uint16_t rssiColor = (rssi > -60) ? C_GREEN : (rssi > -80 ? C_ORANGE : C_RED);
    tft.text(20, y, "Signal", C_GREY, 1);
    tft.text(80, y, std::to_string(rssi) + " dBm", rssiColor, 1);

    // Barre de signal visuelle
    y += (LCD_HEIGHT == 320) ? 50 : 40;
    int barW = 160;
    int signalPct = 0;
    if (rssi > -100) signalPct = map(rssi, -100, -50, 0, 100);
    if (signalPct < 0) signalPct = 0; if (signalPct > 100) signalPct = 100;
    
    tft.drawRect(40, y, barW, 10, C_GREY); // Cadre
    tft.bar(40, y, barW, 10, signalPct, 100, rssiColor);
    d.show();
}

void pageSystem_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    SystemInfo s = getSystemInfo();
    
    tft.clear();
    tft.drawHeader("Système", getPageStr(pageIndex, pageCount));

    int y = (LCD_HEIGHT == 320) ? 80 : 60;
    int dy = (LCD_HEIGHT == 320) ? 40 : 30;

    tft.text(20, y, "Heap", C_GREY, 1);
    tft.text(100, y, std::to_string(s.heapFree / 1024) + " KB", C_GREEN, 1);
    
    y += dy;
    tft.text(20, y, "PSRAM", C_GREY, 1);
    tft.text(100, y, std::to_string(s.psramFree / 1024) + " KB", C_GREEN, 1);
    
    y += dy;
    tft.text(20, y, "Flash", C_GREY, 1);
    tft.text(100, y, std::to_string(s.flashSize / 1024 / 1024) + " MB", C_YELLOW, 1);
    
    y += dy;
    tft.text(20, y, "Version", C_GREY, 1);
    tft.text(100, y, std::string(PROJECT_VERSION), C_MAGENTA, 1);

    tft.center((LCD_HEIGHT == 320) ? 300 : 220, "MeteoHub S3", C_TEAL, 1);
    d.show();
}

void pageLogs_st7789(DisplayInterface& d, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    tft.clear();
    tft.drawHeader("Logs", getTimeStr());

    int y = 50;
    for (int i = 0; i < getLogCount(); i++) {
        std::string logLine = getLog(i);
        uint16_t color = C_WHITE;
        if (logLine.find("[E]") != std::string::npos) color = C_RED;
        else if (logLine.find("[W]") != std::string::npos) color = C_ORANGE;
        else if (logLine.find("[D]") != std::string::npos) color = C_GREY;
        
        tft.text(5, y, logLine, color, 1);
        y += 18;
        if (y > ((LCD_HEIGHT == 320) ? 310 : 230)) break;
    }
    d.show();
}

void pageWeather_st7789(DisplayInterface& d, SensorManager& sensors, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    SensorData data = sensors.read();
    
    tft.clear();
    tft.drawHeader("Météo Locale", getTimeStr());

    if (data.valid) {
        // Température Principale (Gros)
        uint16_t tempColor = (data.temperature > 28) ? C_RED : (data.temperature < 10 ? C_BLUE : C_ORANGE);
        
        tft.center((LCD_HEIGHT == 320) ? 80 : 60, "Température", C_GREY, 1);
        
        // Astuce: Utiliser size 2 ou 3 pour simuler une grosse police
        std::string tempStr = formatFloatLCD(data.temperature, 1);
        int16_t x1, y1; uint16_t w, h;
        // Centrage manuel pour le "gros" texte
        int xCenter = 120;
        int yCenter = (LCD_HEIGHT == 320) ? 130 : 102;
        tft.text(xCenter - 50, yCenter, tempStr, tempColor, 3); // Size 3
        tft.text(xCenter + 60, yCenter, "C", tempColor, 1); // Unité plus petite

        // Cartes Humidité et Pression en bas
        int cardY = (LCD_HEIGHT == 320) ? 220 : 160;
        int cardH = (LCD_HEIGHT == 320) ? 80 : 60;
        int cardW = 100;
        
        // Humidité (Gauche)
        tft.drawCard(10, cardY, cardW, cardH, C_DARKGREY);
        // On refait propre (suppression doublons):
        tft.text(25, cardY + 15, "Humidité", C_CYAN, 1);
        tft.text(30, cardY + 40, formatFloatLCD(data.humidity, 0) + " %", C_WHITE, 1);

        // Pression (Droite)
        tft.drawCard(130, cardY, cardW, cardH, C_DARKGREY);
        tft.text(145, cardY + 15, "Pression", C_GREEN, 1);
        tft.text(140, cardY + 40, formatFloatLCD(data.pressure, 0) + " hPa", C_WHITE, 1);

    } else {
        tft.center(100, "Capteurs", C_RED, 1);
        tft.center(130, "Non détectés", C_WHITE, 1);
    }
    d.show();
}

void pageGraph_st7789(DisplayInterface& d, HistoryManager& history, int type, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    tft.clear();
    
    std::string title = (type == 0) ? "Température" : (type == 1 ? "Humidité" : "Pression");
    tft.drawHeader(title, getPageStr(pageIndex, pageCount));

    // Axes
    int graph_x0 = 40; 
    int graph_x1 = 230;
    int graph_y0 = 55;
    int graph_y1 = (LCD_HEIGHT == 320) ? 290 : 210;

    // Cadre du graphe
    tft.drawLine(graph_x0, graph_y0, graph_x0, graph_y1, C_GREY); // Y Axis
    tft.drawLine(graph_x0, graph_y1, graph_x1, graph_y1, C_GREY); // X Axis

    const auto& buffer = history.getRecentHistory();
    int n = buffer.size();
    if (n == 0) { tft.center(120, "Attente donnees...", C_GREY, 1); d.show(); return; }
    
    float min = 9999, max = -9999;
    for (int i = 0; i < n; ++i) {
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        if (v < min) min = v;
        if (v > max) max = v;
    }
    
    // Marge pour éviter que la courbe touche les bords
    float margin = (max - min) * 0.1;
    if (margin < 0.5) margin = 0.5;
    min -= margin;
    max += margin;

    float yscale = (graph_y1 - graph_y0) / (max - min);
    
    // Graduation Y (valeurs)
    // Grille horizontale
    for (int i = 0; i <= 4; ++i) {
        float v = min + (max - min) * (4 - i) / 4.0f;
        int y = graph_y0 + i * ((graph_y1 - graph_y0) / 4);
        
        // Label à gauche
        tft.text(2, y - 6, formatFloatLCD(v, (type==2?0:1)), C_GREY, 1);
        
        // Ligne pointillée
        for(int x=graph_x0; x<graph_x1; x+=4) tft.drawPixel(x, y, C_DARKGREY);
    }
    
    // Graduation X (temps)
    int label_y = (LCD_HEIGHT == 320) ? 300 : 220;
    tft.text(graph_x0, label_y, "-2h", C_GREY, 1);
    tft.text(graph_x1 - 30, label_y, "now", C_GREY, 1);
    
    // Courbe
    int prevx = graph_x0;
    float v0 = (type == 0) ? buffer[0].t : (type == 1 ? buffer[0].h : buffer[0].p);
    int prevy = graph_y1 - (int)((v0 - min) * yscale);
    
    uint16_t color = (type == 0) ? C_ORANGE : (type == 1 ? C_CYAN : C_GREEN);
    
    for (int i = 1; i < n; ++i) {
        // Correction : Utiliser la largeur disponible pour le nombre de points actuels
        int x = graph_x0 + ((graph_x1 - graph_x0) * i) / (n > 1 ? n - 1 : 1);
        
        float v = (type == 0) ? buffer[i].t : (type == 1 ? buffer[i].h : buffer[i].p);
        int y = graph_y1 - (int)((v - min) * yscale);
        
        // Dessiner ligne épaisse (double trait) pour visibilité
        tft.drawLine(prevx, prevy, x, y, color);
        tft.drawLine(prevx, prevy+1, x, y+1, color); 
        
        prevx = x;
        prevy = y;
    }
    d.show();
}

void pageForecast_st7789(DisplayInterface& d, ForecastManager& forecast, int view, int pageIndex, int pageCount) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    tft.clear();
    tft.drawHeader("Prévisions", getTimeStr());

    int y_title = (LCD_HEIGHT == 320) ? 60 : 50;
    int y_desc = (LCD_HEIGHT == 320) ? 100 : 80;
    int y_lbl = (LCD_HEIGHT == 320) ? 160 : 120;
    int y_val = (LCD_HEIGHT == 320) ? 200 : 155;

    if (view == 0) {
        tft.center(y_title, "Aujourd'hui", C_CYAN, 1);
        tft.center(y_desc, forecast.today.description, C_WHITE, 1);
        
        // Températures
        tft.text(40, y_lbl, "Min", C_BLUE, 1);
        tft.text(40, y_val, formatFloatLCD(forecast.today.temp_min, 0), C_WHITE, 2);
        
        tft.text(140, y_lbl, "Max", C_RED, 1);
        tft.text(140, y_val, formatFloatLCD(forecast.today.temp_max, 0), C_WHITE, 2);

    } else if (view == 1) {
        tft.center(y_title, "Demain", C_CYAN, 1);
        tft.center(y_desc, forecast.tomorrow.description, C_WHITE, 1);
        
        tft.text(40, y_lbl, "Min", C_BLUE, 1);
        tft.text(40, y_val, formatFloatLCD(forecast.tomorrow.temp_min, 0), C_WHITE, 2);
        
        tft.text(140, y_lbl, "Max", C_RED, 1);
        tft.text(140, y_val, formatFloatLCD(forecast.tomorrow.temp_max, 0), C_WHITE, 2);

    } else if (view == 2 && forecast.alert_active) {
        tft.fillScreen(C_RED); // Fond rouge pour alerte
        int y_base = (LCD_HEIGHT == 320) ? 60 : 40;
        tft.center(y_base, "ALERTE", C_YELLOW, 2);
        
        tft.center(y_base + 50, translateAlert(forecast.alert.event), C_WHITE, 1);
        tft.center(y_base + 80, "Niveau: " + std::to_string(forecast.alert.severity), C_WHITE, 1);
        tft.center(y_base + 110, forecast.alert.sender, C_BLACK, 1);

    } else {
        tft.center(100, "Aucune Alerte", C_GREEN, 1);
        tft.center(130, "Météo Normale", C_GREY, 1);
    }
    d.show();
}

// --- Ecrans de démarrage (Splash & Boot) ---

void drawSplashScreen_st7789(DisplayInterface& d) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    
    int cx = LCD_WIDTH / 2;
    int cy = LCD_HEIGHT / 2;

    // Séquence 1 : Studio MORFREDUS
    tft.clear();
    
    // Lignes décoratives "Modernes" qui encadrent le nom
    tft.drawLine(20, cy - 35, LCD_WIDTH - 20, cy - 35, C_ORANGE);
    tft.drawLine(20, cy + 25, LCD_WIDTH - 20, cy + 25, C_ORANGE);
    
    tft.center(cy + 5, "morfredus", C_ORANGE, 2); // Police taille 2
    
    d.show();
    delay(2000); // Temps de lecture

    // Séquence 2 : Projet & Version
    tft.clear();
    tft.center(cy - 25, PROJECT_NAME, C_CYAN, 2);
    tft.center(cy + 10, std::string("v") + PROJECT_VERSION, C_WHITE, 1);
    tft.center(cy + 50, "Initialisation...", C_GREY, 1);
    
    d.show();
    delay(1500);
}

void drawBootProgress_st7789(DisplayInterface& d, int step, int total, const std::string& msg) {
    St7789Display& tft = static_cast<St7789Display&>(d);
    tft.clear();

    int cy = LCD_HEIGHT / 2;

    // Rappel du titre
    tft.center(40, PROJECT_NAME, C_CYAN, 1);

    // Barre de progression
    int barW = LCD_WIDTH - 60;
    int barH = 14;
    int barX = 30;
    int barY = cy - 10;

    // Dessin de la barre
    tft.bar(barX, barY, barW, barH, step, total, C_GREEN);
    
    // Message de l'étape en cours
    tft.center(cy + 20, msg, C_WHITE, 1);
    
    d.show();
}
#endif

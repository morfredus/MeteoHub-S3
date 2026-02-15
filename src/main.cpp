#include <Arduino.h>
#include "sh1106_display.h"
#include "wifi_manager.h"
#include "ui_manager.h"
#include "neopixel_status.h"
#include "sensors.h"
#include "logs.h"
#include "forecast_manager.h"
#include <time.h>

Sh1106Display display;
WifiManager wifi;
UiManager ui;
SensorManager sensors;
ForecastManager forecast;

void drawBootStep(const String& label, int percent) {
    display.clear();
    display.center(10, PROJECT_NAME);
    display.center(30, label);
    display.bar(10, 50, 108, 8, percent, 100);
    display.show();
}

void setup() {
    Serial.begin(115200);

    display.begin();
    neoInit();
    
    // Etape 1 : Demarrage
    drawBootStep("Booting...", 0);
    addLog("System Boot");

    // Etape 2 : Capteurs
    drawBootStep("Init Sensors...", 20);
    sensors.begin();
    delay(200); // Petit delai visuel

    // Etape 3 : WiFi
    drawBootStep("Connecting WiFi...", 40);
    wifi.begin();
    
    // Boucle d'attente WiFi (Max ~10s)
    int w = 0;
    while (wifi.ip() == "0.0.0.0" && w < 100) {
        wifi.update();
        drawBootStep("Connecting WiFi...", 40 + (w % 20)); // Animation 40-60%
        delay(100);
        w++;
    }
    
    // Etape 4 : Heure
    drawBootStep("Sync Time...", 60);
    configTime(3600, 3600, "pool.ntp.org");
    
    // Boucle d'attente NTP (Max 10s)
    struct tm timeinfo;
    int t = 0;
    while (!getLocalTime(&timeinfo, 0) && t < 100) {
        drawBootStep("Sync Time...", 60 + (t * 30 / 100)); // Progression 60-90%
        delay(100);
        t++;
    }
    
    if (getLocalTime(&timeinfo, 0)) {
        addLog("NTP Sync OK");
    } else {
        addLog("NTP Sync Fail");
    }

    // Etape 5 : Pret
    drawBootStep("System Ready", 100);
    delay(800); // Laisser le temps de lire "System Ready"

    drawBootStep("Load History...", 100);
    
    // Lancement des modules principaux
    forecast.begin();

    ui.begin(display, wifi, sensors, forecast);
}

void loop() {
    ui.update();
}

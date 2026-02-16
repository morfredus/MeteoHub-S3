#include <string>
#include <time.h>
#include <Arduino.h>

#include "managers/forecast_manager.h"
#include "managers/ui_manager.h"
#include "managers/wifi_manager.h"
#include "modules/neopixel_status.h"
#include "modules/sensors.h"
#if defined(ESP32_S3_OLED)
#include "modules/sh1106_display.h"
#endif
#if defined(ESP32_S3_LCD)
#include "modules/st7789_display.h"
#endif
#include "utils/logs.h"

DisplayInterface* display = nullptr;
WifiManager wifi;
UiManager ui;
SensorManager sensors;
ForecastManager forecast;

void drawBootStep(const std::string& label, int percent) {
    display->clear();
    display->center(10, PROJECT_NAME);
    display->center(30, label);
    display->bar(10, 50, 108, 8, percent, 100);
    display->show();
}

void setup() {
    Serial.begin(115200);

#if defined(ESP32_S3_OLED)
    static Sh1106Display oled;
    display = &oled;
#elif defined(ESP32_S3_LCD)
    static St7789Display lcd;
    display = &lcd;
#endif
    display->begin();
    neoInit();

    // Etape 1 : Demarrage
    drawBootStep("Booting...", 0);
    LOG_INFO("System Boot");

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
        LOG_INFO("NTP Sync OK");
    } else {
        LOG_WARNING("NTP Sync Fail");
    }

    // Etape 5 : Pret
    drawBootStep("System Ready", 100);
    delay(800); // Laisser le temps de lire "System Ready"

    drawBootStep("Load History...", 100);
    
    // Lancement des modules principaux
    forecast.begin();

    ui.begin(*display, wifi, sensors, forecast);
}

void loop() {
    ui.update();
}

#include <string>
#include <time.h>
#include <Arduino.h>

#include "managers/forecast_manager.h"
#include "managers/ui_manager.h"
#include "managers/wifi_manager.h"
#include "managers/web_manager.h"
#include "managers/history_manager.h"
#include "modules/neopixel_status.h"
#include "modules/sensors.h"
#if defined(ESP32_S3_OLED)
#include "modules/sh1106_display.h"
#endif
#if defined(ESP32_S3_LCD)
#include "modules/st7789_display.h"
#include "modules/pages_st7789.h"
#endif
#include "utils/logs.h"

DisplayInterface* display = nullptr;
WifiManager wifi;
UiManager ui;
SensorManager sensors;
ForecastManager forecast;
WebManager webManager;
HistoryManager history;

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

    // Etape 1 : Splash Screen (MORFREDUS + Projet)
#if defined(ESP32_S3_LCD)
    drawSplashScreen_st7789(*display);
#elif defined(ESP32_S3_OLED)
    drawSplashScreen_sh1106(*display);
#endif

    LOG_INFO("System Boot");

    // Etape 2 : Capteurs
#if defined(ESP32_S3_LCD)
    drawBootProgress_st7789(*display, 1, 5, "Init Capteurs...");
#elif defined(ESP32_S3_OLED)
    drawBootProgress_sh1106(*display, 1, 5, "Init Capteurs...");
#endif
    sensors.begin();
    delay(200); // Petit delai visuel

    // Etape 3 : WiFi
#if defined(ESP32_S3_LCD)
    drawBootProgress_st7789(*display, 2, 5, "Connexion WiFi...");
#elif defined(ESP32_S3_OLED)
    drawBootProgress_sh1106(*display, 2, 5, "Connexion WiFi...");
#endif
    wifi.begin();

    // Boucle d'attente WiFi (Max ~10s)
    int w = 0;
    while (wifi.ip() == "0.0.0.0" && w < 100) {
        wifi.update();
        delay(100);
        w++;
    }

    // Etape 4 : Heure
#if defined(ESP32_S3_LCD)
    drawBootProgress_st7789(*display, 3, 5, "Sync Heure...");
#elif defined(ESP32_S3_OLED)
    drawBootProgress_sh1106(*display, 3, 5, "Sync Heure...");
#endif
    configTime(3600, 3600, "pool.ntp.org");

    // Boucle d'attente NTP (Max 10s)
    struct tm timeinfo;
    int t = 0;
    while (!getLocalTime(&timeinfo, 0) && t < 100) {
        delay(100);
        t++;
    }
    
    if (getLocalTime(&timeinfo, 0)) {
        LOG_INFO("NTP Sync OK");
    } else {
        LOG_WARNING("NTP Sync Fail");
    }

    // Etape 5 : Pret
#if defined(ESP32_S3_LCD)
    drawBootProgress_st7789(*display, 4, 5, "Chargement Historique...");
#elif defined(ESP32_S3_OLED)
    drawBootProgress_sh1106(*display, 4, 5, "Chargement Historique...");
#endif
    
    // Etape 6 : Lancement
#if defined(ESP32_S3_LCD)
    drawBootProgress_st7789(*display, 5, 5, "Systeme Pret");
#elif defined(ESP32_S3_OLED)
    drawBootProgress_sh1106(*display, 5, 5, "Systeme Pret");
#endif
    delay(800);
    
    history.begin();
    
    // Lancement des modules principaux
    forecast.begin();
    webManager.begin(history);

    ui.begin(*display, wifi, sensors, forecast, history);
}

void loop() {
    static unsigned long lastHistoryUpdate = 0;
    static unsigned long lastLedUpdate = 0;

    wifi.update();
    forecast.update();
    history.update(); 
    ui.update();
    webManager.handle();

    // Gestion LED Status (toutes les 500ms)
    if (millis() - lastLedUpdate >= 500) {
        lastLedUpdate = millis();
        static bool blink = false;
        blink = !blink;

        if (forecast.alert_active) {
            if (forecast.alert.severity >= 3) { if (blink) neoAlertRed(); else neoOff(); }
            else if (forecast.alert.severity == 2) neoAlertOrange();
            else neoAlertYellow();
        } else {
            if (wifi.ip() != "0.0.0.0") neoWifiOK();
            else { if (blink) neoWifiLost(); else neoOff(); }
        }
    }

    // Enregistrement historique toutes les minutes
    if (millis() - lastHistoryUpdate >= 60000) {
        lastHistoryUpdate = millis();
        SensorData data = sensors.read();
        if (data.valid) {
            history.add(data.temperature, data.humidity, data.pressure);
        }
    }
}

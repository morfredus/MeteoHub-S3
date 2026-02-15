#include "wifi_manager.h"
#include "secrets.h"
#include "config.h"
#include <WiFi.h>
#include "logs.h"

void WifiManager::begin() {
    WiFi.mode(WIFI_STA);
    lastAttempt = millis() - WIFI_RETRY_DELAY_MS;
}

void WifiManager::update() {
    if (WiFi.status() == WL_CONNECTED) return;

    unsigned long now = millis();
    if (now - lastAttempt < WIFI_RETRY_DELAY_MS) return;
    lastAttempt = now;

    for (size_t i = 0; i < WIFI_CREDENTIALS_COUNT; i++) {
        WiFi.begin(WIFI_CREDENTIALS[i].ssid, WIFI_CREDENTIALS[i].password);
        unsigned long t0 = millis();
        while (millis() - t0 < 3000) {
            if (WiFi.status() == WL_CONNECTED) {
                currentSSID = WIFI_CREDENTIALS[i].ssid;
                addLog("WiFi: " + currentSSID);
                return;
            }
            delay(100);
        }
    }
}

String WifiManager::ip() const {
    if (WiFi.status() != WL_CONNECTED) return "0.0.0.0";
    return WiFi.localIP().toString();
}

int WifiManager::rssi() const {
    if (WiFi.status() != WL_CONNECTED) return 0;
    return WiFi.RSSI();
}

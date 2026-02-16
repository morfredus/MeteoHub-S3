#include <string>
#include <Preferences.h>
#include <WiFi.h>

#include "ui_manager.h"
#include "board_config.h"
#include "config.h"
#include "../modules/neopixel_status.h"
#include "../modules/pages.h"
#include "../utils/logs.h"


void UiManager::begin(DisplayInterface& display, WifiManager& wifiRef, SensorManager& sensorRef, ForecastManager& forecastRef) {
    d = &display;
    wifi = &wifiRef;
    sensors = &sensorRef;
    forecast = &forecastRef;
    history.begin();
    enc.begin();

    pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);

    back_raw = digitalRead(BUTTON_BACK_PIN) == LOW;
    back_stable = back_raw;
    back_change_ms = millis();

    confirm_raw = digitalRead(BUTTON_CONFIRM_PIN) == LOW;
    confirm_stable = confirm_raw;
    confirm_change_ms = millis();

    // Forcer la page 1 (Meteo) au demarrage
    page = PAGE_WEATHER;

    // Initialiser le timer de la page prévisions
    lastForecastViewSwitch = millis();
}

void UiManager::update() {
    enc.update();
    wifi->update();
    history.update(sensors->read());
    forecast->update();
    handleButtons();

    // Gestion Alerte Température et LED
    static bool tempAlert = false;
    static unsigned long lastTempCheck = 0;
    static unsigned long lastBlink = 0;
    static bool blinkState = false;
    unsigned long now = millis();

    if (now - lastTempCheck > 2000) {
        lastTempCheck = now;
        SensorData data = sensors->read();
        if (data.valid && data.temperature > 28.0) tempAlert = true;
        else tempAlert = false;
    }

    bool weatherAlert = forecast->alert_active;

    if (weatherAlert) { // Priorité 1: Alerte météo
        if (now - lastBlink > 400) { // Clignotement plus rapide pour l'alerte
            lastBlink = now;
            blinkState = !blinkState;
            if (blinkState) {
                if (forecast->alert.severity == 3) neoAlertRed();
                else if (forecast->alert.severity == 2) neoAlertOrange();
                else neoAlertYellow();
            } else {
                neoOff();
            }
        }
    } else if (tempAlert) { // Priorité 2: Alerte température locale
        if (now - lastBlink > 500) {
            lastBlink = now;
            blinkState = !blinkState;
            if (blinkState) neoWifiKO(); else neoOff(); // Clignotement Rouge / Eteint
        }
    } else { // Statut par défaut
        if (WiFi.status() == WL_CONNECTED) neoWifiOK(); else neoWifiKO();
    }

    if (menuMode) {
        bool rotated = false;
        if (enc.rotatedCW())  { menuIndex++; rotated = true; }
        if (enc.rotatedCCW()) { menuIndex--; rotated = true; }

        if (rotated) {
            unsigned long now = millis();
            ignoreButtonsUntilMs = now + BUTTON_GUARD_MS;
            ignoreEncoderClickUntilMs = now + ENCODER_CLICK_GUARD_MS;
        }

        if (menuIndex < 0) menuIndex = MENU_COUNT - 1;
        if (menuIndex >= MENU_COUNT) menuIndex = 0;

        drawMenu();
        return;
    }

    // Auto-cycle pour la page prévisions
    if (page == PAGE_FORECAST) {
        if (now - lastForecastViewSwitch > 5000) {
            forecastViewIndex = (forecastViewIndex + 1) % 3;
            lastForecastViewSwitch = now;
            // Le rafraîchissement principal s'occupera de redessiner
        }
    }

    bool rotatedPage = false;
    if (enc.rotatedCW())  { page--; rotatedPage = true; }
    if (enc.rotatedCCW()) { page++; rotatedPage = true; }

    if (page < 0) page = PAGE_COUNT - 1;
    if (page >= PAGE_COUNT) page = 0;

    if (rotatedPage) {
        ignoreEncoderClickUntilMs = millis() + ENCODER_CLICK_GUARD_MS;
        lastRefresh = millis();
        
        Preferences prefs;
        prefs.begin("dash", false); // Mode écriture
        prefs.putInt("page", page);
        prefs.end();

        drawPage();
        return;
    }

    if (now - lastRefresh > DASHBOARD_REFRESH_MS) {
        lastRefresh = now;
        drawPage();
    }
}

void UiManager::handleButtons() {
    unsigned long now = millis();
    bool backPressed = digitalRead(BUTTON_BACK_PIN) == LOW;
    bool confirmPressed = digitalRead(BUTTON_CONFIRM_PIN) == LOW;
    bool encoderClickEvent = enc.clicked();
    if (now < ignoreEncoderClickUntilMs) {
        encoderClickEvent = false;
    }

    if (now < ignoreButtonsUntilMs) {
        back_raw = backPressed;
        back_stable = backPressed;
        back_change_ms = now;

        confirm_raw = confirmPressed;
        confirm_stable = confirmPressed;
        confirm_change_ms = now;
        return;
    }

    if (backPressed != back_raw) {
        back_raw = backPressed;
        back_change_ms = now;
    }

    if (confirmPressed != confirm_raw) {
        confirm_raw = confirmPressed;
        confirm_change_ms = now;
    }

    bool backEvent = false;
    bool confirmEvent = false;

    if ((now - back_change_ms) >= BUTTON_DEBOUNCE_MS && back_stable != back_raw) {
        back_stable = back_raw;
        if (back_stable) backEvent = true;
    }

    if ((now - confirm_change_ms) >= BUTTON_DEBOUNCE_MS && confirm_stable != confirm_raw) {
        confirm_stable = confirm_raw;
        if (confirm_stable) confirmEvent = true;
    }

    if (backEvent) {

        if (menuMode) {
            menuMode = false;
            drawPage();
        } else {
            page--;
            if (page < 0) page = PAGE_COUNT - 1;
            
            Preferences prefs;
            prefs.begin("dash", false);
            prefs.putInt("page", page);
            prefs.end();

            drawPage();
        }
    }

    if (confirmEvent && !menuMode) {
        menuMode = true;
        menuIndex = 0;
        ignoreButtonsUntilMs = now + BUTTON_GUARD_MS;
        drawMenu();
    }

    if (encoderClickEvent) {
        if (menuMode) {
            switch (menuIndex) {
                case MENU_EXIT:
                    menuMode = false;
                    drawPage();
                    break;
                case MENU_REBOOT:
                    ESP.restart();
                    break;
                case MENU_CLEAR_LOGS:
                    LOG_INFO("Logs cleared");
                    menuMode = false;
                    drawPage();
                    break;
                case MENU_CLEAR_HISTORY:
                    history.clear();
                    LOG_INFO("History cleared");
                    menuMode = false;
                    drawPage();
                    break;
            }
        } else {
            // Gérer les clics hors du menu (par page)
            if (page == PAGE_FORECAST) {
                forecastViewIndex = (forecastViewIndex + 1) % 3;
                lastForecastViewSwitch = now; // Réinitialiser le timer auto
                drawPage();
            }
        }
    }
}

void UiManager::drawMenu() {
    d->clear();
    d->text(0, 0, "Menu");

    const char* items[MENU_COUNT] = { "Retour", "Reboot", "Clear logs", "Clear History" };

    for (int i = 0; i < MENU_COUNT; i++) {
        std::string line = (i == menuIndex ? "> " : "  ");
        line += items[i];
        d->text(0, 14 + i * 12, line);
    }

    d->show();
}

void UiManager::drawPage() {
    switch (page) {
        case PAGE_WEATHER: pageWeather(*d, *sensors, page + 1, PAGE_COUNT); break;
        case PAGE_FORECAST: pageForecast(*d, *forecast, forecastViewIndex, page + 1, PAGE_COUNT); break;
        case PAGE_GRAPH_TEMP: pageGraph(*d, history, 0, page + 1, PAGE_COUNT); break;
        case PAGE_GRAPH_HUM:  pageGraph(*d, history, 1, page + 1, PAGE_COUNT); break;
        case PAGE_GRAPH_PRES: pageGraph(*d, history, 2, page + 1, PAGE_COUNT); break;
        case PAGE_NETWORK: pageNetwork(*d, *wifi, page + 1, PAGE_COUNT); break;
        case PAGE_LOGS:    pageLogs(*d, page + 1, PAGE_COUNT); break;
        case PAGE_SYSTEM:  pageSystem(*d, page + 1, PAGE_COUNT); break;
    }
}
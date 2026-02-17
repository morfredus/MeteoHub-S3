#include "managers/ui_manager.h"
#include "utils/logs.h"
#include <Arduino.h>
#include "board_config.h"
#include "config.h"

void UiManager::begin(DisplayInterface& display, WifiManager& wifiMgr, SensorManager& sensorMgr, ForecastManager& forecastMgr, HistoryManager& historyMgr) {
    d = &display;
    wifi = &wifiMgr;
    sensors = &sensorMgr;
    forecast = &forecastMgr;
    history = &historyMgr;

    enc.begin();
    
    // Configuration des boutons (si non gérés par Encoder)
    pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);

    page = 0; 
}

void UiManager::update() {
    // Gestion Encodeur
    enc.update();
    int diff = enc.getStepCount();
    if (diff != 0) {
        enc.clearQueue();

#if defined(ESP32_S3_OLED)
        diff = -diff;
#endif

        if (menuMode) {
#if defined(ESP32_S3_OLED)
            diff = -diff; // Inversion specifique pour le menu OLED (Horaire = Descendre)
#endif
            menuIndex += diff;
            if (menuIndex < 0) menuIndex = MENU_COUNT - 1;
            if (menuIndex >= MENU_COUNT) menuIndex = 0;
        } else {
            page += diff;
            if (page < 0) page = PAGE_COUNT - 1;
            if (page >= PAGE_COUNT) page = 0;
        }
        drawPage();
        lastRefresh = millis();
    }

    handleButtons();

    // Rafraîchissement auto
    if (millis() - lastRefresh > DASHBOARD_REFRESH_MS) {
        drawPage();
        lastRefresh = millis();
    }
    
    // Défilement auto Prévisions
    if (page == PAGE_FORECAST && !menuMode) {
        if (millis() - lastForecastViewSwitch > 5000) {
            forecastViewIndex = (forecastViewIndex + 1) % 3;
            lastForecastViewSwitch = millis();
            drawPage();
        }
    }
}

void UiManager::handleButtons() {
    // Lecture simple des boutons (Back / Confirm)
    bool btnBack = (digitalRead(BUTTON_BACK_PIN) == LOW);
    bool btnConfirm = (digitalRead(BUTTON_CONFIRM_PIN) == LOW);
    bool btnEncoder = enc.clicked();

    // Anti-rebond basique
    if (millis() < ignoreButtonsUntilMs) return;

    if (btnBack) {
        if (menuMode) {
            menuMode = false;
        } else {
            // Page précédente ou action spécifique
        }
        ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
        drawPage();
        return;
    }

    if (menuMode) {
        // Dans le menu, Confirm et Clic Encodeur valident l'option
        if (btnConfirm || btnEncoder) {
            // Action Menu
            switch(menuIndex) {
                case MENU_EXIT: 
                    menuMode = false; 
                    break;
                case MENU_REBOOT: 
                    ESP.restart(); 
                    break;
                case MENU_CLEAR_LOGS: 
                    // clearLogs(); // TODO: Implementer dans logs.h
                    menuMode = false;
                    break;
                case MENU_CLEAR_HISTORY: 
                    history->clear();
                    menuMode = false;
                    break;
            }
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
            drawPage();
        }
    } else {
        // Mode normal (hors menu)
        if (btnEncoder) {
            // Le clic encodeur ouvre le menu
            menuMode = true;
            menuIndex = 0;
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
            drawPage();
        } else if (btnConfirm) {
            // Le bouton Confirm est contextuel
            if (page == PAGE_FORECAST) {
                // Changement de vue prévisions
                forecastViewIndex = (forecastViewIndex + 1) % 3;
                lastForecastViewSwitch = millis();
                drawPage();
            }
            // Sur les autres pages, validation future (rien pour l'instant)
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
        }
    }
}

void UiManager::drawPage() {
    if (menuMode) {
        // drawMenu(); // Simplification pour l'exemple
        d->clear();
#if defined(ESP32_S3_OLED)
        // Layout compact pour OLED 128x64
        d->center(0, "MENU");
        d->text(0, 16, (menuIndex == MENU_EXIT) ? "> Retour" : "  Retour");
        d->text(0, 28, (menuIndex == MENU_REBOOT) ? "> Reboot" : "  Reboot");
        d->text(0, 40, (menuIndex == MENU_CLEAR_LOGS) ? "> Clear Logs" : "  Clear Logs");
        d->text(0, 52, (menuIndex == MENU_CLEAR_HISTORY) ? "> Clear Hist" : "  Clear Hist");
#else
        // Layout aere pour LCD
        d->center(20, "MENU");
        d->text(20, 60, (menuIndex == MENU_EXIT) ? "> Retour" : "  Retour");
        d->text(20, 90, (menuIndex == MENU_REBOOT) ? "> Reboot" : "  Reboot");
        d->text(20, 120, (menuIndex == MENU_CLEAR_LOGS) ? "> Clear Logs" : "  Clear Logs");
        d->text(20, 150, (menuIndex == MENU_CLEAR_HISTORY) ? "> Clear Hist" : "  Clear Hist");
#endif
        d->show();
        return;
    }

    int pCount = PAGE_COUNT;
#if defined(ESP32_S3_OLED)
    switch(page) {
        case PAGE_WEATHER: pageWeather_sh1106(*d, *sensors, page + 1, pCount); break;
        case PAGE_FORECAST: pageForecast_sh1106(*d, *forecast, forecastViewIndex, page + 1, pCount); break;
        case PAGE_GRAPH_TEMP: pageGraph_sh1106(*d, *history, 0, page + 1, pCount); break;
        case PAGE_GRAPH_HUM: pageGraph_sh1106(*d, *history, 1, page + 1, pCount); break;
        case PAGE_GRAPH_PRES: pageGraph_sh1106(*d, *history, 2, page + 1, pCount); break;
        case PAGE_NETWORK: pageNetwork_sh1106(*d, *wifi, page + 1, pCount); break;
        case PAGE_LOGS: pageLogs_sh1106(*d, page + 1, pCount); break;
        case PAGE_SYSTEM: pageSystem_sh1106(*d, page + 1, pCount); break;
    }
#elif defined(ESP32_S3_LCD)
    switch(page) {
        case PAGE_WEATHER: pageWeather_st7789(*d, *sensors, page + 1, pCount); break;
        case PAGE_FORECAST: pageForecast_st7789(*d, *forecast, forecastViewIndex, page + 1, pCount); break;
        case PAGE_GRAPH_TEMP: pageGraph_st7789(*d, *history, 0, page + 1, pCount); break;
        case PAGE_GRAPH_HUM: pageGraph_st7789(*d, *history, 1, page + 1, pCount); break;
        case PAGE_GRAPH_PRES: pageGraph_st7789(*d, *history, 2, page + 1, pCount); break;
        case PAGE_NETWORK: pageNetwork_st7789(*d, *wifi, page + 1, pCount); break;
        // Inversion Logs/System pour LCD (voir Changelog v1.0.29)
        case PAGE_LOGS: pageSystem_st7789(*d, page + 1, pCount); break;
        case PAGE_SYSTEM: pageLogs_st7789(*d, page + 1, pCount); break;
    }
#endif
}
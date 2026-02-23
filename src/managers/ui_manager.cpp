#include "managers/ui_manager.h"
#include "utils/logs.h"
#include <Arduino.h>
#include "board_config.h"
#include "config.h"
#include "modules/pages_oled.h"

void UiManager::begin(DisplayInterface& display, WifiManager& wifiMgr, SensorManager& sensorMgr, ForecastManager& forecastMgr, HistoryManager& historyMgr, SdManager& sdMgr) {
    d = &display;
    wifi = &wifiMgr;
    sensors = &sensorMgr;
    forecast = &forecastMgr;
    history = &historyMgr;
    sd = &sdMgr;

    enc.begin();
    
    // Configuration des boutons (si non gérés par Encoder)
    pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CONFIRM_PIN, INPUT_PULLUP);

    page = 0; 
    last_rendered_page = -1;
    last_rendered_menu_mode = false;
    last_rendered_confirm_mode = false;
}

void UiManager::update() {
    // Gestion Encodeur
    enc.update();
    int diff = enc.getStepCount();
    if (diff != 0) {
        enc.clearQueue();
        
        // Normalisation du sens sur OLED : rotation horaire = diff positif.
        diff = -diff;

        if (menuMode) {
            menuIndex -= diff; // Inversion du sens pour le menu (horaire = monter)
            if (menuIndex < 0) menuIndex = MENU_COUNT - 1;
            if (menuIndex >= MENU_COUNT) menuIndex = 0;

            const int MENU_VISIBLE_ITEMS = 4;
            if (menuIndex < menuScrollOffset) {
                menuScrollOffset = menuIndex;
            } else if (menuIndex >= menuScrollOffset + MENU_VISIBLE_ITEMS) {
                menuScrollOffset = menuIndex - MENU_VISIBLE_ITEMS + 1;
            }
        } else {
            page += diff;
            if (page < 0) page = PAGE_COUNT - 1;
            if (page >= PAGE_COUNT) page = 0;
            logScrollLine = 0; // Reset scroll logs au changement de page
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

    // --- Gestion des confirmations (Format, Logs, Hist) ---
    // On utilise une logique commune : Encoder/Confirm = OUI, Back = NON
    
    // 1. Confirmation Format SD
    if (confirmFormatMode) {
        if (btnEncoder || btnConfirm) {
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
            d->clear();
            int h = 64;
            d->center(h / 2 - 10, "Formatage SD...");
            d->center(h / 2 + 10, "Veuillez patienter.");
            d->show();
            
            bool success = sd->format();
            
            d->clear();
            if (success) {
                d->center(h / 2, "Formatage reussi !");
            } else {
                d->center(h / 2, "Echec du formatage.");
            }
            d->show();
            delay(2000);

            confirmFormatMode = false;
            drawPage();
            return;
        }
        if (btnBack) {
            confirmFormatMode = false;
            drawPage(); // Go back to normal menu
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
            return;
        }
        return; // Don't process other buttons
    }

    // 2. Confirmation Clear Logs
    if (confirmClearLogsMode) {
        if (btnEncoder || btnConfirm) {
            clearLogs();
            confirmClearLogsMode = false;
            d->clear();
            d->center(32, "Logs effaces !");
            d->show();
            delay(1000);
            drawPage();
        } else if (btnBack) {
            confirmClearLogsMode = false;
            drawPage();
        }
        if (btnEncoder || btnConfirm || btnBack) ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
        return;
    }

    // 3. Confirmation Clear History
    if (confirmClearHistMode) {
        if (btnEncoder || btnConfirm) {
            history->clearHistory();
            confirmClearHistMode = false;
            d->clear();
            d->center(32, "Historique efface !");
            d->show();
            delay(1000);
            drawPage();
        } else if (btnBack) {
            confirmClearHistMode = false;
            drawPage();
        }
        if (btnEncoder || btnConfirm || btnBack) ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
        return;
    }

    // --- Navigation normale ---

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
                    confirmClearLogsMode = true;
                    drawPage();
                    return;
                case MENU_CLEAR_HISTORY: 
                    confirmClearHistMode = true;
                    drawPage();
                    return;
                case MENU_FORMAT_SD:
                    confirmFormatMode = true;
                    ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
                    drawPage();
                    return; // Return to show confirmation screen immediately
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
            } else if (page == PAGE_LOGS) {
                // Défilement des logs
                logScrollLine++;
                // Si on dépasse la fin, on boucle au début
                if (logScrollLine >= getLogCount()) logScrollLine = 0;
                drawPage();
            }
            // Sur les autres pages, validation future (rien pour l'instant)
            ignoreButtonsUntilMs = millis() + BUTTON_GUARD_MS;
        }
    }
}

void UiManager::drawPage() {
    const bool current_confirm_mode = confirmClearLogsMode || confirmClearHistMode || confirmFormatMode;
    const bool screen_context_changed = (page != last_rendered_page) ||
        (menuMode != last_rendered_menu_mode) ||
        (current_confirm_mode != last_rendered_confirm_mode);

    if (screen_context_changed) {
        d->clear();
    }

    last_rendered_page = page;
    last_rendered_menu_mode = menuMode;
    last_rendered_confirm_mode = current_confirm_mode;
    // Ecrans de confirmation
    if (confirmClearLogsMode || confirmClearHistMode || confirmFormatMode) {
        d->clear();
        std::string title = "CONFIRMER";
        std::string msg = confirmFormatMode ? "Formater SD ?" : (confirmClearLogsMode ? "Effacer Logs ?" : "Effacer Hist ?");
        
        d->center(10, title);
        d->center(30, msg);
        d->text(0, 50, "Clic=OK, Back=Non");
        d->show();
        return;
    }

    if (confirmFormatMode) {
        d->clear();
        d->center(10, "CONFIRMER");
        d->center(30, "Formatage SD ?");
        d->text(0, 50, "Clic=OK, Back=Annuler");
        d->show();
        return;
    }

    if (menuMode) {
        // drawMenu(); // Simplification pour l'exemple
        d->clear();
        // Layout compact OLED 128x64
        const char* itemNames[] = { "Retour", "Reboot", "Clear Logs", "Clear Hist", "Format SD" };
        const int MENU_VISIBLE_ITEMS = 4;

        d->center(0, "MENU");

        for (int i = 0; i < MENU_VISIBLE_ITEMS; i++) {
            int itemIndex = menuScrollOffset + i;
            if (itemIndex >= MENU_COUNT) break;

            std::string line = (itemIndex == menuIndex) ? "> " : "  ";
            line += itemNames[itemIndex];
            d->text(0, 16 + i * 12, line);
        }
        d->show();
        return;
    }

    int pCount = PAGE_COUNT;
    switch(page) {
        case PAGE_WEATHER: pageWeather_oled(*d, *sensors, *forecast, page + 1, pCount); break;
        case PAGE_FORECAST: pageForecast_oled(*d, *forecast, forecastViewIndex, page + 1, pCount); break;
        case PAGE_GRAPH_TEMP: pageGraph_oled(*d, *history, 0, page + 1, pCount); break;
        case PAGE_GRAPH_HUM: pageGraph_oled(*d, *history, 1, page + 1, pCount); break;
        case PAGE_GRAPH_PRES: pageGraph_oled(*d, *history, 2, page + 1, pCount); break;
        case PAGE_NETWORK: pageNetwork_oled(*d, *wifi, page + 1, pCount); break;
        case PAGE_LOGS: pageLogs_oled(*d, page + 1, pCount, logScrollLine); break;
        case PAGE_SYSTEM: pageSystem_oled(*d, page + 1, pCount); break;
    }
}
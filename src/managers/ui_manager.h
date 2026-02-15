#pragma once
#include "../modules/sh1106_display.h"
#include "../modules/encoder.h"
#include "wifi_manager.h"
#include "../modules/sensors.h"
#include "history.h"
#include "forecast_manager.h"

class UiManager {
public:
    void begin(Sh1106Display& display, WifiManager& wifi, SensorManager& sensors, ForecastManager& forecast);
    void update();

private:
    static constexpr unsigned long BUTTON_DEBOUNCE_MS = 50;
    static constexpr unsigned long BUTTON_GUARD_MS = 150;
    static constexpr unsigned long ENCODER_CLICK_GUARD_MS = 220;

    enum PageId {
        PAGE_WEATHER,
        PAGE_FORECAST,
        PAGE_GRAPH_TEMP,
        PAGE_GRAPH_HUM,
        PAGE_GRAPH_PRES,
        PAGE_NETWORK,
        PAGE_LOGS,
        PAGE_SYSTEM,
        PAGE_COUNT // Vaut automatiquement 4, et passera a 5 si vous ajoutez une page avant
    };

    enum MenuAction {
        MENU_EXIT,
        MENU_REBOOT,
        MENU_CLEAR_LOGS,
        MENU_CLEAR_HISTORY,
        MENU_COUNT
    };

    Sh1106Display* d;
    WifiManager* wifi;
    SensorManager* sensors;
    HistoryManager history;
    ForecastManager* forecast;
    Encoder enc;

    int page = 0;
    unsigned long lastRefresh = 0;

    int forecastViewIndex = 0;
    unsigned long lastForecastViewSwitch = 0;

    bool menuMode = false;
    int menuIndex = 0;

    bool back_raw = false;
    bool back_stable = false;
    unsigned long back_change_ms = 0;

    bool confirm_raw = false;
    bool confirm_stable = false;
    unsigned long confirm_change_ms = 0;
    unsigned long ignoreButtonsUntilMs = 0;
    unsigned long ignoreEncoderClickUntilMs = 0;

    void drawPage();
    void handleButtons();
    void drawMenu();
};
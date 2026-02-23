#pragma once

#include "modules/display_interface.h"
#include "managers/wifi_manager.h"
#include "modules/sensors.h"
#include "managers/forecast_manager.h"
#include "managers/history_manager.h"
#include "managers/sd_manager.h"
#include "modules/encoder.h"
 
enum Page {
    PAGE_WEATHER,
    PAGE_FORECAST,
    PAGE_GRAPH_TEMP,
    PAGE_GRAPH_HUM,
    PAGE_GRAPH_PRES,
    PAGE_NETWORK,
    PAGE_SYSTEM,
    PAGE_LOGS,
    PAGE_COUNT
};

enum MenuItem {
    MENU_EXIT,
    MENU_REBOOT,
    MENU_CLEAR_LOGS,
    MENU_CLEAR_HISTORY,
    MENU_FORMAT_SD,
    MENU_COUNT
};

class UiManager {
public:
    void begin(DisplayInterface& display, WifiManager& wifiMgr, SensorManager& sensorMgr, ForecastManager& forecastMgr, HistoryManager& historyMgr, SdManager& sdMgr);
    void update();

private:
    DisplayInterface* d = nullptr;
    WifiManager* wifi = nullptr;
    SensorManager* sensors = nullptr;
    ForecastManager* forecast = nullptr;
    HistoryManager* history = nullptr;
    SdManager* sd = nullptr;
    Encoder enc;
    int page = 0;
    bool menuMode = false;
    int menuIndex = 0;
    int forecastViewIndex = 0;
    unsigned long lastRefresh = 0;
    unsigned long lastForecastViewSwitch = 0;
    unsigned long ignoreButtonsUntilMs = 0;
    int menuScrollOffset = 0;
    int logScrollLine = 0;
    bool confirmFormatMode = false;
    bool confirmClearLogsMode = false;
    bool confirmClearHistMode = false;
    int last_rendered_page = -1;
    bool last_rendered_menu_mode = false;
    bool last_rendered_confirm_mode = false;

    void handleButtons();
    void drawPage();
};
#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "managers/history_manager.h"
#include "managers/forecast_manager.h"

class WebManager {
public:
    WebManager();
    void begin(HistoryManager& history, SdManager& sd, ForecastManager& forecast);
    void handle();

private:
    AsyncWebServer _server;
    
    void _setupRoutes();
    void _setupApi();
    HistoryManager* _history = nullptr;
    SdManager* _sd = nullptr;
    ForecastManager* _forecast = nullptr;
};

#endif // WEB_MANAGER_H
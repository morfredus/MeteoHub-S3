#pragma once
#include <ESPAsyncWebServer.h>
#include "managers/history_manager.h"
#include "managers/sd_manager.h"

class WebManager {
public:
    WebManager();
    void begin(HistoryManager& history, SdManager& sd);
    void handle();

private:
    AsyncWebServer _server;
    HistoryManager* _history = nullptr;
    SdManager* _sd = nullptr;

    void _setupRoutes();
    void _setupApi();
};
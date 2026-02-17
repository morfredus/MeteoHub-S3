#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "managers/history_manager.h"

class WebManager {
public:
    WebManager();
    void begin(HistoryManager& history);
    void handle(); // Appelé dans loop() si nécessaire (pas requis pour AsyncWebServer mais utile pour maintenance)

private:
    AsyncWebServer _server;
    
    void _setupRoutes();
    void _setupApi();
    HistoryManager* _history;
};

#endif // WEB_MANAGER_H
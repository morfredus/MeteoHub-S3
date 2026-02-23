#pragma once
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <string>
 
class SdManager {
public:
    bool begin();
    bool format();
    bool isAvailable();
    bool ensureMounted();
    
    // Méthodes futures pour logs/historique
    // bool appendLog(const std::string& message);
    // bool saveHistory(const std::string& filename, const std::string& data);

private:
    bool _available = false;
    unsigned long _last_reconnect_attempt_ms = 0;
    unsigned long _reconnect_cooldown_ms = 15000;
    int _consecutive_reconnect_failures = 0;
    bool mountWithRetries();
};

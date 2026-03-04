#pragma once
#include <FS.h>
#include <SD_MMC.h>
#include <string>

// Alias SD → SD_MMC : assure la compatibilité de tous les fichiers utilisant SD.xxx()
// (history_manager, system_info, web_manager, etc.) sans les modifier.
#define SD SD_MMC

class SdManager {
public:
    bool begin();
    bool format();
    bool isAvailable();
    bool ensureMounted();

private:
    bool _available = false;
    unsigned long _last_reconnect_attempt_ms = 0;
    unsigned long _reconnect_cooldown_ms = 15000;
    int _consecutive_reconnect_failures = 0;
    bool mountWithRetries();
};

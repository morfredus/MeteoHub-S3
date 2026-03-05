#pragma once

#include <FS.h>
#include <SD.h>
#include <SD_MMC.h>
#include <string>

extern fs::FS* g_sd_fs;

// Alias SD vers le backend actif (SPI ou SDMMC).
#define SD (*g_sd_fs)

class SdManager {
public:
    bool begin();
    bool format();
    bool isAvailable();
    bool ensureMounted();

private:
    enum class Backend {
        none,
        spi,
        sdmmc
    };

    bool _available = false;
    Backend _backend = Backend::none;
    unsigned long _last_reconnect_attempt_ms = 0;
    unsigned long _reconnect_cooldown_ms = 15000;
    int _consecutive_reconnect_failures = 0;

    bool mountWithRetries();
    void unmountActiveBackend();
    bool cardIsPresent() const;
};

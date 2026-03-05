#pragma once

#include <FS.h>
#include <SD.h>
#include <SD_MMC.h>
#include <string>
 
class SdManager {
public:
    bool begin();
    bool format();
    bool isAvailable();
    bool ensureMounted();

    fs::FS* fs();
    uint64_t totalBytes() const;
    uint64_t usedBytes() const;
    uint64_t cardSize() const;
    std::string modeLabel() const;

private:
    enum class SdMode {
        None,
        Spi,
        SdMmc1Bit
    };

    bool _available = false;
    SdMode _mode = SdMode::None;
    unsigned long _last_reconnect_attempt_ms = 0;
    unsigned long _reconnect_cooldown_ms = 15000;
    int _consecutive_reconnect_failures = 0;

    bool mountWithRetries();
    bool mountSpiWithRetries();
    bool mountSdMmcWithRetries();
    bool mountSdMmcOnce(bool format_if_failed);
};

#pragma once

#include <FS.h>
#include <SD.h>
#include <SPI.h>

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
    SPIClass* _sd_spi = nullptr;

    bool mountWithRetries(bool formatIfFail);
    void ensureSpiInstance();
    void powerCycleIfSupported();
    bool isCardDetected() const;
    void logPinMapping() const;
};

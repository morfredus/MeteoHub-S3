#include "sd_manager.h"

#include "board_config.h"
#include "../utils/logs.h"

#include <algorithm>

#ifndef SD_CLK_PIN
#define SD_CLK_PIN 9
#endif

#ifndef SD_MISO_PIN
#define SD_MISO_PIN 10
#endif

#ifndef SD_MOSI_PIN
#define SD_MOSI_PIN 11
#endif

#ifndef SD_CS_PIN
#define SD_CS_PIN 12
#endif

#ifndef SD_DET_PIN
#define SD_DET_PIN -1
#endif

#ifndef SD_OFF_PIN
#define SD_OFF_PIN -1
#endif

namespace {
constexpr int SD_INIT_RETRY_COUNT = 4;
constexpr int SD_INIT_FREQUENCIES[] = {40000000, 20000000, 10000000, 4000000};
constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;
constexpr int SD_POWER_OFF_DELAY_MS = 30;
constexpr int SD_POWER_ON_STABILIZE_MS = 80;
}

void SdManager::ensureSpiInstance() {
    if (_sd_spi == nullptr) {
        _sd_spi = new SPIClass(FSPI);
    }
}

void SdManager::powerCycleIfSupported() {
    if (SD_OFF_PIN < 0) {
        return;
    }

    pinMode(SD_OFF_PIN, OUTPUT);
    digitalWrite(SD_OFF_PIN, LOW);
    delay(SD_POWER_OFF_DELAY_MS);
    digitalWrite(SD_OFF_PIN, HIGH);
    delay(SD_POWER_ON_STABILIZE_MS);
}

bool SdManager::isCardDetected() const {
    if (SD_DET_PIN < 0) {
        return true;
    }

    pinMode(SD_DET_PIN, INPUT_PULLUP);
    return digitalRead(SD_DET_PIN) == LOW;
}

void SdManager::logPinMapping() const {
    LOG_INFO(
        "SD pin mapping: CLK=" + std::to_string(SD_CLK_PIN) +
        " MISO/D0=" + std::to_string(SD_MISO_PIN) +
        " MOSI/CMD=" + std::to_string(SD_MOSI_PIN) +
        " CS/D3=" + std::to_string(SD_CS_PIN)
    );

    if (SD_DET_PIN >= 0) {
        LOG_INFO("SD card detect pin: GPIO " + std::to_string(SD_DET_PIN) + " (LOW=present)");
    }
}

bool SdManager::mountWithRetries(bool formatIfFail) {
    logPinMapping();

    if (!isCardDetected()) {
        LOG_WARNING("SD card detect reports no card inserted");
        _available = false;
        SD.end();
        return false;
    }

    ensureSpiInstance();

    for (int i = 0; i < SD_INIT_RETRY_COUNT; i++) {
        const int frequency_hz = SD_INIT_FREQUENCIES[i];

        SD.end();
        powerCycleIfSupported();
        _sd_spi->begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

        bool mounted = SD.begin(SD_CS_PIN, *_sd_spi, frequency_hz, "/sd", 5, formatIfFail);
        if (!mounted) {
            LOG_WARNING("SD mount failed at " + std::to_string(frequency_hz) + "Hz");
            delay(120);
            continue;
        }

        uint8_t card_type = SD.cardType();
        if (card_type == CARD_NONE) {
            LOG_WARNING("SD mount returned true but card type is NONE");
            SD.end();
            delay(120);
            continue;
        }

        if (!SD.exists("/history") && !SD.mkdir("/history")) {
            LOG_WARNING("SD mounted but failed to create /history directory");
        }

        LOG_INFO("SD mount OK at " + std::to_string(frequency_hz) + "Hz");
        _available = true;
        return true;
    }

    _available = false;
    return false;
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card...");

    _available = false;
    SD.end();

    bool ok = mountWithRetries(true);
    if (!ok) {
        LOG_ERROR("SD mount failed on all retries. Check wiring/card integrity.");
        return false;
    }

    LOG_INFO("SD Card OK. Size: " + std::to_string(SD.cardSize() / (1024 * 1024)) + "MB");

    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }

    if (!isCardDetected() || SD.cardType() == CARD_NONE) {
        LOG_WARNING("SD became unavailable");
        _available = false;
        SD.end();
        return ensureMounted();
    }

    return true;
}

bool SdManager::ensureMounted() {
    if (_available) {
        return true;
    }

    const unsigned long now = millis();
    if (now - _last_reconnect_attempt_ms < _reconnect_cooldown_ms) {
        return false;
    }

    _last_reconnect_attempt_ms = now;
    LOG_INFO("SD reconnect attempt... (cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms)");

    bool mounted = mountWithRetries(false);
    if (mounted) {
        _consecutive_reconnect_failures = 0;
        _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
        return true;
    }

    _consecutive_reconnect_failures++;
    _reconnect_cooldown_ms = std::min(
        SD_RECONNECT_COOLDOWN_MAX_MS,
        SD_RECONNECT_COOLDOWN_DEFAULT_MS * static_cast<unsigned long>(_consecutive_reconnect_failures + 1)
    );

    LOG_WARNING(
        "SD reconnect failed (count=" + std::to_string(_consecutive_reconnect_failures) +
        "), next cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms"
    );
    return false;
}

bool SdManager::format() {
    LOG_WARNING("Formatting SD Card...");

    SD.end();
    _available = false;

    // Comme dans le code validé: remonter avec format_if_fail=true
    bool ok = mountWithRetries(true);
    if (!ok) {
        LOG_ERROR("SD format/remount failed");
        return false;
    }

    if (!SD.exists("/history") && !SD.mkdir("/history")) {
        LOG_WARNING("SD format: failed to create /history directory");
    }

    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

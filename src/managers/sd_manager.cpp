#include "sd_manager.h"

#include "board_config.h"
#include "../utils/logs.h"

#include <Arduino.h>
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

#ifndef SD_DET_ACTIVE_LEVEL
#define SD_DET_ACTIVE_LEVEL LOW
#endif

namespace {
constexpr int SD_PRIMARY_FREQUENCY_HZ = 10000000;
constexpr int SD_RECONNECT_FREQUENCIES[] = {10000000, 4000000, 1000000};
constexpr int SD_FORMAT_FREQUENCY_HZ = 10000000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;
constexpr int SD_POWER_DOWN_DELAY_MS = 1000;
constexpr int SD_POST_BEGIN_DELAY_MS = 40;
constexpr int SD_MAX_OPEN_FILES = 5;
constexpr const char* SD_MOUNT_POINT = "/sd";
constexpr const char* SD_WRITE_TEST_FILE = "/sd_write_test.tmp";
}

void SdManager::logPinMapping() const {
    LOG_INFO(
        "SD pin mapping: CLK=" + std::to_string(SD_CLK_PIN) +
        " MISO/D0=" + std::to_string(SD_MISO_PIN) +
        " MOSI/CMD=" + std::to_string(SD_MOSI_PIN) +
        " CS/D3=" + std::to_string(SD_CS_PIN)
    );
}



bool SdManager::isCardDetected() const {
#ifdef SD_DET_PIN
    if (SD_DET_PIN >= 0) {
        pinMode(SD_DET_PIN, INPUT_PULLUP);
        int level = digitalRead(SD_DET_PIN);
        bool detected = (level == SD_DET_ACTIVE_LEVEL);
        LOG_INFO(
            "SD detect pin GPIO" + std::to_string(SD_DET_PIN) +
            " level=" + std::to_string(level) +
            " active=" + std::string(SD_DET_ACTIVE_LEVEL == LOW ? "LOW" : "HIGH") +
            " detected=" + std::string(detected ? "yes" : "no")
        );
        return detected;
    }
#endif
    return true;
}

void SdManager::resetSpiBus() {
    SD.end();
    delay(SD_POWER_DOWN_DELAY_MS);

    if (_sd_spi) {
        delete _sd_spi;
        _sd_spi = nullptr;
        delay(50);
    }

    _sd_spi = new SPIClass(FSPI);
    _sd_spi->begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
}

bool SdManager::mountAtFrequency(int frequency_hz, bool format_if_fail) {
    resetSpiBus();

    LOG_INFO(
        "SD mount attempt: " + std::to_string(frequency_hz) +
        "Hz, format_if_fail=" + std::string(format_if_fail ? "true" : "false")
    );

    bool mounted = SD.begin(SD_CS_PIN, *_sd_spi, frequency_hz, SD_MOUNT_POINT, SD_MAX_OPEN_FILES, format_if_fail);
    delay(SD_POST_BEGIN_DELAY_MS);

    if (!mounted) {
        LOG_WARNING("SD.begin failed at " + std::to_string(frequency_hz) + "Hz");
        return false;
    }

    if (SD.cardType() == CARD_NONE) {
        LOG_WARNING("SD.begin returned true but cardType is NONE");
        SD.end();
        return false;
    }

    return true;
}

bool SdManager::verifyWriteAccess() {
    File test_file = SD.open(SD_WRITE_TEST_FILE, FILE_WRITE);
    if (!test_file) {
        LOG_WARNING("SD write test: cannot open temp file (possible read-only card)");
        SD.remove(SD_WRITE_TEST_FILE);
        return false;
    }

    size_t written = test_file.println("TEST_OK");
    test_file.close();
    SD.remove(SD_WRITE_TEST_FILE);

    if (written == 0) {
        LOG_WARNING("SD write test failed: no bytes written");
        return false;
    }

    LOG_INFO("SD write test OK");
    return true;
}

void SdManager::ensureHistoryDirectory() {
    if (!SD.exists("/history") && !SD.mkdir("/history")) {
        LOG_WARNING("SD: failed to create /history");
    }
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card (stable 10MHz mode)...");
    logPinMapping();

    _available = false;
    SD.end();

    if (!isCardDetected()) {
        LOG_WARNING("SD detect indicates no card (non-blocking check)");
    }

    if (!mountAtFrequency(SD_PRIMARY_FREQUENCY_HZ, true)) {
        LOG_ERROR("SD mount failed at startup (10MHz)");
        return false;
    }

    ensureHistoryDirectory();
    verifyWriteAccess();

    LOG_INFO("SD Card OK. Size: " + std::to_string(SD.cardSize() / (1024 * 1024)) + "MB");

    _available = true;
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }

    if (SD.cardType() == CARD_NONE) {
        LOG_WARNING("SD became unavailable (cardType NONE)");
        _available = false;
        SD.end();
        return ensureMounted();
    }

    if (!isCardDetected()) {
        LOG_WARNING("SD detect indicates missing card while mounted; keeping SD available (detect likely inverted/noisy)");
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

    for (int frequency_hz : SD_RECONNECT_FREQUENCIES) {
        if (mountAtFrequency(frequency_hz, false)) {
            ensureHistoryDirectory();
            verifyWriteAccess();
            _available = true;
            _consecutive_reconnect_failures = 0;
            _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
            LOG_INFO("SD reconnect OK at " + std::to_string(frequency_hz) + "Hz");
            return true;
        }
        delay(120);
    }

    _available = false;
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
    LOG_WARNING("SD format requested (stable 10MHz mode)...");

    _available = false;

    if (!mountAtFrequency(SD_FORMAT_FREQUENCY_HZ, true)) {
        LOG_ERROR("SD format/remount failed");
        return false;
    }

    ensureHistoryDirectory();

    bool write_ok = verifyWriteAccess();
    if (!write_ok) {
        LOG_ERROR("SD format finished but write test failed (possible read-only card)");
        _available = false;
        return false;
    }

    _available = true;
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    LOG_INFO("SD format completed and verified");
    return true;
}

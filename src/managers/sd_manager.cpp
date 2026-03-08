#include "sd_manager.h"
#include "board_config.h"
#include "../utils/logs.h"
#include <Arduino.h>
#include <algorithm>

namespace {
    constexpr int SD_TARGET_FREQUENCY_HZ = 1000000;
    constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 30000;
    constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;
    constexpr int SD_POWER_DOWN_DELAY_MS = 500;
    constexpr int SD_POST_BEGIN_DELAY_MS = 100;
    constexpr int SD_MAX_OPEN_FILES = 5;
    constexpr const char* SD_MOUNT_POINT = "/sd";
    constexpr const char* SD_WRITE_TEST_FILE = "/sd_write_test.tmp";
}

void SdManager::logPinMapping() const {
    LOG_INFO(
        "SD pin mapping: CLK=" + std::to_string(SD_CLK_PIN) +
        " MISO=" + std::to_string(SD_MISO_PIN) +
        " MOSI=" + std::to_string(SD_MOSI_PIN) +
        " CS=" + std::to_string(SD_CS_PIN)
    );
}

bool SdManager::isCardDetected() const {
#ifdef SD_DET_PIN
    if (SD_DET_PIN >= 0) {
        int level = digitalRead(SD_DET_PIN);
        return (level == SD_DET_ACTIVE_LEVEL);
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
    delay(50);
}

bool SdManager::mountAtFrequency(int frequency_hz, bool format_if_fail) {
    resetSpiBus();
    LOG_INFO("SD mount attempt: " + std::to_string(frequency_hz) + "Hz");

    bool mounted = SD.begin(SD_CS_PIN, *_sd_spi, frequency_hz, SD_MOUNT_POINT, SD_MAX_OPEN_FILES, format_if_fail);
    delay(SD_POST_BEGIN_DELAY_MS);

    if (!mounted) {
        LOG_WARNING("SD.begin failed");
        return false;
    }

    if (SD.cardType() == CARD_NONE) {
        LOG_WARNING("SD cardType is NONE");
        SD.end();
        return false;
    }
    return true;
}

bool SdManager::verifyWriteAccess() {
    std::lock_guard<std::mutex> lock(_sd_mutex);

    File test_file = SD.open(SD_WRITE_TEST_FILE, FILE_WRITE);
    if (!test_file) {
        LOG_WARNING("SD write test: Cannot open file");
        return false;
    }

    const char* testContent = "MeteoHub Test";
    size_t written = test_file.println(testContent);
    
    // Correction : flush() appelé sans test de retour (peut être void)
    test_file.flush(); 
    test_file.close();

    if (written == 0) {
        LOG_WARNING("SD write test: 0 bytes");
        SD.remove(SD_WRITE_TEST_FILE);
        return false;
    }

    SD.remove(SD_WRITE_TEST_FILE);
    LOG_INFO("SD write test PASSED");
    return true;
}

bool SdManager::ensureHistoryDirectory() {
    std::lock_guard<std::mutex> lock(_sd_mutex);
    if (SD.exists("/history")) return true;
    if (SD.mkdir("/history")) return true;
    if (SD.exists("/sd/history")) return true;
    if (SD.mkdir("/sd/history")) return true;
    return false;
}

bool SdManager::begin() {
    LOG_INFO("=== SD Init ===");
    logPinMapping();
    _available = false;
    SD.end();

#ifdef SD_DET_PIN
    if (SD_DET_PIN >= 0) {
        pinMode(SD_DET_PIN, INPUT_PULLUP);
        delay(10);
        if (!isCardDetected()) LOG_WARNING("SD DET: No card");
    }
#endif

    if (!mountAtFrequency(SD_TARGET_FREQUENCY_HZ, true)) {
        LOG_ERROR("SD Mount FAILED");
        return false;
    }

    if (!ensureHistoryDirectory() || !verifyWriteAccess()) {
        LOG_ERROR("SD Mount OK but checks failed");
        SD.end();
        return false;
    }

    LOG_INFO("SD READY: " + std::to_string(SD.cardSize() / (1024 * 1024)) + " MB");
    _available = true;
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) return ensureMounted();
    if (SD.cardType() == CARD_NONE) {
        _available = false;
        SD.end();
        return ensureMounted();
    }
    return true;
}

bool SdManager::ensureMounted() {
    if (_available) return true;
    const unsigned long now = millis();
    if (now - _last_reconnect_attempt_ms < _reconnect_cooldown_ms) return false;

    _last_reconnect_attempt_ms = now;
    if (mountAtFrequency(SD_TARGET_FREQUENCY_HZ, false)) {
        if (ensureHistoryDirectory() && verifyWriteAccess()) {
            _available = true;
            _consecutive_reconnect_failures = 0;
            _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
            return true;
        }
        SD.end();
    }
    
    _consecutive_reconnect_failures++;
    _reconnect_cooldown_ms = std::min(SD_RECONNECT_COOLDOWN_MAX_MS, 
        (unsigned long)(SD_RECONNECT_COOLDOWN_DEFAULT_MS * (_consecutive_reconnect_failures + 1)));
    return false;
}

bool SdManager::format() {
    LOG_WARNING("=== SD Format ===");
    _available = false;
    SD.end();
    delay(500);
    if (!mountAtFrequency(SD_TARGET_FREQUENCY_HZ, true)) return false;
    if (!ensureHistoryDirectory() || !verifyWriteAccess()) {
        _available = false;
        SD.end();
        return false;
    }
    _available = true;
    return true;
}

bool SdManager::openFileSafe(const char* path, const char* mode, File& out_file) {
    std::lock_guard<std::mutex> lock(_sd_mutex);
    if (!isAvailable()) return false;
    out_file = SD.open(path, mode);
    return (bool)out_file;
}

void SdManager::closeFileSafe(File& file) {
    if (file) {
        file.flush();
        file.close();
    }
}
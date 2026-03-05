#include "sd_manager.h"

#include "../utils/logs.h"
#include "board_config.h"

#include <algorithm>
#include <SPI.h>

#ifndef SD_SPI_CS_PIN
#define SD_SPI_CS_PIN 10
#endif
#ifndef SD_SPI_SCK_PIN
#define SD_SPI_SCK_PIN 12
#endif
#ifndef SD_SPI_MISO_PIN
#define SD_SPI_MISO_PIN 13
#endif
#ifndef SD_SPI_MOSI_PIN
#define SD_SPI_MOSI_PIN 11
#endif

#ifndef SDMMC_CLK_PIN
#define SDMMC_CLK_PIN 12
#endif
#ifndef SDMMC_CMD_PIN
#define SDMMC_CMD_PIN 11
#endif
#ifndef SDMMC_D0_PIN
#define SDMMC_D0_PIN 13
#endif
#ifndef SDMMC_D1_PIN
#define SDMMC_D1_PIN -1
#endif
#ifndef SDMMC_D2_PIN
#define SDMMC_D2_PIN -1
#endif
#ifndef SDMMC_D3_PIN
#define SDMMC_D3_PIN -1
#endif

namespace {
constexpr int SD_SPI_RETRY_COUNT = 4;
constexpr int SD_SPI_FREQUENCIES[] = {20000000, 10000000, 4000000, 1000000};
constexpr int SD_SDMMC_RETRY_COUNT = 3;
constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;

void logPinMapping() {
    LOG_INFO(
        "SD SPI mapping: CS=" + std::to_string(SD_SPI_CS_PIN) +
        " SCK=" + std::to_string(SD_SPI_SCK_PIN) +
        " MISO(D0/SO)=" + std::to_string(SD_SPI_MISO_PIN) +
        " MOSI(CMD/SI)=" + std::to_string(SD_SPI_MOSI_PIN)
    );

    LOG_INFO(
        "SD SDMMC [1-bit] mapping: CLK=" + std::to_string(SDMMC_CLK_PIN) +
        " CMD=" + std::to_string(SDMMC_CMD_PIN) +
        " DAT0=" + std::to_string(SDMMC_D0_PIN)
    );
}
}

bool SdManager::mountSpiWithRetries() {
    pinMode(SD_SPI_CS_PIN, OUTPUT);
    digitalWrite(SD_SPI_CS_PIN, HIGH);
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);

    for (int i = 0; i < SD_SPI_RETRY_COUNT; i++) {
        const int frequency_hz = SD_SPI_FREQUENCIES[i];
        SD.end();

        if (SD.begin(SD_SPI_CS_PIN, SPI, frequency_hz)) {
            if (SD.cardType() != CARD_NONE) {
                if (!SD.exists("/history")) {
                    SD.mkdir("/history");
                }
                _mode = SdMode::Spi;
                _available = true;
                LOG_INFO("SD SPI mount OK @ " + std::to_string(frequency_hz) + "Hz");
                return true;
            }
            SD.end();
        }

        LOG_WARNING("SD SPI mount failed @ " + std::to_string(frequency_hz) + "Hz");
        delay(120);
    }

    _available = false;
    return false;
}

bool SdManager::mountSdMmcOnce(bool format_if_failed) {
    SD_MMC.end();

    if (!SD_MMC.setPins(SDMMC_CLK_PIN, SDMMC_CMD_PIN, SDMMC_D0_PIN, SDMMC_D1_PIN, SDMMC_D2_PIN, SDMMC_D3_PIN)) {
        LOG_WARNING("SD_MMC setPins failed");
        return false;
    }

    if (!SD_MMC.begin("/sdcard", true, format_if_failed)) {
        return false;
    }

    if (SD_MMC.cardType() == CARD_NONE) {
        SD_MMC.end();
        return false;
    }

    if (!SD_MMC.exists("/history")) {
        SD_MMC.mkdir("/history");
    }

    _mode = SdMode::SdMmc1Bit;
    _available = true;
    return true;
}

bool SdManager::mountSdMmcWithRetries() {
    LOG_INFO("SD: fallback SD_MMC 1-bit...");

    for (int i = 0; i < SD_SDMMC_RETRY_COUNT; i++) {
        if (mountSdMmcOnce(false)) {
            LOG_INFO("SD_MMC 1-bit mount OK (attempt " + std::to_string(i + 1) + "/" + std::to_string(SD_SDMMC_RETRY_COUNT) + ")");
            return true;
        }

        LOG_WARNING("SD_MMC 1-bit mount failed (attempt " + std::to_string(i + 1) + ")");
        delay(120);
    }

    _available = false;
    return false;
}

bool SdManager::mountWithRetries() {
    logPinMapping();

    if (mountSpiWithRetries()) {
        return true;
    }

    return mountSdMmcWithRetries();
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card (SPI primary, SD_MMC fallback)...");

    _available = false;
    _mode = SdMode::None;

    bool ok = mountWithRetries();
    if (!ok) {
        LOG_ERROR("SD mount failed on all modes. Check CLK/CMD-DI/D0-SO/D3-CS wiring and card integrity.");
        return false;
    }

    LOG_INFO("SD Card OK. Mode=" + modeLabel() + " Size: " + std::to_string(cardSize() / (1024 * 1024)) + "MB");
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }

    bool card_present = false;
    if (_mode == SdMode::Spi) {
        card_present = (SD.cardType() != CARD_NONE);
    } else if (_mode == SdMode::SdMmc1Bit) {
        card_present = (SD_MMC.cardType() != CARD_NONE);
    }

    if (!card_present) {
        LOG_WARNING("SD became unavailable");
        _available = false;
        SD.end();
        SD_MMC.end();
        _mode = SdMode::None;
        return ensureMounted();
    }

    return true;
}

bool SdManager::ensureMounted() {
    if (_available) {
        return true;
    }

    unsigned long now = millis();
    if (now - _last_reconnect_attempt_ms < _reconnect_cooldown_ms) {
        return false;
    }

    _last_reconnect_attempt_ms = now;
    LOG_INFO("SD reconnect attempt... (cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms)");

    bool mounted = mountWithRetries();
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

    LOG_WARNING("SD reconnect failed (count=" + std::to_string(_consecutive_reconnect_failures) + "), next cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms");
    return false;
}

bool SdManager::format() {
    LOG_WARNING("Formatting SD Card...");

    SD.end();
    SD_MMC.end();
    _available = false;
    _mode = SdMode::None;

    if (!mountSdMmcOnce(true)) {
        LOG_ERROR("SD format/remount failed");
        return false;
    }

    if (!SD_MMC.exists("/history") && !SD_MMC.mkdir("/history")) {
        LOG_WARNING("SD format: failed to create /history directory");
    }

    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

fs::FS* SdManager::fs() {
    if (!_available) {
        return nullptr;
    }

    if (_mode == SdMode::Spi) {
        return &SD;
    }

    if (_mode == SdMode::SdMmc1Bit) {
        return &SD_MMC;
    }

    return nullptr;
}

uint64_t SdManager::totalBytes() const {
    if (!_available) return 0;
    if (_mode == SdMode::Spi) return SD.totalBytes();
    if (_mode == SdMode::SdMmc1Bit) return SD_MMC.totalBytes();
    return 0;
}

uint64_t SdManager::usedBytes() const {
    if (!_available) return 0;
    if (_mode == SdMode::Spi) return SD.usedBytes();
    if (_mode == SdMode::SdMmc1Bit) return SD_MMC.usedBytes();
    return 0;
}

uint64_t SdManager::cardSize() const {
    if (!_available) return 0;
    if (_mode == SdMode::Spi) return SD.cardSize();
    if (_mode == SdMode::SdMmc1Bit) return SD_MMC.cardSize();
    return 0;
}

std::string SdManager::modeLabel() const {
    if (_mode == SdMode::Spi) {
        return "SPI";
    }
    if (_mode == SdMode::SdMmc1Bit) {
        return "SD_MMC_1BIT";
    }
    return "NONE";
}

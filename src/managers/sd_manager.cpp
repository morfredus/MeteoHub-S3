#include "sd_manager.h"

#include "../utils/logs.h"
#include "board_config.h"

#include <algorithm>

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
#ifndef SDMMC_CD_PIN
#define SDMMC_CD_PIN 10
#endif

namespace {
constexpr int SD_MOUNT_RETRY_COUNT = 4;
constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;

void logSdPinMapping() {
    LOG_INFO(
        "SD_MMC pin mapping: CLK=" + std::to_string(SDMMC_CLK_PIN) +
        " CMD=" + std::to_string(SDMMC_CMD_PIN) +
        " D0=" + std::to_string(SDMMC_D0_PIN) +
        " D1=" + std::to_string(SDMMC_D1_PIN) +
        " D2=" + std::to_string(SDMMC_D2_PIN) +
        " D3=" + std::to_string(SDMMC_D3_PIN)
    );

    if (SDMMC_CD_PIN >= 0) {
        LOG_INFO("SD_MMC CD pin configured on GPIO " + std::to_string(SDMMC_CD_PIN));
    } else {
        LOG_INFO("SD_MMC CD pin not used");
    }
}
}

bool SdManager::mountOnce(bool format_if_failed) {
    SD_MMC.end();

    if (!SD_MMC.setPins(SDMMC_CLK_PIN, SDMMC_CMD_PIN, SDMMC_D0_PIN, SDMMC_D1_PIN, SDMMC_D2_PIN, SDMMC_D3_PIN)) {
        LOG_ERROR("SD_MMC setPins failed");
        _available = false;
        return false;
    }

    // 1-bit bus mode for sniffer-style modules exposing DAT0/CMD/CLK.
    if (!SD_MMC.begin("/sdcard", true, format_if_failed)) {
        _available = false;
        return false;
    }

    if (SD_MMC.cardType() == CARD_NONE) {
        LOG_WARNING("SD_MMC mounted but card type is NONE");
        SD_MMC.end();
        _available = false;
        return false;
    }

    if (!SD_MMC.exists("/history")) {
        SD_MMC.mkdir("/history");
    }

    _available = true;
    return true;
}

bool SdManager::mountWithRetries() {
    logSdPinMapping();

    for (int i = 0; i < SD_MOUNT_RETRY_COUNT; i++) {
        if (mountOnce(false)) {
            LOG_INFO("SD_MMC mount OK (attempt " + std::to_string(i + 1) + "/" + std::to_string(SD_MOUNT_RETRY_COUNT) + ")");
            return true;
        }
        delay(120);
    }

    _available = false;
    return false;
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card (SD_MMC 1-bit)...");

    _available = false;
    bool ok = mountWithRetries();
    if (!ok) {
        LOG_ERROR("SD_MMC mount failed on all retries. Check DAT0/CMD/CLK wiring and card integrity.");
        return false;
    }

    LOG_INFO("SD Card OK. Size: " + std::to_string(SD_MMC.cardSize() / (1024 * 1024)) + "MB");
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }

    if (SD_MMC.cardType() == CARD_NONE) {
        LOG_WARNING("SD became unavailable");
        _available = false;
        SD_MMC.end();
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
    LOG_WARNING("Formatting SD Card (SD_MMC)...");

    SD_MMC.end();
    _available = false;

    // For SD_MMC, request format if mount fails.
    if (!mountOnce(true)) {
        LOG_ERROR("SD_MMC format/remount failed");
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

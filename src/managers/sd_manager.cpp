#include "sd_manager.h"
#include "../utils/logs.h"
#include "board_config.h"

#include <algorithm>
#include <memory>

// Valeurs par défaut si non définies dans board_config.h
#ifndef SD_CS_PIN
#define SD_CS_PIN 12
#endif
#ifndef SD_CLK_PIN
#define SD_CLK_PIN 9
#endif
#ifndef SD_MISO_PIN
#define SD_MISO_PIN 10
#endif
#ifndef SD_MOSI_PIN
#define SD_MOSI_PIN 11
#endif
#ifndef SD_DET_PIN
#define SD_DET_PIN -1
#endif

namespace {

constexpr uint32_t SD_FREQUENCY_HZ              = 40000000UL; // 40 MHz — identique au code de référence
constexpr const char* SD_MOUNT_POINT            = "/sd";
constexpr uint8_t    SD_MAX_FILES               = 5;
constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS     = 120000;

bool hasSdDetectionPin() {
    return SD_DET_PIN >= 0;
}

void initSdDetectionPin() {
    if (!hasSdDetectionPin()) {
        return;
    }
    pinMode(SD_DET_PIN, INPUT_PULLUP);
}

bool isCardPhysicallyPresent() {
    if (!hasSdDetectionPin()) {
        return true; // Sans pin de détection, on suppose la carte présente
    }
    // LOW = carte insérée (contact fermé vers GND)
    return digitalRead(SD_DET_PIN) == LOW;
}

void logSdPinMapping() {
    LOG_INFO(
        "SD pin mapping: CS=" + std::to_string(SD_CS_PIN) +
        " CLK=" + std::to_string(SD_CLK_PIN) +
        " MISO=" + std::to_string(SD_MISO_PIN) +
        " MOSI=" + std::to_string(SD_MOSI_PIN)
    );
    if (hasSdDetectionPin()) {
        LOG_INFO("SD DET pin: GPIO " + std::to_string(SD_DET_PIN) + " (LOW=card present)");
    } else {
        LOG_INFO("SD DET pin: not configured (presence always assumed)");
    }
}

} // namespace


bool SdManager::mountWithRetries() {
    initSdDetectionPin();
    logSdPinMapping();

    if (!isCardPhysicallyPresent()) {
        LOG_WARNING("SD card not physically present (DET pin HIGH)");
        _available = false;
        return false;
    }

    SD.end();

    // Initialisation FSPI avec les broches câblées — même approche que le code de référence
    _spi = std::make_unique<SPIClass>(FSPI);
    _spi->begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    // format_if_fail=true : formate automatiquement si la carte n'est pas reconnue comme valide
    if (!SD.begin(SD_CS_PIN, *_spi, SD_FREQUENCY_HZ, SD_MOUNT_POINT, SD_MAX_FILES, true)) {
        LOG_ERROR("SD mount failed. Verifiez le cablage et l'alimentation 3.3V.");
        _available = false;
        return false;
    }

    if (SD.cardType() == CARD_NONE) {
        LOG_WARNING("SD mount reported OK but card type is NONE");
        SD.end();
        _available = false;
        return false;
    }

    if (!SD.exists("/history")) {
        SD.mkdir("/history");
    }

    _available = true;
    return true;
}


bool SdManager::begin() {
    LOG_INFO("Init SD Card...");
    _available = false;

    bool ok = mountWithRetries();
    if (!ok) {
        LOG_ERROR("SD mount failed. Verifiez le cablage ou lancez un formatage.");
        return false;
    }

    uint8_t card_type = SD.cardType();
    std::string type_str = "UNKNOWN";
    if (card_type == CARD_MMC)       type_str = "MMC";
    else if (card_type == CARD_SD)   type_str = "SD";
    else if (card_type == CARD_SDHC) type_str = "SDHC";

    LOG_INFO("SD Card OK. Type: " + type_str + " / Size: " + std::to_string(SD.cardSize() / (1024 * 1024)) + "MB");

    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}


bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }

    if (hasSdDetectionPin() && !isCardPhysicallyPresent()) {
        LOG_WARNING("SD card physically removed (DET pin HIGH)");
        _available = false;
        SD.end();
        return false;
    }

    if (SD.cardType() == CARD_NONE) {
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

    LOG_WARNING(
        "SD reconnect failed (count=" + std::to_string(_consecutive_reconnect_failures) +
        "), next cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms"
    );
    return false;
}


bool SdManager::format() {
    LOG_WARNING("Formatting SD Card...");

    if (_available) {
        SD.end();
        _available = false;
    }

    delay(1000); // Laisser la carte se libérer — identique au code de référence

    // Réinitialisation FSPI puis montage avec format_if_fail=true
    _spi = std::make_unique<SPIClass>(FSPI);
    _spi->begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    bool ok = SD.begin(SD_CS_PIN, *_spi, SD_FREQUENCY_HZ, SD_MOUNT_POINT, SD_MAX_FILES, true);

    if (!ok) {
        LOG_ERROR("SD format/remount failed. Essayez de formater sur un PC.");
        return false;
    }

    LOG_INFO("SD formatted and remounted OK.");
    _available = true;
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;

    if (!SD.exists("/history") && !SD.mkdir("/history")) {
        LOG_WARNING("SD format: failed to create /history directory");
    }

    return true;
}

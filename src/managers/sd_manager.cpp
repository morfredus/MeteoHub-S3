#include "sd_manager.h"

#include <algorithm>
#include <driver/gpio.h>
#include <ff.h>

#include "../utils/logs.h"
#include "board_config.h"

// Déclarations des fonctions internes de la librairie SD (sd_diskio.cpp)
// Nécessaire pour initialiser la carte sans la monter (ce qui échoue si non formatée)
uint8_t sdcard_init(uint8_t cs, SPIClass* spi, int hz);
bool sdcard_uninit(uint8_t pdrv);

namespace {

// =================================================================
// 1) Constantes de robustesse SD (init, format, supervision)
// =================================================================
#if defined(SD_ULTRA_SAFE_DEBUG)
constexpr int SD_INIT_FREQUENCIES[] = {400000, 250000};
constexpr int SD_FORMAT_FREQUENCIES[] = {250000, 400000};
#else
constexpr int SD_INIT_FREQUENCIES[] = {8000000, 4000000, 1000000, 400000};
constexpr int SD_FORMAT_FREQUENCIES[] = {400000, 1000000, 4000000};
#endif

constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;
constexpr unsigned long SD_HEALTH_CHECK_PERIOD_MS = 5000;

constexpr int SD_POWER_OFF_DELAY_MS = 50;
constexpr int SD_POWER_ON_STABILIZE_MS = 200;
#if defined(SD_ULTRA_SAFE_DEBUG)
constexpr int SD_WAKEUP_CLOCK_BYTES = 64; // 64 * 8 = 512 cycles SPI
#else
constexpr int SD_WAKEUP_CLOCK_BYTES = 20; // 20 * 8 = 160 cycles SPI
#endif

#ifndef SD_OFF_PIN
#define SD_OFF_PIN -1
#endif

// =================================================================
// 2) Helpers GPIO/SPI : alimentation, bus, réinitialisation
// =================================================================
bool hasSdOffControl() {
    return SD_OFF_PIN >= 0;
}


const char* sdCardTypeToString(uint8_t card_type) {
    switch (card_type) {
        case CARD_MMC: return "MMC";
        case CARD_SD: return "SDSC";
        case CARD_SDHC: return "SDHC/SDXC";
        case CARD_NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

void logSdDebugPinsState() {
    const int cs_state = digitalRead(SD_CS_PIN);
    const int sck_state = digitalRead(SD_SCK_PIN);
    const int mosi_state = digitalRead(SD_MOSI_PIN);
    const int miso_state = digitalRead(SD_MISO_PIN);

    LOG_DEBUG(
        "SD pin states (pre-mount): CS=" + std::to_string(cs_state) +
        ", SCK=" + std::to_string(sck_state) +
        ", MOSI=" + std::to_string(mosi_state) +
        ", MISO=" + std::to_string(miso_state));
}

void setSdModulePower(bool power_on) {
    if (!hasSdOffControl()) {
        return;
    }

    pinMode(SD_OFF_PIN, OUTPUT);
    digitalWrite(SD_OFF_PIN, power_on ? HIGH : LOW);
}

void floatSdLines() {
    pinMode(SD_SCK_PIN, INPUT);
    pinMode(SD_MOSI_PIN, INPUT);
    pinMode(SD_CS_PIN, INPUT);
    pinMode(SD_MISO_PIN, INPUT);
}

void initializeSpiBus() {
    pinMode(SD_SCK_PIN, OUTPUT);
    pinMode(SD_MOSI_PIN, OUTPUT);
    pinMode(SD_CS_PIN, OUTPUT);
    pinMode(SD_MISO_PIN, INPUT_PULLUP);
    digitalWrite(SD_CS_PIN, HIGH);

#if defined(SD_ULTRA_SAFE_DEBUG)
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_SCK_PIN), GPIO_DRIVE_CAP_0);
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_MOSI_PIN), GPIO_DRIVE_CAP_0);
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_CS_PIN), GPIO_DRIVE_CAP_0);
#else
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_SCK_PIN), GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_MOSI_PIN), GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability(static_cast<gpio_num_t>(SD_CS_PIN), GPIO_DRIVE_CAP_3);
#endif

    SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
    LOG_INFO("SD SPI bus initialized (CS=" + std::to_string(SD_CS_PIN) +
             ", SCK=" + std::to_string(SD_SCK_PIN) +
             ", MISO=" + std::to_string(SD_MISO_PIN) +
             ", MOSI=" + std::to_string(SD_MOSI_PIN) + ")");
}

void sendWakeupClocks() {
    digitalWrite(SD_CS_PIN, HIGH);
    for (int i = 0; i < SD_WAKEUP_CLOCK_BYTES; ++i) {
        SPI.transfer(0xFF);
    }
    delay(10);
}

void powerCycleSdModule() {
    if (!hasSdOffControl()) {
        return;
    }

    SD.end();
    SPI.end();
    floatSdLines();

    setSdModulePower(false);
    delay(SD_POWER_OFF_DELAY_MS);
    setSdModulePower(true);
    delay(SD_POWER_ON_STABILIZE_MS);
}

// =================================================================
// 3) Helpers SD : diagnostic et montage
// =================================================================
void logSdPinMapping() {
    LOG_INFO(
        "SD pin mapping: CS=" + std::to_string(SD_CS_PIN) +
        ", SCK=" + std::to_string(SD_SCK_PIN) +
        ", MISO=" + std::to_string(SD_MISO_PIN) +
        ", MOSI=" + std::to_string(SD_MOSI_PIN));

#if defined(SD_ULTRA_SAFE_DEBUG)
    LOG_WARNING("SD ultra-safe debug mode enabled (very low SPI speeds + stronger diagnostics)");
#else
    LOG_INFO("SD normal mode enabled");
#endif

    if (hasSdOffControl()) {
        LOG_INFO("SD power control enabled on GPIO " + std::to_string(SD_OFF_PIN));
    } else {
        LOG_INFO("SD power control disabled (SD_OFF_PIN not defined)");
    }
}

bool tryMountAtFrequency(int frequency_hz, int attempt_index, int total_attempts) {
    LOG_INFO("[1/4] SD mount attempt #" + std::to_string(attempt_index) + "/" + std::to_string(total_attempts) +
             " at " + std::to_string(frequency_hz / 1000) + " kHz");
    logSdDebugPinsState();

    if (!SD.begin(SD_CS_PIN, SPI, frequency_hz)) {
        LOG_WARNING("SD mount failed at " + std::to_string(frequency_hz / 1000) + " kHz");
        return false;
    }

    const uint8_t card_type = SD.cardType();
    LOG_INFO("SD card type after begin: " + std::string(sdCardTypeToString(card_type)) +
             " (" + std::to_string(card_type) + ")");
    if (card_type == CARD_NONE) {
        LOG_WARNING("SD mount reported OK but card type is NONE. Unmounting.");
        SD.end();
        return false;
    }

    const uint64_t card_size_mb = SD.cardSize() / (1024ULL * 1024ULL);
    LOG_INFO("SD mount OK at " + std::to_string(frequency_hz / 1000) + " kHz (size=" + std::to_string(card_size_mb) + "MB)");
    return true;
}

bool tryLowLevelFormat(int frequency_hz) {
    sendWakeupClocks();

    uint8_t pdrv = sdcard_init(SD_CS_PIN, &SPI, frequency_hz);
    if (pdrv == 0xFF) {
        LOG_WARNING("SD format: low-level init failed at " + std::to_string(frequency_hz / 1000) + " kHz");
        return false;
    }

    char drv[4];
    snprintf(drv, sizeof(drv), "%d:", pdrv);

    uint8_t* work_buffer = static_cast<uint8_t*>(malloc(FF_MAX_SS));
    if (work_buffer == nullptr) {
        LOG_ERROR("SD format: failed to allocate FatFS work buffer");
        sdcard_uninit(pdrv);
        return false;
    }

    LOG_WARNING("[2/4] SD low-level format in progress...");
    const FRESULT format_result = f_mkfs(drv, FM_ANY, 0, work_buffer, FF_MAX_SS);

    free(work_buffer);
    sdcard_uninit(pdrv);

    if (format_result != FR_OK) {
        LOG_WARNING("SD format failed with f_mkfs error code: " + std::to_string(format_result));
        return false;
    }

    LOG_INFO("SD low-level format completed successfully.");
    return true;
}

} // namespace

bool SdManager::begin() {
    LOG_INFO("Initializing SD Card...");
    logSdPinMapping();

    setSdModulePower(true);
    delay(SD_POWER_ON_STABILIZE_MS);

    if (!mountWithRetries()) {
        LOG_ERROR("SD mount failed on all retries. Check wiring/card or run format.");
        return false;
    }

    _last_reconnect_attempt_ms = millis();
    _last_health_check_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;

    return ensureHistoryDirectory();
}

bool SdManager::mountWithRetries() {
    SD.end();
    SPI.end();
    _available = false;

    powerCycleSdModule();
    initializeSpiBus();

    const int total_attempts = static_cast<int>(sizeof(SD_INIT_FREQUENCIES) / sizeof(SD_INIT_FREQUENCIES[0]));
    for (int index = 0; index < total_attempts; ++index) {
        const int frequency_hz = SD_INIT_FREQUENCIES[index];
        sendWakeupClocks();

        if (tryMountAtFrequency(frequency_hz, index + 1, total_attempts)) {
            _available = true;
            return true;
        }

        SD.end();
        delay(100);
    }

    return false;
}

bool SdManager::healthCheck() {
    if (!_available) {
        return false;
    }

    const uint8_t card_type = SD.cardType();
    if (card_type == CARD_NONE) {
        LOG_WARNING("SD health check failed: cardType is NONE.");
        _available = false;
        SD.end();
        return false;
    }

    File root = SD.open("/");
    if (!root || !root.isDirectory()) {
        LOG_WARNING("SD health check failed: unable to open root directory.");
        if (root) {
            root.close();
        }
        _available = false;
        SD.end();
        return false;
    }

    root.close();
    LOG_DEBUG("SD health check OK (type=" + std::string(sdCardTypeToString(card_type)) + ")");
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return false;
    }

    const unsigned long now = millis();
    if (now - _last_health_check_ms < SD_HEALTH_CHECK_PERIOD_MS) {
        return true;
    }

    _last_health_check_ms = now;
    return healthCheck();
}

bool SdManager::ensureMounted() {
    if (isAvailable()) {
        return true;
    }

    const unsigned long now = millis();
    if (now - _last_reconnect_attempt_ms < _reconnect_cooldown_ms) {
        return false;
    }

    _last_reconnect_attempt_ms = now;
    LOG_INFO("[3/4] Attempting SD reconnect (cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms)...");

    if (mountWithRetries()) {
        LOG_INFO("SD card reconnected successfully.");
        _consecutive_reconnect_failures = 0;
        _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
        _last_health_check_ms = now;
        return ensureHistoryDirectory();
    }

    _consecutive_reconnect_failures++;
    _reconnect_cooldown_ms = std::min(
        SD_RECONNECT_COOLDOWN_MAX_MS,
        SD_RECONNECT_COOLDOWN_DEFAULT_MS * (1 + static_cast<unsigned long>(_consecutive_reconnect_failures)));

    LOG_WARNING("SD reconnect failed (failure #" + std::to_string(_consecutive_reconnect_failures) + "). Next attempt in " + std::to_string(_reconnect_cooldown_ms) + "ms.");
    return false;
}

bool SdManager::ensureHistoryDirectory() {
    if (!_available && !ensureMounted()) {
        return false;
    }

    if (SD.exists("/history")) {
        return true;
    }

    LOG_INFO("[4/4] Creating /history directory on SD card.");
    if (!SD.mkdir("/history")) {
        LOG_WARNING("Failed to create /history directory on SD card.");
        return false;
    }

    return true;
}

bool SdManager::format() {
    LOG_WARNING("Formatting SD Card... This will erase all data!");

    SD.end();
    SPI.end();
    _available = false;

    powerCycleSdModule();

    bool format_success = false;
    const size_t attempts = sizeof(SD_FORMAT_FREQUENCIES) / sizeof(SD_FORMAT_FREQUENCIES[0]);

    for (size_t i = 0; i < attempts; ++i) {
        const int frequency_hz = SD_FORMAT_FREQUENCIES[i];

        SPI.end();
        initializeSpiBus();

        LOG_INFO("SD format attempt #" + std::to_string(i + 1) + " at " + std::to_string(frequency_hz / 1000) + " kHz...");
        if (tryLowLevelFormat(frequency_hz)) {
            format_success = true;
            break;
        }

        delay(200);
    }

    SPI.end();

    if (!format_success) {
        LOG_ERROR("SD format failed after all retries.");
        return false;
    }

    LOG_INFO("Remounting card after format...");
    return begin();
}

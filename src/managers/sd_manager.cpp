#include "sd_manager.h"
#include "../utils/logs.h"
#include "board_config.h"

#include "ff.h" // For f_mkfs

// Valeurs par défaut si non définies dans board_config.h
#ifndef SD_CS_PIN
#define SD_CS_PIN 10
#endif
#ifndef SPI_SCK_PIN
#define SPI_SCK_PIN 12
#endif
#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN 13
#endif
#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN 11
#endif

// Déclarations des fonctions internes de la librairie SD (sd_diskio.cpp)
// Nécessaire pour initialiser la carte sans la monter (ce qui échoue si non formatée)
uint8_t sdcard_init(uint8_t cs, SPIClass * spi, int hz);
bool sdcard_uninit(uint8_t pdrv);

namespace {
constexpr int SD_INIT_RETRY_COUNT = 3;
constexpr int SD_FORMAT_RETRY_COUNT = 3;
constexpr int SD_INIT_FREQUENCIES[] = {8000000, 4000000, 1000000};
constexpr int SD_FORMAT_FREQUENCIES[] = {4000000, 1000000, 400000};

bool mountSdAtFrequency(int frequency_hz) {
    return SD.begin(SD_CS_PIN, SPI, frequency_hz);
}

bool lowLevelInit(uint8_t& pdrv, int frequency_hz) {
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, -1);
    pdrv = sdcard_init(SD_CS_PIN, &SPI, frequency_hz);
    return pdrv != 0xFF;
}

bool formatSdOnce(int frequency_hz) {
    uint8_t pdrv = 0xFF;
    if (!lowLevelInit(pdrv, frequency_hz)) {
        LOG_WARNING("SD format: low-level init failed at " + std::to_string(frequency_hz) + "Hz");
        SPI.end();
        return false;
    }

    LOG_INFO("SD format: low-level init OK (pdrv=" + std::to_string(pdrv) + ") at " + std::to_string(frequency_hz) + "Hz");

    char drv[4];
    snprintf(drv, sizeof(drv), "%d:", pdrv);

    uint8_t* work_buffer = static_cast<uint8_t*>(malloc(FF_MAX_SS));
    if (!work_buffer) {
        LOG_ERROR("SD format: failed to allocate work buffer");
        sdcard_uninit(pdrv);
        SPI.end();
        return false;
    }

    // FM_ANY est plus robuste selon la géométrie de la carte et la version FatFS.
    FRESULT res = f_mkfs(drv, FM_ANY, 0, work_buffer, FF_MAX_SS);

    free(work_buffer);
    sdcard_uninit(pdrv);
    SPI.end();

    if (res != FR_OK) {
        LOG_WARNING("SD format failed with f_mkfs error: " + std::to_string(res));
        return false;
    }

    return true;
}
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card...");

    if (_available) {
        SD.end();
        _available = false;
    }

    pinMode(SD_CS_PIN, OUTPUT);
    digitalWrite(SD_CS_PIN, HIGH);
    delay(10);

    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SD_CS_PIN);

    bool mounted = false;
    for (int i = 0; i < SD_INIT_RETRY_COUNT; i++) {
        int frequency_hz = SD_INIT_FREQUENCIES[i];
        if (mountSdAtFrequency(frequency_hz)) {
            LOG_INFO("SD mount OK at " + std::to_string(frequency_hz) + "Hz");
            mounted = true;
            break;
        }

        LOG_WARNING("SD mount failed at " + std::to_string(frequency_hz) + "Hz");
        SD.end();
        delay(60);
    }

    if (!mounted) {
        LOG_ERROR("SD mount failed on all retries. Check wiring/card integrity or run SD format.");
        _available = false;
        return false;
    }

    uint8_t card_type = SD.cardType();
    if (card_type == CARD_NONE) {
        LOG_WARNING("No SD card attached");
        _available = false;
        return false;
    }

    LOG_INFO("SD Card OK. Size: " + std::to_string(SD.cardSize() / (1024 * 1024)) + "MB");
    _available = true;
    return true;
}

bool SdManager::isAvailable() const {
    return _available;
}

bool SdManager::format() {
    LOG_WARNING("Formatting SD Card...");

    if (_available) {
        SD.end();
        _available = false;
    }

    bool format_success = false;
    for (int i = 0; i < SD_FORMAT_RETRY_COUNT; i++) {
        int frequency_hz = SD_FORMAT_FREQUENCIES[i];
        LOG_INFO("SD format attempt " + std::to_string(i + 1) + " at " + std::to_string(frequency_hz) + "Hz");

        if (formatSdOnce(frequency_hz)) {
            LOG_INFO("SD format successful at " + std::to_string(frequency_hz) + "Hz");
            format_success = true;
            break;
        }

        delay(120);
    }

    if (!format_success) {
        LOG_ERROR("SD format failed after all retries");
        return false;
    }

    delay(100);
    if (!begin()) {
        LOG_ERROR("SD formatted but remount failed");
        return false;
    }

    if (!SD.exists("/history") && !SD.mkdir("/history")) {
        LOG_WARNING("SD format: failed to create /history directory");
    }

    return true;
}

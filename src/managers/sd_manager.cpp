#include <algorithm>
#include <string>

#include <SPI.h>
#include "ff.h"

#include "../utils/logs.h"
#include "board_config.h"
#include "sd_manager.h"

// Le macro SD de sd_manager.h sert aux autres fichiers du projet.
// On le désactive ici pour pouvoir utiliser explicitement l'instance ::SD (SPI).
#ifdef SD
#undef SD
#endif

#include "../utils/logs.h"
#include "board_config.h"
#include "sd_manager.h"

// Valeurs par défaut si non définies dans board_config.h
#ifndef SD_CLK_PIN
#define SD_CLK_PIN 12
#endif
#ifndef SD_CMD_PIN
#define SD_CMD_PIN 11
#endif
#ifndef SD_DAT0_PIN
#define SD_DAT0_PIN 13
#endif
#ifndef SD_DAT1_PIN
#define SD_DAT1_PIN 14
#endif
#ifndef SD_DAT2_PIN
#define SD_DAT2_PIN 10
#endif
#ifndef SD_DAT3_PIN
#define SD_DAT3_PIN 9
#endif

fs::FS* g_sd_fs = &SD_MMC;

namespace {

constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000;

constexpr int SD_INIT_FREQS_SPI[] = {1000000, 4000000, 10000000, 20000000};
constexpr int SD_INIT_FREQS_SPI_COUNT = 4;

// SD_MMC.begin attend des kHz.
constexpr int SD_INIT_FREQS_4BIT[] = {
    SDMMC_FREQ_PROBING,
    1000,
    4000,
    SDMMC_FREQ_DEFAULT,
    SDMMC_FREQ_HIGHSPEED
};
constexpr int SD_INIT_FREQS_4BIT_COUNT = 5;

constexpr int SD_INIT_FREQS_1BIT[] = {SDMMC_FREQ_PROBING, 1000, 4000};
constexpr int SD_INIT_FREQS_1BIT_COUNT = 3;

constexpr int SD_FORMAT_FREQS[] = {SDMMC_FREQ_PROBING, 1000, 4000};
constexpr int SD_FORMAT_FREQS_COUNT = 3;

SPIClass sd_spi(FSPI);

fs::FS& activeFs() {
    return *g_sd_fs;
}

void configurePins4bit() {
    SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_DAT0_PIN, SD_DAT1_PIN, SD_DAT2_PIN, SD_DAT3_PIN);
}

void configurePins1bit() {
    SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_DAT0_PIN);
}

void logSpiPinMapping() {
    std::string msg =
        "SD SPI : CLK=" + std::to_string(SD_CLK_PIN) +
        " MISO(D0)=" + std::to_string(SD_DAT0_PIN) +
        " MOSI(CMD)=" + std::to_string(SD_CMD_PIN) +
        " CS(D3)=" + std::to_string(SD_DAT3_PIN);
    LOG_INFO(msg);
}

void logSdioPinMapping(bool mode_4bit) {
    std::string msg =
        std::string("SD SDIO [") + (mode_4bit ? "4-bit" : "1-bit") + "] : " +
        "CLK=" + std::to_string(SD_CLK_PIN) +
        " CMD=" + std::to_string(SD_CMD_PIN) +
        " DAT0=" + std::to_string(SD_DAT0_PIN);
    if (mode_4bit) {
        msg +=
            " DAT1=" + std::to_string(SD_DAT1_PIN) +
            " DAT2=" + std::to_string(SD_DAT2_PIN) +
            " DAT3=" + std::to_string(SD_DAT3_PIN);
    }
    LOG_INFO(msg);
}

void ensureHistoryDir() {
    if (!activeFs().exists("/history")) {
        activeFs().mkdir("/history");
    }
}

} // namespace

void SdManager::unmountActiveBackend() {
    if (_backend == Backend::spi) {
        ::SD.end();
    } else if (_backend == Backend::sdmmc) {
        SD_MMC.end();
    }
    _backend = Backend::none;
    g_sd_fs = &SD_MMC;
}

bool SdManager::cardIsPresent() const {
    if (_backend == Backend::spi) {
        return ::SD.cardType() != CARD_NONE;
    }
    if (_backend == Backend::sdmmc) {
        return SD_MMC.cardType() != CARD_NONE;
    }
    return false;
}

uint64_t SdManager::cardSizeBytes() const {
    if (_backend == Backend::spi) {
        return ::SD.cardSize();
    }
    if (_backend == Backend::sdmmc) {
        return SD_MMC.cardSize();
    }
    return 0;
}

uint64_t SdManager::totalBytes() const {
    if (_backend == Backend::spi) {
        return ::SD.totalBytes();
    }
    if (_backend == Backend::sdmmc) {
        return SD_MMC.totalBytes();
    }
    return 0;
}

uint64_t SdManager::usedBytes() const {
    if (_backend == Backend::spi) {
        return ::SD.usedBytes();
    }
    if (_backend == Backend::sdmmc) {
        return SD_MMC.usedBytes();
    }
    return 0;
}

bool SdManager::mountWithRetries() {
    delay(10);
    unmountActiveBackend();

    logSpiPinMapping();
    sd_spi.begin(SD_CLK_PIN, SD_DAT0_PIN, SD_CMD_PIN, SD_DAT3_PIN);
    for (int i = 0; i < SD_INIT_FREQS_SPI_COUNT; i++) {
        int freq_hz = SD_INIT_FREQS_SPI[i];
        if (::SD.begin(SD_DAT3_PIN, sd_spi, freq_hz, "/sdcard", 10, false)) {
            _backend = Backend::spi;
            g_sd_fs = &::SD;
            if (cardIsPresent()) {
                LOG_INFO("SD OK (SPI) @ " + std::to_string(freq_hz) + " Hz");
                ensureHistoryDir();
                _available = true;
                return true;
            }
            LOG_WARNING("SD SPI monté mais carte absente @ " + std::to_string(freq_hz) + " Hz");
            unmountActiveBackend();
        } else {
            LOG_WARNING("SD SPI échec @ " + std::to_string(freq_hz) + " Hz");
        }
        delay(120);
    }

    logSdioPinMapping(true);
    for (int i = 0; i < SD_INIT_FREQS_4BIT_COUNT; i++) {
        int freq_khz = SD_INIT_FREQS_4BIT[i];
        SD_MMC.end();
        configurePins4bit();
        if (SD_MMC.begin("/sdcard", false, false, freq_khz)) {
            _backend = Backend::sdmmc;
            g_sd_fs = &SD_MMC;
            if (cardIsPresent()) {
                LOG_INFO("SD OK (SDIO 4-bit) @ " + std::to_string(freq_khz * 1000) + " Hz");
                ensureHistoryDir();
                _available = true;
                return true;
            }
            LOG_WARNING("SD SDIO 4-bit monté mais carte absente @ " + std::to_string(freq_khz * 1000) + " Hz");
            unmountActiveBackend();
        } else {
            LOG_WARNING("SD SDIO 4-bit échec @ " + std::to_string(freq_khz * 1000) + " Hz");
        }
        delay(120);
    }

    logSdioPinMapping(false);
    LOG_INFO("SD : repli SDIO en mode 1-bit...");
    for (int i = 0; i < SD_INIT_FREQS_1BIT_COUNT; i++) {
        int freq_khz = SD_INIT_FREQS_1BIT[i];
        SD_MMC.end();
        configurePins1bit();
        if (SD_MMC.begin("/sdcard", true, false, freq_khz)) {
            _backend = Backend::sdmmc;
            g_sd_fs = &SD_MMC;
            if (cardIsPresent()) {
                LOG_INFO("SD OK (SDIO 1-bit repli) @ " + std::to_string(freq_khz * 1000) + " Hz");
                ensureHistoryDir();
                _available = true;
                return true;
            }
            LOG_WARNING("SD SDIO 1-bit monté mais carte absente @ " + std::to_string(freq_khz * 1000) + " Hz");
            unmountActiveBackend();
        } else {
            LOG_WARNING("SD SDIO 1-bit échec @ " + std::to_string(freq_khz * 1000) + " Hz");
        }
        delay(120);
    }

    _available = false;
    return false;
}

bool SdManager::begin() {
    LOG_INFO("Init SD Card (auto SPI/SDIO)...");
    unmountActiveBackend();
    _available = false;

    bool ok = mountWithRetries();
    if (!ok) {
        LOG_ERROR("SD : échec de montage sur toutes les tentatives. Vérifier câblage/carte.");
        return false;
    }

    LOG_INFO("SD Card OK. Taille : " + std::to_string(cardSizeBytes() / (1024ULL * 1024ULL)) + " MB");
    _last_reconnect_attempt_ms = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }
    if (!cardIsPresent()) {
        LOG_WARNING("SD : carte devenue indisponible");
        _available = false;
        unmountActiveBackend();
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
    LOG_INFO("SD : tentative de reconnexion (cooldown=" +
             std::to_string(_reconnect_cooldown_ms) + " ms)");

    bool mounted = mountWithRetries();
    if (mounted) {
        _consecutive_reconnect_failures = 0;
        _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
        return true;
    }

    _consecutive_reconnect_failures++;
    _reconnect_cooldown_ms = std::min(
        SD_RECONNECT_COOLDOWN_MAX_MS,
        SD_RECONNECT_COOLDOWN_DEFAULT_MS *
            static_cast<unsigned long>(_consecutive_reconnect_failures + 1));
    LOG_WARNING("SD : reconnexion échouée (échec #" +
                std::to_string(_consecutive_reconnect_failures) +
                "), prochain cooldown=" + std::to_string(_reconnect_cooldown_ms) + " ms");
    return false;
}

bool SdManager::format() {
    LOG_WARNING("Formatage SD Card...");

    if (_available) {
        unmountActiveBackend();
        _available = false;
    }

    // Le formatage FatFS bas niveau est disponible via SDIO (SD_MMC).
    bool mounted = false;
    for (int i = 0; i < SD_FORMAT_FREQS_COUNT && !mounted; i++) {
        int freq_khz = SD_FORMAT_FREQS[i];

        SD_MMC.end();
        configurePins4bit();
        LOG_INFO("SD format : montage SDIO 4-bit @ " + std::to_string(freq_khz * 1000) + " Hz");
        mounted = SD_MMC.begin("/sdcard", false, false, freq_khz);

        if (!mounted) {
            SD_MMC.end();
            configurePins1bit();
            LOG_INFO("SD format : repli SDIO 1-bit @ " + std::to_string(freq_khz * 1000) + " Hz");
            mounted = SD_MMC.begin("/sdcard", true, false, freq_khz);
        }

        if (!mounted) {
            delay(120);
        }
    }

    if (!mounted) {
        LOG_ERROR("SD format : impossible de monter la carte en SDIO");
        return false;
    }

    FRESULT res_umount = f_mount(NULL, "0:", 0);
    if (res_umount != FR_OK) {
        LOG_WARNING("SD format : f_mount(NULL) code=" + std::to_string(res_umount) + " (ignoré)");
    }

    uint8_t* work_buf = static_cast<uint8_t*>(malloc(FF_MAX_SS));
    bool format_ok = false;
    if (work_buf) {
        FRESULT res = f_mkfs("0:", FM_ANY, 0, work_buf, FF_MAX_SS);
        format_ok = (res == FR_OK);
        if (!format_ok) {
            LOG_WARNING("SD format : f_mkfs code=" + std::to_string(res));
        }
        free(work_buf);
    } else {
        LOG_ERROR("SD format : allocation work_buf échouée");
    }

    SD_MMC.end();
    _backend = Backend::none;
    g_sd_fs = &SD_MMC;
    _available = false;

    if (!format_ok) {
        LOG_ERROR("SD format échoué");
        return false;
    }

    LOG_INFO("SD format réussi, remontage en cours...");
    delay(100);

    if (!begin()) {
        LOG_ERROR("SD formatée mais remontage échoué");
        return false;
    }

    if (!activeFs().exists("/history") && !activeFs().mkdir("/history")) {
        LOG_WARNING("SD format : échec création dossier /history");
    }

    return true;
}

#include "sd_manager.h"
#include "../utils/logs.h"
#include "board_config.h"

#include <algorithm>
#include "esp_vfs_fat.h"

// Valeurs par défaut si non définies dans board_config.h
#ifndef SD_CLK_PIN
#define SD_CLK_PIN  12
#endif
#ifndef SD_CMD_PIN
#define SD_CMD_PIN  11
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
#define SD_DAT3_PIN  9
#endif

namespace {

constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS     = 120000;

// Fréquences d'initialisation (Hz), du plus conservateur au plus rapide.
// Les modules de qualité moyenne bénéficient d'un démarrage à basse fréquence.
constexpr int SD_INIT_FREQS_4BIT[] = {
    SDMMC_FREQ_PROBING,   // 400 kHz — toujours tenté en premier
    1000000,              // 1 MHz
    4000000,              // 4 MHz
    SDMMC_FREQ_DEFAULT,   // 20 MHz
    SDMMC_FREQ_HIGHSPEED  // 40 MHz
};
constexpr int SD_INIT_FREQS_4BIT_COUNT = 5;

constexpr int SD_INIT_FREQS_1BIT[] = {
    SDMMC_FREQ_PROBING,   // 400 kHz
    1000000,              // 1 MHz
    4000000               // 4 MHz
};
constexpr int SD_INIT_FREQS_1BIT_COUNT = 3;

constexpr int SD_FORMAT_FREQS[] = {
    SDMMC_FREQ_PROBING,   // 400 kHz
    1000000,              // 1 MHz
    4000000               // 4 MHz
};
constexpr int SD_FORMAT_FREQS_COUNT = 3;

void configurePins4bit() {
    SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_DAT0_PIN, SD_DAT1_PIN, SD_DAT2_PIN, SD_DAT3_PIN);
}

void configurePins1bit() {
    // En mode 1-bit seul DAT0 est utilisé (plus tolérant au bruit)
    SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_DAT0_PIN);
}

void logPinMapping(bool mode_4bit) {
    std::string msg =
        std::string("SD SDMMC [") + (mode_4bit ? "4-bit" : "1-bit") + "] : " +
        "CLK=" + std::to_string(SD_CLK_PIN) +
        " CMD=" + std::to_string(SD_CMD_PIN) +
        " DAT0=" + std::to_string(SD_DAT0_PIN);
    if (mode_4bit) {
        msg += " DAT1=" + std::to_string(SD_DAT1_PIN) +
               " DAT2=" + std::to_string(SD_DAT2_PIN) +
               " DAT3/CD=" + std::to_string(SD_DAT3_PIN);
    }
    LOG_INFO(msg);
}

bool tryMount(bool mode_4bit, int frequency_hz) {
    SD_MMC.end();
    if (mode_4bit) {
        configurePins4bit();
    } else {
        configurePins1bit();
    }
    // format_if_failed=false : on ne formate jamais automatiquement lors d'une reconnexion
    return SD_MMC.begin("/sdcard", !mode_4bit, false, frequency_hz);
}

bool cardIsPresent() {
    return SD_MMC.cardType() != CARD_NONE;
}

void ensureHistoryDir() {
    if (!SD_MMC.exists("/history")) {
        SD_MMC.mkdir("/history");
    }
}

} // namespace

// ---------------------------------------------------------------------------
// mountWithRetries — tente le montage en 4-bit puis repli en 1-bit.
// Robuste pour les modules de qualité moyenne (plusieurs fréquences testées).
// ---------------------------------------------------------------------------
bool SdManager::mountWithRetries() {
    delay(10);

    // --- Mode 4-bit (DAT0..DAT3) ---
    logPinMapping(true);
    for (int i = 0; i < SD_INIT_FREQS_4BIT_COUNT; i++) {
        int freq = SD_INIT_FREQS_4BIT[i];
        if (tryMount(true, freq)) {
            if (cardIsPresent()) {
                LOG_INFO("SD OK (4-bit) @ " + std::to_string(freq) + " Hz");
                ensureHistoryDir();
                _available = true;
                return true;
            }
            LOG_WARNING("SD 4-bit monté mais carte absente @ " + std::to_string(freq) + " Hz");
            SD_MMC.end();
        } else {
            LOG_WARNING("SD 4-bit échec @ " + std::to_string(freq) + " Hz");
        }
        delay(120);
    }

    // --- Repli mode 1-bit (plus tolérant aux problèmes de signal) ---
    logPinMapping(false);
    LOG_INFO("SD : repli en mode 1-bit...");
    for (int i = 0; i < SD_INIT_FREQS_1BIT_COUNT; i++) {
        int freq = SD_INIT_FREQS_1BIT[i];
        if (tryMount(false, freq)) {
            if (cardIsPresent()) {
                LOG_INFO("SD OK (1-bit repli) @ " + std::to_string(freq) + " Hz");
                ensureHistoryDir();
                _available = true;
                return true;
            }
            LOG_WARNING("SD 1-bit monté mais carte absente @ " + std::to_string(freq) + " Hz");
            SD_MMC.end();
        } else {
            LOG_WARNING("SD 1-bit échec @ " + std::to_string(freq) + " Hz");
        }
        delay(120);
    }

    _available = false;
    return false;
}

// ---------------------------------------------------------------------------
// begin — initialisation principale (appelée au démarrage)
// ---------------------------------------------------------------------------
bool SdManager::begin() {
    LOG_INFO("Init SD Card (SDMMC natif)...");
    SD_MMC.end();
    _available = false;

    bool ok = mountWithRetries();
    if (!ok) {
        LOG_ERROR("SD : échec de montage sur toutes les tentatives. Vérifier câblage/carte.");
        return false;
    }

    LOG_INFO("SD Card OK. Taille : " +
             std::to_string(SD_MMC.cardSize() / (1024ULL * 1024ULL)) + " MB");
    _last_reconnect_attempt_ms  = millis();
    _consecutive_reconnect_failures = 0;
    _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
    return true;
}

// ---------------------------------------------------------------------------
// isAvailable — vérifie la disponibilité, tente une reconnexion si nécessaire
// ---------------------------------------------------------------------------
bool SdManager::isAvailable() {
    if (!_available) {
        return ensureMounted();
    }
    if (SD_MMC.cardType() == CARD_NONE) {
        LOG_WARNING("SD : carte devenue indisponible");
        _available = false;
        SD_MMC.end();
        return ensureMounted();
    }
    return true;
}

// ---------------------------------------------------------------------------
// ensureMounted — reconnexion avec refroidissement exponentiel
// ---------------------------------------------------------------------------
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
            static_cast<unsigned long>(_consecutive_reconnect_failures + 1)
    );
    LOG_WARNING("SD : reconnexion échouée (échec #" +
                std::to_string(_consecutive_reconnect_failures) +
                "), prochain cooldown=" + std::to_string(_reconnect_cooldown_ms) + " ms");
    return false;
}

// ---------------------------------------------------------------------------
// format — formatage explicite (déclenché par l'utilisateur via l'UI)
// Utilise esp_vfs_fat_sdcard_format (ESP-IDF 5.x / Arduino ESP32 3.x)
// ---------------------------------------------------------------------------
bool SdManager::format() {
    LOG_WARNING("Formatage SD Card (SDMMC)...");

    if (_available) {
        SD_MMC.end();
        _available = false;
    }

    // Tenter le montage à basse fréquence pour obtenir le handle interne
    bool mounted = false;
    for (int i = 0; i < SD_FORMAT_FREQS_COUNT && !mounted; i++) {
        int freq = SD_FORMAT_FREQS[i];
        LOG_INFO("SD format : montage @ " + std::to_string(freq) + " Hz (4-bit)");
        mounted = tryMount(true, freq);
        if (!mounted) {
            LOG_INFO("SD format : tentative 1-bit @ " + std::to_string(freq) + " Hz");
            mounted = tryMount(false, freq);
        }
        if (!mounted) {
            delay(120);
        }
    }

    if (!mounted) {
        LOG_ERROR("SD format : impossible de monter la carte pour le formatage");
        return false;
    }

    sdmmc_card_t* card = SD_MMC.get_card();
    if (!card) {
        LOG_ERROR("SD format : handle de carte indisponible");
        SD_MMC.end();
        return false;
    }

    esp_err_t err = esp_vfs_fat_sdcard_format("/sdcard", card);
    SD_MMC.end();
    _available = false;

    if (err != ESP_OK) {
        LOG_ERROR("SD format échoué (esp_err=" +
                  std::to_string(static_cast<int>(err)) + ")");
        return false;
    }

    LOG_INFO("SD format réussi, remontage en cours...");
    delay(100);

    if (!begin()) {
        LOG_ERROR("SD formatée mais remontage échoué");
        return false;
    }

    if (!SD_MMC.exists("/history") && !SD_MMC.mkdir("/history")) {
        LOG_WARNING("SD format : échec création dossier /history");
    }

    return true;
}

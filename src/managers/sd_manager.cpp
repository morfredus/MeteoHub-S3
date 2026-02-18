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

bool SdManager::begin() {
    LOG_INFO("Init SD Card...");

    if (_available) {
        SD.end();
        _available = false;
    }

    // Initialisation du bus SPI avec les broches définies
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SD_CS_PIN);

    // Tentative de montage
    if (!SD.begin(SD_CS_PIN)) {
        LOG_WARNING("SD Mount Failed (Default), retrying at 4MHz...");
        // Tentative à vitesse réduite pour plus de stabilité
        if (!SD.begin(SD_CS_PIN, SPI, 4000000)) {
            LOG_ERROR("SD Mount Failed. Check wiring or Format SD to FAT32.");
            _available = false;
            return false;
        }
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        LOG_WARNING("No SD Card attached");
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
    
    // Unmount if it was mounted
    if (_available) {
        SD.end();
        _available = false;
    }

    // Attente pour la stabilisation matérielle
    delay(100);

    // Réinitialisation explicite du bus SPI, sans CS car géré par sdcard_init
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, -1);

    // Initialisation bas niveau. Enregistre le disque physique.
    uint8_t pdrv = sdcard_init(SD_CS_PIN, &SPI, 4000000);
    
    if (pdrv == 0xFF) {
        LOG_ERROR("Low-level SD init failed. Check wiring.");
        SPI.end();
        return false;
    }

    LOG_INFO("Card hardware OK (pdrv=" + std::to_string(pdrv) + "). Formatting...");
    
    char drv[4]; 
    snprintf(drv, sizeof(drv), "%d:", pdrv); // "0:" généralement
    
    // Allocation du tampon de travail sur le tas pour éviter tout débordement de pile
    uint8_t* work_buffer = (uint8_t*)malloc(FF_MAX_SS);
    if (!work_buffer) {
        LOG_ERROR("Failed to allocate work buffer for format!");
        sdcard_uninit(pdrv);
        SPI.end();
        return false;
    }

    FRESULT res = f_mkfs(drv, FM_FAT32, 0, work_buffer, FF_MAX_SS);
    
    free(work_buffer);
    
    // Désenregistrement du disque physique et libération du bus SPI
    sdcard_uninit(pdrv);
    SPI.end();

    if (res != FR_OK) {
        LOG_ERROR("SD Format Failed. f_mkfs error: " + std::to_string(res));
        return false;
    }

    LOG_INFO("SD Format Successful. Remounting...");
    delay(100);
    return begin();
}
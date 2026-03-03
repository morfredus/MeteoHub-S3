#pragma once
 
// ------------------------------------
// I2C (AHT20 + BMP280 / OLED SH1106)
// ------------------------------------
#define I2C_SDA_PIN 15
#define I2C_SCL_PIN 16

// ------------------------------------
// Boutons
// ------------------------------------
#define BUTTON_BOOT_PIN 0
#define BUTTON_BACK_PIN 7
#define BUTTON_CONFIRM_PIN 8

// ------------------------------------
// Neopixel soudée
// ------------------------------------
#define NEOPIXEL_PIN 48

// ------------------------------------
// ENCODEUR — GPIO communs
// (EC11 et HW-040 utilisent les mêmes pins)
// ------------------------------------
#define ENCODER_A_PIN      4
#define ENCODER_B_PIN      5
#define ENCODER_BTN_PIN    6

// ------------------------------------
// Module microSD Sniffer (SD_MMC 1-bit)
// Broches module: DAT1 DAT0 CLK VCC GND CMD CD DAT2
// ------------------------------------
#define SDMMC_CLK_PIN   12   // module CLK
#define SDMMC_CMD_PIN   11   // module CMD
#define SDMMC_D0_PIN    13   // module DAT0
#define SDMMC_CD_PIN    10   // module CD (card detect optionnel)
#define SDMMC_D1_PIN    -1   // module DAT1 (non utilisé en 1-bit)
#define SDMMC_D2_PIN    -1   // module DAT2 (non utilisé en 1-bit)
#define SDMMC_D3_PIN    -1   // DAT3/CS non câblé sur ce module


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

    #define ENCODER_MODEL_EC11

#endif

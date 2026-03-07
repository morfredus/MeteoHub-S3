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
// Module SD 3.3V (brochage compatible SPI + SDIO)
// Labels module : CLK, D0/SO, CMD/S1, D3/CS, D1, DAT2, DET
// ------------------------------------
#define SD_CLK_PIN   9    // SCK
#define SD_MISO_PIN  10   // DAT0 / SO
#define SD_MOSI_PIN  11   // CMD / SI
#define SD_CS_PIN    12   // DAT3 / CS
#define SD_DAT2_PIN  13   // DAT2
// Pin de détection (optionnel, mettez -1 si non câblé)
// Si câblé : LOW = Carte présente, HIGH = Vide
#define SD_DET_PIN   14
// Mode de bus SD :
// 0 = AUTO (SPI puis SDIO), 1 = SPI uniquement, 2 = SDIO uniquement
#define SD_BUS_MODE  1


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

    #define ENCODER_MODEL_EC11

#endif

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
// Module SD (SPI secondaire SAFE)
// ------------------------------------
#define SD_CS_PIN    10
#define SD_SCK_PIN   12
#define SD_MOSI_PIN  11
#define SD_MISO_PIN  13


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

    #define ENCODER_MODEL_EC11

#elif defined(ESP32_S3_LCD)

    #define ENCODER_MODEL_HW040

    // ------------------------------------
    // TFT ST7789 — Mapping FIXE ESP32-S3
    // ------------------------------------
    #define DISPLAY_CS_PIN      13
    #define DISPLAY_MOSI_PIN    11
    #define DISPLAY_SCK_PIN     12
    #define DISPLAY_DC_PIN      10
    #define DISPLAY_RST_PIN      9
    #define DISPLAY_BL_PIN      46
    #define DISPLAY_MISO_PIN    -1

#endif

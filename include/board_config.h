#pragma once


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

// I2C OLED SH1106
#define I2C_SDA_PIN 15
#define I2C_SCL_PIN 16

// Encodeur EC11
#define ENCODER_A_PIN      4   // tra
#define ENCODER_B_PIN      5   // trb
#define ENCODER_BTN_PIN    6   // psh (push encodeur)

// Boutons dédiés
#define BUTTON_BACK_PIN    7  // bak
#define BUTTON_CONFIRM_PIN 8  // com

    #define NEOPIXEL_PIN 48   // npx (neopixel status)

// Masse et alim sont câblées physiqement :
// grd -> GND
// vcc -> 3V3

#elif defined(ESP32_S3_LCD)

// ------------------------------------
// TFT ST7789 — Mapping FIXE ESP32-S3
// ------------------------------------
#define DISPLAY_CS_PIN      13   // Chip Select TFT
#define DISPLAY_MOSI_PIN    11   // SDA / DIN
#define DISPLAY_SCK_PIN     12   // SCLK
#define DISPLAY_DC_PIN      10   // Data/Command
#define DISPLAY_RST_PIN      9   // Reset matériel
#define DISPLAY_BL_PIN       8   // Rétroéclairage (PWM OK)
#define DISPLAY_MISO_PIN    -1   // Non utilisé (ST7789 = write-only)

// ------------------------------------
// Boutons
// ------------------------------------
#define BUTTON_BOOT_PIN 0
#define BUTTON_BACK_PIN   7
#define BUTTON_CONFIRM_PIN   8

#define NEOPIXEL_PIN 48   // npx (neopixel status)

// ------------------------------------
// ENCODEUR ROTATIF (HW-040)
// ------------------------------------
#define ENCODER_A_PIN   4
#define ENCODER_B_PIN    5
#define ENCODER_BTN_PIN    6

// ------------------------------------
// I2C (AHT20 + BMP280)
// ------------------------------------
#define I2C_SDA_PIN 15
#define I2C_SCL_PIN 16


#endif

#pragma once


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

// I2C OLED SH1106
#define OLED_SDA 15
#define OLED_SCL 16

// Encodeur EC11
#define ENC_A      4   // tra
#define ENC_B      5   // trb
#define ENC_BTN    6   // psh (push encodeur)

// Boutons dédiés
#define BTN_BACK    7  // bak
#define BTN_CONFIRM 8  // com

#define NEOPIXEL_PIN 48   // npx (neopixel status)

// Masse et alim sont câblées physiquement :
// grd -> GND
// vcc -> 3V3

// Alias génériques pour code commun
#define BUTTON_BACK_PIN BTN_BACK
#define BUTTON_CONFIRM_PIN BTN_CONFIRM
#define ENCODER_A_PIN ENC_A
#define ENCODER_B_PIN ENC_B
#define ENCODER_BTN_PIN ENC_BTN

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
#define BUTTON_BOOT 0
#define BUTTON_1   38
#define BUTTON_2   39

// ------------------------------------
// ENCODEUR ROTATIF (HW-040)
// ------------------------------------
#define ROTARY_CLK   47
#define ROTARY_DT    45
#define ROTARY_SW    40

// ------------------------------------
// I2C (AHT20 + BMP280)
// ------------------------------------
#define I2C_SDA 15
#define I2C_SCL 16

// Alias génériques pour code commun
#define BUTTON_BACK_PIN BUTTON_1
#define BUTTON_CONFIRM_PIN BUTTON_2
#define ENCODER_A_PIN ROTARY_CLK
#define ENCODER_B_PIN ROTARY_DT
#define ENCODER_BTN_PIN ROTARY_SW

#endif

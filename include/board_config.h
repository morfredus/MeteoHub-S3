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
#define SD_CLK_PIN   12   // CLK / SCK
#define SD_CMD_PIN   11   // CMD / MOSI (DI)
#define SD_DAT0_PIN  13   // DAT0 / MISO (DO)
#define SD_DAT1_PIN  14   // DAT1
#define SD_DAT2_PIN  10   // DAT2
#define SD_DAT3_PIN   9   // DAT3 / CS

// Mode de bus SD :
// 0 = AUTO (SPI puis SDIO), 1 = SPI uniquement, 2 = SDIO uniquement
#define SD_BUS_MODE 1


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

    #define ENCODER_MODEL_EC11

#endif

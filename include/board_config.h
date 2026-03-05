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
// Nouveau module SD 3.3V (SPI ou SDIO)
// Broches module: 3V, GND, CLK, D0/SO, CMD/SI, D3/CS, D1, DAT2, DET4
// ------------------------------------
// Mapping SPI (mode principal)
#define SD_SPI_SCK_PIN   12   // CLK
#define SD_SPI_MISO_PIN  13   // D0/SO
#define SD_SPI_MOSI_PIN  11   // CMD/SI
#define SD_SPI_CS_PIN    10   // D3/CS

// Mapping SDIO 1-bit (repli)
#define SDMMC_CLK_PIN    12   // CLK
#define SDMMC_CMD_PIN    11   // CMD/SI
#define SDMMC_D0_PIN     13   // D0/SO
#define SDMMC_D1_PIN     -1   // D1 non utilisé
#define SDMMC_D2_PIN     -1   // DAT2 non utilisé
#define SDMMC_D3_PIN     -1   // D3 non utilisé en 1-bit

// Détection carte (DET4) optionnelle
#define SD_DET_PIN       -1


// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

    #define ENCODER_MODEL_EC11

#endif

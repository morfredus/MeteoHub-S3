#pragma once
 
// -------------------------------------------------------------------
// Configuration conditionnelle selon l'environnement PlatformIO
// -------------------------------------------------------------------
#if defined(ESP32_S3_OLED)

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
    #define SD_CS_PIN    45
    #define SD_MOSI_PIN  41
    #define SD_SCK_PIN   40
    #define SD_MISO_PIN  42

    #define ENCODER_MODEL_EC11

#elif defined(ESP32_DEV_MODULE_OLED)

    // ------------------------------------
    // I2C (AHT20 + BMP280 / OLED SH1106)
    // Pins sûres et standards sur ESP32 Dev Module
    // ------------------------------------
    #define I2C_SDA_PIN 21
    #define I2C_SCL_PIN 22

    // ------------------------------------
    // Boutons
    // ------------------------------------
    #define BUTTON_BOOT_PIN 0
    #define BUTTON_BACK_PIN 25
    #define BUTTON_CONFIRM_PIN 26

    // ------------------------------------
    // Neopixel externe
    // ------------------------------------
    #define NEOPIXEL_PIN 27

    // ------------------------------------
    // ENCODEUR — GPIO sûrs sur ESP32 Dev Module
    // ------------------------------------
    #define ENCODER_A_PIN      32
    #define ENCODER_B_PIN      33
    #define ENCODER_BTN_PIN    14

    // ------------------------------------
    // Module SD (VSPI classique et compatible)
    // ------------------------------------
    #define SD_CS_PIN    13
    #define SD_MOSI_PIN  23
    #define SD_SCK_PIN   18
    #define SD_MISO_PIN  19

    #define ENCODER_MODEL_EC11

#else

    #error "Unsupported board environment. Define ESP32_S3_OLED or ESP32_DEV_MODULE_OLED."

#endif

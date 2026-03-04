# Hardware Wiring

Minimum valid version: 1.0.173

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings

Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional SD card (native SDMMC 4-bit, 3.3V)

## SD Card Module (Native SDMMC 4-bit, 3.3V)

The SD card uses the ESP32-S3 native SDMMC interface (not SPI).
This provides better performance and reliability than SPI mode.

| Module pin | GPIO | Description             |
|------------|------|-------------------------|
| CLK        | 12   | Clock                   |
| CMD        | 11   | Command                 |
| DAT0       | 13   | Data line 0             |
| DAT1       | 14   | Data line 1             |
| DAT2       | 10   | Data line 2             |
| CD / DAT3  | 9    | Card detect / Data line 3 |
| VCC        | 3.3V | Power supply            |
| GND        | GND  | Ground                  |

> **Note:** The module's CD pin corresponds to DAT3 in the SDMMC 4-bit protocol.
> Connect it to GPIO 9 as shown above.

### Compatibility and robustness

The driver attempts mounting in the following order:
1. 4-bit mode at 400 kHz (most conservative)
2. 4-bit mode at 1 MHz
3. 4-bit mode at 4 MHz
4. 4-bit mode at 20 MHz
5. 4-bit mode at 40 MHz
6. 1-bit fallback at 400 kHz (for noisy or lower-quality wiring)
7. 1-bit fallback at 1 MHz
8. 1-bit fallback at 4 MHz

This strategy ensures compatibility with average-quality SD modules and short-trace breakout boards.

## Other peripherals

| Peripheral   | Pin(s)                        |
|-------------|-------------------------------|
| I2C SDA     | GPIO 15                       |
| I2C SCL     | GPIO 16                       |
| NeoPixel    | GPIO 48                       |
| Encoder A   | GPIO 4                        |
| Encoder B   | GPIO 5                        |
| Encoder btn | GPIO 6                        |
| Button BACK | GPIO 7                        |
| Button OK   | GPIO 8                        |

# Hardware Wiring

Minimum valid version: 1.0.182

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings

Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional SD module 3.3V (SPI primary, SDIO 1-bit fallback)

## SD module 3.3V wiring (ESP32-S3)
- VCC -> 3V3
- GND -> GND
- CLK -> GPIO12
- D0/SO -> GPIO13
- CMD/SI -> GPIO11
- D3/CS -> GPIO10
- D1 / DAT2: not used
- DET4: optional (not used in code by default)

Runtime strategy:
- Primary mode: SPI
- Fallback mode: SD_MMC 1-bit
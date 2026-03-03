# Hardware Wiring

Minimum valid version: 1.0.180

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings

Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional microSD Sniffer module (SD_MMC 1-bit)

## microSD Sniffer wiring (ESP32-S3)
- VCC -> 3V3
- GND -> GND
- CLK -> GPIO12
- CMD -> GPIO11
- DAT0 -> GPIO13
- CD -> GPIO10 (optional card detect)
- DAT1/DAT2: not used in 1-bit mode

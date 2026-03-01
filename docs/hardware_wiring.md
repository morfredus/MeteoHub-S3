# Hardware Wiring

Minimum valid version: 1.0.177

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings

Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional SD card (SPI)

PlatformIO environments:
1. `esp32-s3-oled` for ESP32-S3 DevKitC-1
2. `esp32-dev-oled` for ESP32 Dev Module


## SD Card Reliability (ESP32-S3 3.3V modules)
1. Keep SPI wires short and common ground stable.
2. Use a 3.3V-compatible SD module (no 5V-only level shifter).
3. Firmware now performs adaptive SPI mount retries (8MHz to 400kHz).
4. Runtime health checks and automatic remount protect long logging sessions.
5. History writes are retried safely after remount to reduce data loss.

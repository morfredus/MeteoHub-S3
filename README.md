# MeteoHub S3

> **Minimum valid version: 1.0.166**

## Full Documentation
- [Documentation Index](docs/index.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project focused on an OLED dashboard (SH1106/SSD1306 via U8g2). It displays local sensor values, weather forecast data, logs, and system status.

## Required Hardware
- OLED display (SH1106 or SSD1306, I2C)
- HW-040 rotary encoder module (encoder + center button)
- Back and Confirm buttons
- AHT20 and BMP280 sensors
- Optional SD card for long-term history

## Build
- Install PlatformIO in VS Code
- Select environment: `esp32-s3-oled`
- Build: `platformio run`
- Upload: `platformio run --target upload`

## Key Features
- Real-time local sensor display
- Weather forecast pages
- Local log pages
- Persistent history (LittleFS + optional SD archiving)
- OTA update from web interface
- UTF-8 rendering on OLED

For setup, usage, and troubleshooting, see `docs/`.

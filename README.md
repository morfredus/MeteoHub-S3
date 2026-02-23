# MeteoHub S3

Minimum valid version: 1.0.157

## Full Documentation
- [Documentation Index](docs/index.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project that displays local sensor data, weather forecast, logs, and system status on an OLED dashboard (SH1106/SSD1306 through U8g2).

## Required Hardware
- OLED screen (SH1106 or SSD1306, I2C)
- HW-040 rotary module (encoder + click)
- Back and Confirm buttons
- AHT20 + BMP280 sensors
- Optional SD card for long-term history

## Build
- Install PlatformIO in VS Code.
- Select environment `esp32-s3-oled`.
- Build: `platformio run`
- Upload: `platformio run --target upload`

## Key Features
- Weather pages, forecast pages, history graphs, network/system pages and logs on OLED.
- OTA update support (ArduinoOTA + web OTA page).
- SD formatting and history management from device menu.
- Web dashboard with live data, graphs, logs, and file manager.

## Notes
- OLED rendering is handled by U8g2 and supports SH1106/SSD1306 backends.
- For full setup, usage and troubleshooting, see the `docs/` folder.

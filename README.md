# MeteoHub S3

Minimum valid version: 1.0.26

## Full Documentation
- [Documentation Index](docs/index.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project that displays local sensor data, weather forecast information, logs, and system status on an SH1106 OLED dashboard.

## Project Architecture
- `src/modules/`: hardware and page modules (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `pages`)
- `src/managers/`: orchestration and state managers (`wifi_manager`, `ui_manager`, `history`, `forecast_manager`)
- `src/utils/`: reusable utility code (`logs`, `system_info`)
- `include/`: reserved configuration headers only (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Build
- Install PlatformIO in VS Code.
- Select environment `esp32-s3-devkitc-1-n16r8v`.
- Build command: `platformio run`
- Upload command: `platformio run --target upload`

## Runtime Features
- Sensor acquisition (AHT20 + BMP280)
- Wi-Fi management with retry logic
- NTP synchronization
- Forecast and alerts retrieval (OpenWeatherMap)
- OLED page-based UI with encoder and buttons
- History persistence in LittleFS
- Status LED with NeoPixel

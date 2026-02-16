
# MeteoHub S3

Minimum valid version: 1.0.30

## Full Documentation
- [Documentation Index](docs/index.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project that displays local sensor data, weather forecast information, logs, and system status on a color LCD (TFT ST7789) or SH1106 OLED dashboard. The firmware auto-detects the display type and adapts the UI and navigation accordingly.

## Supported Environments
- **OLED (SH1106, I2C)**: Monochrome, 128x64, rotary encoder + 2 buttons, NeoPixel, AHT20/BMP280 sensors.
- **LCD (TFT ST7789, SPI)**: Color, 240x240, rotary encoder (HW-040) + 3 buttons, AHT20/BMP280 sensors.

## Project Architecture
- `src/modules/`: hardware/display/page modules (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `st7789_display`, `pages`)
- `src/managers/`: orchestration and state managers (`wifi_manager`, `ui_manager`, `history`, `forecast_manager`)
- `src/utils/`: reusable utility code (`logs`, `system_info`)
- `include/`: reserved configuration headers only (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Build
- Install PlatformIO in VS Code.
- Select environment `esp32-s3-lcd` (TFT) or `esp32-s3-oled` (OLED) as needed.
- Build command: `platformio run`
- Upload command: `platformio run --target upload`

## Runtime Features
- Sensor acquisition (AHT20 + BMP280)
- Wi-Fi management with retry logic
- NTP synchronization
- Forecast and alerts retrieval (OpenWeatherMap)
- Page-based UI with rotary encoder and buttons (OLED or LCD)
- History persistence in LittleFS
- Status LED with NeoPixel (OLED) or on-screen alert (LCD)

## LCD vs OLED: Key Differences
- **Navigation**: On LCD, one detent = one page; on OLED, 2 detents = one page.
- **Display**: LCD is color, higher resolution, and supports more advanced layouts (axes, graphs, etc.).
- **Wiring**: See [Hardware Wiring](docs/hardware_wiring.md) for exact pinout per environment.

For detailed setup, usage, and troubleshooting, see the user documentation in the `docs/` folder.

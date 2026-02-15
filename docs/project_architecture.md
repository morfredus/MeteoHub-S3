# Project Architecture

Minimum valid version: 1.0.22

## Goal
Explain how source code is organized and how data flows across the system.

## Folder structure
- `src/main.cpp`: boot and top-level orchestration
- `src/modules/`: hardware/display/page modules
- `src/managers/`: stateful managers and orchestration
- `src/utils/`: reusable utilities
- `include/`: reserved configuration headers

## Module responsibilities
### `src/modules/`
- `encoder`: rotary input decoding
- `neopixel_status`: LED color/status output
- `sensors`: AHT20 + BMP280 acquisition
- `sh1106_display`: OLED rendering abstraction
- `pages`: page rendering logic

### `src/managers/`
- `wifi_manager`: Wi-Fi retries and connection state
- `forecast_manager`: OpenWeatherMap fetch + parse
- `history`: ring buffer + LittleFS persistence
- `ui_manager`: input handling + page/menu state

### `src/utils/`
- `logs`: circular in-memory log buffer + log macros
- `system_info`: runtime memory/system metrics

## Runtime data flow
1. `main.cpp` initializes display, sensors, Wi-Fi, time.
2. `ui_manager.update()` drives periodic runtime loop.
3. Managers refresh data (Wi-Fi, sensors, forecast, history).
4. Pages render current state to OLED.
5. NeoPixel reflects connection/alert state.

## Persistence model
- NVS (Preferences): last active page index
- LittleFS: historical sensor records

## External dependencies
- OLED driver (SH1106)
- Adafruit sensor libraries
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences

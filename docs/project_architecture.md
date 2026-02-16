
# Project Architecture

Minimum valid version: 1.0.30

## Goal
Explain how source code is organized, how OLED and LCD environments are managed, and how data flows across the system.

## Folder structure
- `src/main.cpp`: boot and top-level orchestration, auto-detects display type (OLED/LCD)
- `src/modules/`: hardware/display/page modules (see below)
- `src/managers/`: stateful managers and orchestration
- `src/utils/`: reusable utilities
- `include/`: reserved configuration headers

## Display environment management
- The firmware auto-detects at boot whether an SH1106 OLED or a TFT ST7789 LCD is connected.
- All display/page logic is abstracted via `DisplayInterface`.
- Dedicated modules exist for each display: `sh1106_display` (OLED), `st7789_display` (LCD).
- Page rendering logic is split: `pages_sh1106.cpp` (OLED), `pages_st7789.cpp` (LCD).
- Navigation logic adapts: on LCD, 1 detent = 1 page; on OLED, 2 detents = 1 page.

## Module responsibilities
### `src/modules/`
- `encoder`: rotary input decoding (hardware abstraction)
- `neopixel_status`: LED color/status output (OLED only)
- `sensors`: AHT20 + BMP280 acquisition
- `sh1106_display`: OLED rendering abstraction
- `st7789_display`: LCD rendering abstraction
- `pages_sh1106`: page rendering logic for OLED
- `pages_st7789`: page rendering logic for LCD

### `src/managers/`
- `wifi_manager`: Wi-Fi retries and connection state
- `forecast_manager`: OpenWeatherMap fetch + parse
- `history`: ring buffer + LittleFS persistence
- `ui_manager`: input handling, page/menu state, navigation logic (OLED/LCD)

### `src/utils/`
- `logs`: circular in-memory log buffer + log macros
- `system_info`: runtime memory/system metrics

## Runtime data flow
1. `main.cpp` initializes display (auto-detect), sensors, Wi-Fi, time.
2. `ui_manager.update()` drives periodic runtime loop and adapts navigation to display type.
3. Managers refresh data (Wi-Fi, sensors, forecast, history).
4. Pages render current state to the detected display (OLED or LCD).
5. NeoPixel (OLED) or on-screen alert (LCD) reflects connection/alert state.

## Persistence model
- NVS (Preferences): last active page index
- LittleFS: historical sensor records

## External dependencies
- OLED driver (SH1106) and/or LCD driver (Adafruit ST7789)
- Adafruit sensor libraries
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences

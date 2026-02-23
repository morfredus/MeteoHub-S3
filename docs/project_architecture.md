## Weather Alert Description
The API /api/alert now returns the full alert description (in French when available). The web dashboard displays this text for maximum clarity and localization.
# Project Architecture

Minimum valid version: 1.0.154

## New Features (since 1.0.127)
- **Weather Alert Card**: The dashboard always displays a weather alert card (via `/api/alert`).
- **Live Sensor Data**: The `/api/live` endpoint returns real sensor values (temperature, humidity, pressure).
- **Dashboard vs History**: The dashboard graph shows the last 2 hours; the history page shows 24 hours.
- **Strict Project Compliance**: All API routes are declared only inside `_setupApi()`. No forbidden comments or placeholders exist in the codebase.
- **OLED Improvement (>=1.0.139)**: No more flicker during OLED page transitions thanks to the removal of the intermediate `show()` and optimized buffer management. Smoother and more stable rendering on SH1106.


## Goal
Explain the source code organization, the management of OLED and LCD environments, and the data flow within the system.

## Folder Structure
- `src/main.cpp`: Boot and main orchestration for the selected display environment (OLED/LCD).
- `src/modules/`: Hardware/display/page modules (see below).
- `src/managers/`: State and orchestration managers.
- `src/utils/`: Reusable utilities.
- `include/`: Reserved configuration headers only.

## Display Environment Management
- Display type is chosen at build time (`esp32-s3-oled` or `esp32-s3-lcd`).
- All display/page logic is abstracted through `DisplayInterface`.
- Dedicated modules for each display: `sh1106_display` (OLED), `st7789_display` (LCD).
- Separate page rendering logic: `pages_oled.cpp` (OLED), `pages_st7789.cpp` (LCD).
- Adapted navigation: On LCD, 1 detent = 1 page; on OLED, 2 detents = 1 page.

## Module Responsibilities
### `src/modules/`
- `encoder`: Decodes encoder inputs (hardware abstraction).
- `neopixel_status`: Color/status LED output (OLED only).
- `sensors`: AHT20 + BMP280 data acquisition.
- `sh1106_display`: OLED rendering abstraction.
- `st7789_display`: LCD rendering abstraction.
- `pages_oled`: Renders pages for OLED.
- `pages_st7789`: Renders pages for LCD.

### `src/managers/`
- `wifi_manager`: Wi-Fi retries and connection state.
- `forecast_manager`: OpenWeatherMap retrieval + parsing.
- `history`: Circular buffer + LittleFS persistence.
- `ui_manager`: Input management, page/menu state, navigation logic (OLED/LCD).

### `src/utils/`
- `logs`: In-memory circular log buffer + logging macros.
- `system_info`: Runtime memory/system metrics.

## Runtime Data Flow
1. `main.cpp` initializes the selected display backend, sensors, Wi-Fi, and time.
2. `ui_manager.update()` drives the periodic loop and adapts navigation based on the display.
3. Managers refresh data (Wi-Fi, sensors, forecasts, history).
4. Pages display the current state on the selected display environment (OLED or LCD).
5. NeoPixel (OLED) or on-screen alert (LCD) reflects the connection/alert status.

## Persistence Model
- **NVS (Preferences)**: Non-volatile storage for small data (e.g., index of the last active page).
- **LittleFS**: Filesystem on the internal flash memory for larger data.
  - **Recent History**: The last 24 hours are appended to a file (`/history/recent.dat`) to minimize flash wear.
  - **Role**: Serves as a fast cache for rebooting and as backup storage for logs.
- **SD Card**: Primary storage for long-term archiving.
  - **Format**: Daily CSV files (`/history/YYYY-MM-DD.csv`).
  - **Advantage**: High capacity, easy to read on a PC, increased robustness compared to internal flash for frequent writes.

## OLED UTF-8 Rendering

From version 1.0.154, the OLED backend (`OledDisplay`) supports UTF-8 rendering for all text, including accented characters and special symbols. The display module is now named `OledDisplay` to reflect its neutrality and compatibility with both SH1106 and SSD1306 controllers.

---
**[v1.0.119+] SD Card Robustness**

From version 1.0.119, SD card formatting and mounting are much more robust:
- Multiple retry attempts at decreasing SPI speeds (4MHz, 1MHz, 400kHz) during formatting.
- Low-level reinitialization between each attempt.
- Automatic remount and validation after formatting.
- Startup SD mounting uses multi-frequency retries (8MHz, 4MHz, 1MHz) to tolerate unstable cards and wiring.
This greatly improves reliability with problematic SD cards and reduces the risk of mount failures.

## External Dependencies
- OLED (SH1106) and/or LCD (Adafruit ST7789) driver
- Adafruit sensor libraries
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences
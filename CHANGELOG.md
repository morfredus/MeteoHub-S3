# Project Changelog

Minimum valid version: 1.0.21

## Version 1.0.21
- **Security Documentation**: Clarified that `include/secrets.h` must be created from `include/secrets_example.h`, must never be pushed to GitHub, and must never be shared.

## Version 1.0.20
- **Documentation**: Added a complete beginner-oriented documentation set in `docs/`.
- **Bilingual Consistency**: Added strict English/French equivalents for all new user-facing docs.
- **Project Guidance**: Documented setup, wiring, configuration, usage, architecture, and troubleshooting workflows.

## Version 1.0.19
- **Memory Discipline**: Replaced core runtime text handling with `std::string` in managers, pages, and logging.
- **Display Interface**: Updated `Sh1106Display` API to accept `std::string` and centralized conversion at display driver boundary.
- **Code Consistency**: Harmonized include ordering across updated files to match project include rules.

## Version 1.0.18
- **Architecture**: Reorganized source files into `src/modules`, `src/managers`, and `src/utils`.
- **Build Consistency**: Updated all local include paths to match the new folder structure.
- **Project Hygiene**: Removed duplicate `forecast_manager` files at project root and restricted `include/` to reserved files only.
- **Logging**: Standardized runtime logs through `LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, and `LOG_ERROR` macros.

## Version 1.0.17
- **Bug Fixes**: Resolved String capitalization compilation errors and fixed button pin conflicts.

## Version 1.0.16
- **UX Improvements**: Reordered pages for logical flow (Weather -> Forecast -> Graphs) and harmonized page headers.

## Version 1.0.15
- **Interactivity**: Added click-to-cycle functionality for the Forecast page views.

## Version 1.0.14
- **Localization**: Added French translations for specific weather alert events.

## Version 1.0.13
- **Project Identity**: Renamed project to "MeteoHub S3" and implemented dynamic version injection via PlatformIO build flags.

## Version 1.0.12
- **System Menu**: Added a pop-up menu for maintenance actions (Reboot, Clear Logs, Clear History).

## Version 1.0.11
- **Status LED**: Implemented NeoPixel logic for visual feedback (Green: WiFi OK, Red: WiFi/Temp Alert, Orange: Weather Alert).

## Version 1.0.10
- **Weather Forecast**: Integrated OpenWeatherMap API (`ForecastManager`) to fetch Today/Tomorrow forecasts and alerts.

## Version 1.0.9
- **Graphing Engine**: Added dynamic line graphs for Temperature, Humidity, and Pressure with auto-scaling axes.

## Version 1.0.8
- **Persistence**: Added LittleFS storage for history data and Preferences (NVS) to remember the last active page across reboots.

## Version 1.0.7
- **Data History**: Implemented `HistoryManager` using a circular buffer to store 128 data points.

## Version 1.0.6
- **UI System**: Developed a page-based user interface (`UiManager`) navigable via rotary encoder.

## Version 1.0.5
- **Boot Sequence**: Created an animated splash screen with a progress bar displaying initialization steps (Sensors, WiFi, Time).

## Version 1.0.4
- **Connectivity**: Added `WifiManager` with non-blocking connection logic and NTP time synchronization.

## Version 1.0.3
- **Sensor Driver**: Implemented `SensorManager` for AHT20 and BMP280 sensors.

## Version 1.0.2
- **Hardware Configuration**: Mapped specific GPIOs for ESP32-S3 (OLED, Encoder, Buttons, NeoPixel).

## Version 1.0.1
- **Rotary Encoder Optimization**: Adjusted resolution to 4 pulses per detent and improved debouncing logic for reliable navigation.
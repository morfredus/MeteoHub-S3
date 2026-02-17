
# Project Changelog

Minimum valid version: 1.0.60

## Version 1.0.75
- **Documentation**: Global documentation update (README, Guides, FAQ) to include LCD 240x320 support and new boot screens. Bilingual synchronization.

## Version 1.0.74
- **Fix (OLED)**: Added missing include `sh1106_display.h` in `pages_sh1106.cpp` to resolve compilation error related to `Sh1106Display`.

## Version 1.0.73
- **UI (Boot)**: Aesthetic adjustment of the LCD Splash Screen: "morfredus" is now lowercase and perfectly vertically centered between decorative lines.

## Version 1.0.72
- **Fix (Compilation)**: Added missing declarations for `drawSplashScreen_st7789` and `drawBootProgress_st7789` in the header file, resolving compilation error on LCD environment.

## Version 1.0.71
- **Fix (Main)**: Effective integration of calls to new boot screens (Splash Screen and Boot Progress) in the main initialization sequence, replacing the old static logic.

## Version 1.0.70
- **Fix (Dev)**: Added missing declarations in header files for Splash Screen and Boot Progress functions.

## Version 1.0.69
- **UI (Boot)**: Simplified boot logo ("morfredus" only).
- **UI (OLED)**: Implementation of boot and progress screens for OLED environment (SH1106).

## Version 1.0.68
- **UI (Boot)**: Added animated "morfredus" splash screen and visual progress bar for initialization steps (Sensors, WiFi, NTP, etc.), adaptive to LCD resolution.

## Version 1.0.67
- **Documentation**: Added missing changelog entries for versions 1.0.61 to 1.0.64 (LCD 240x320 Support).

## Version 1.0.66
- **Fix (UI)**: Vertical adjustment (+2px) of temperature display on home screen in 240x240 mode to avoid overlap with title.

## Version 1.0.65
- **Fix (UI)**: Adjustment of vertical position of graphs on LCD. Top of graph shifted down to prevent Y-axis labels from overlapping header separator line.

## Version 1.0.64
- **Feature (UI)**: Reactive adaptation of all pages (Network, System, Logs, Weather, Graphs, Forecast) to support extended vertical resolution (320px) while preserving 240px display.

## Version 1.0.63
- **Driver (LCD)**: `St7789Display` driver now uses dynamic dimensions defined in global configuration.

## Version 1.0.62
- **Configuration**: Cleanup of version definitions in `platformio.ini`.

## Version 1.0.61
- **Configuration**: Introduction of `LCD_WIDTH` and `LCD_HEIGHT` constants in `config.h` to configure screen resolution.

## Version 1.0.60
- **Fix (LCD)**: Corrected graph scaling on ST7789 displays. The plot now dynamically stretches to use the full screen width, ensuring the latest measurement is always at the far right.
- **Refactor (UI)**: Simplified rotary encoder logic. The LCD now uses the natural hardware direction (Clockwise = Next), while the OLED inversion is handled in a single conditional block, improving code readability.

## Version 1.0.59
- **Fix**: Resolved a critical bug where UI pages would not load or display in the LCD environment (ST7789). The page drawing logic is now correctly implemented for both OLED and LCD.

## Version 1.0.58
- **Robust History Management**: Complete refactoring of the data history system for long-term reliability.
  - **Flash-Friendly Storage**: Recent history is now appended to a file, not rewritten, drastically reducing flash memory wear.
  - **Instant Data on Boot**: The system now reloads the last 24 hours of high-resolution data and 30 days of daily summaries into RAM at startup.
  - **UI Consistency**: All UIs (OLED, LCD, Web) have immediate access to the full history, eliminating "empty graphs" after a reboot.
  - **Data Integrity**: Added a check to prevent saving data points before NTP time is synchronized.
  - **Automatic Maintenance**: Old archives (> 2 years) are now automatically deleted.

## Version 1.0.29
- **LCD UI Overhaul**: Complete redesign of the ST7789 interface with a dark theme, colored headers, and data cards.
- **Flicker-Free**: Implemented double buffering (Canvas16) for LCD to eliminate scanning artifacts.
- **Navigation Logic**: Inverted rotary encoder direction on LCD (Pages & Menu) to match natural intuition (CW = Next).
- **Menu Redesign**: LCD system menu now matches the global design language.
- **Page Order**: Swapped "System" and "Logs" pages on LCD for better flow.
- **Bug Fixes**: Fixed `clearLogs` linker error, frozen display issues, and text accent handling.

## Version 1.0.28
- **Forecast Display Fix**: Forecast page now shows real values (temperature, min, max) on LCD.
- **Graph Layout**: Axes and value labels are shifted to avoid overlap on LCD.
- **Rotary Encoder LCD**: On LCD, one detent = one page (OLED logic unchanged).
- **Encoder API**: Public method for step count, no more direct access to private rotary.
- **Docs & Build**: Documentation and code fully synchronized, build and upload validated.

## Version 1.0.27
- **I2C Initialization**: Dynamic I2C initialization in SensorManager for both OLED and LCD environments, ensuring AHT20/BMP280 detection on all boards.
- **LCD Sensor Fix**: Sensors now work out-of-the-box on LCD builds (default I2C pins, can be customized in board_config.h).
- **LCD Display Rotation**: Default LCD orientation is now rotated (setRotation(2)) for correct display direction.

## Version 1.0.26
- **Multi-écrans** : Ajout d'un environnement LCD (TFT ST7789) avec gestion conditionnelle du code et du brochage.
- **Protection compilation** : Ajout de conditions dans tous les fichiers d'affichage pour éviter les conflits entre OLED et LCD.
- **Documentation** : Mise à jour de tous les documents utilisateur pour refléter la nouvelle architecture.

## Version 1.0.25
- **Graph Scale Fix**: Min/max values for Temperature, Humidity, and Pressure graphs are now computed from the displayed history window.
- **Pressure Readability**: Graph side labels now use adaptive precision to prevent visually identical min/max values caused by rounding.

## Version 1.0.24
- **User Documentation**: Added practical guidance to read Temperature, Humidity, and especially Pressure values.
- **Weather Interpretation**: Documented how to interpret pressure trends and combine Temp/Humidity/Pressure to infer short-term weather evolution.

## Version 1.0.23
- **Alert Prioritization**: Weather alert selection now evaluates all received alerts and keeps the most critical one.
- **Flood Priority**: Flood/rising-water events are explicitly prioritized over generic rain events at the same severity level.
- **Display Translation**: Alert label translation now supports French keywords such as `crue`, `inondation`, and `pluie`.

## Version 1.0.22
- **Alert Logic**: Improved weather alert visualization with distinct Yellow/Orange/Red NeoPixel colors based on event description keywords.

## Version 1.0.21
- **Security Documentation**: Clarified that `include/secrets.h` must be created from `include/secrets_example.h`, must never be pushed to GitHub, and must never be shared.

## Version 1.0.20
- **Documentation**: Added a complete beginner-oriented documentation set in `docs/`.
- **Bilingual Consistency**: Added strict English/French equivalents for all new user-facing docs.
- **Project Guidance**: Documented setup, wiring, configuration, usage, architecture, and troubleshooting workflows.


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
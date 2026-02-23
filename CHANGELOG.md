
# Project Changelog

Minimum valid version: 1.0.155

## Version 1.0.155
- **Refactor (OLED Display)**: Renamed all `sh1106_display.*` files and references to `oled_display.*` for clarity and multi-controller support. All code and documentation now use the neutral OLED naming.
- **Feature (OLED UTF-8 Support)**: Improved accented/UTF-8 character rendering on OLED by replacing all `drawStr`/`getStrWidth` calls with `drawUTF8`/`getUTF8Width` in the OLED display class. This ensures correct display of French and other accented characters on all supported OLEDs.
- **Documentation**: User guides (EN/FR) updated to explain the new OLED naming and UTF-8 display support. Minimum valid version updated to 1.0.155.

## Version 1.0.154
- **Documentation (Full Refresh)**: Updated all user-facing EN/FR documents to the current OTA/OLED/SD baseline and synchronized minimum valid version markers.
- **Documentation (OTA UX)**: Added explicit OTA web workflow details (`/ota.html`, progress bar, inline validation area, automatic return to dashboard).
- **Release Notes**: Clarified that OTA web update uses ESP32 built-in `Update`/ArduinoOTA capabilities and introduces no new external library dependency.

## Version 1.0.153
- **Fix (OTA Web Compile)**: Declared missing OTA state members (`_ota_upload_error`, `_ota_restart_at_ms`) in `WebManager` to resolve build errors in OTA handlers.
- **Stability (OTA Runtime)**: Reset OTA state variables at WebManager startup for deterministic OTA upload behavior.

## Version 1.0.152
- **Feature (Web OTA Page)**: Added a dedicated OTA web page with firmware upload, live progress bar, and fixed-height inline status panel.
- **Feature (OTA API)**: Added `/api/ota/update` upload endpoint with firmware validation feedback and automatic reboot scheduling on success.
- **UX (OTA Flow)**: After successful OTA upload, the UI now auto-redirects to the main dashboard.

## Version 1.0.151
- **Feature (OTA)**: Added ArduinoOTA support with hostname binding (`WEB_MDNS_HOSTNAME`) and runtime handling in the main loop.
- **Diagnostics (OTA)**: Added OTA lifecycle logs (start/end/progress/error) plus late-start behavior when WiFi becomes available after boot.

## Version 1.0.150
- **Feature (OLED Weather Page)**: Added current weather text (from OpenWeatherMap, localized by configured `lang`) on the first OLED weather page, displayed under atmospheric pressure.
- **UI (OLED Readability)**: Added safe fallback and truncation for the current weather line to keep layout stable on 128x64 screens.

## Version 1.0.149
- **Refactor (OLED Naming)**: Renamed the OLED page module to `pages_oled.*` and updated OLED page/splash function names to neutral `_oled` suffixes.
- **Refactor (Display Class)**: Renamed display class to `OledDisplay` (with compatibility alias) to reflect U8g2 backend support for both SH1106 and SSD1306.
- **Cleanup (OLED Layout)**: Centralized key OLED geometry constants in the page renderer to reduce magic numbers and simplify maintenance.

## Version 1.0.148
- **Fix (SD Write Recovery)**: Improved SD history write path with explicit failure diagnostics (`mkdir` failure, `open` failure) and strict partial-write detection.
- **Resilience (SD Runtime)**: On `FILE_APPEND` open failure, the firmware now performs an immediate SD remount attempt and retries the write once.

## Version 1.0.147
- **Cleanup (OLED Migration)**: Archived obsolete OLED library evaluation guidance and aligned project docs with the current U8g2-based display baseline.
- **Documentation (Roadmap)**: Added dedicated bilingual TODO roadmap files for code and user-experience improvements (`docs/todo.md`, `docs/todo_fr.md`).

## Version 1.0.146
- **Fix (SD SPI Mapping)**: `SdManager` now uses SD-specific pin macros (`SD_SCK_PIN`, `SD_MISO_PIN`, `SD_MOSI_PIN`) from `board_config.h` for mount and low-level format init, avoiding silent SPI pin mismatches.
- **Diagnostics (SD Runtime)**: Added explicit SD pin mapping log (CS/SCK/MISO/MOSI) before mount retries to speed up wiring/runtime troubleshooting.

## Version 1.0.153
- **Fix (OTA Web Compile)**: Declared missing OTA state members (`_ota_upload_error`, `_ota_restart_at_ms`) in `WebManager` to resolve build errors in OTA handlers.
- **Stability (OTA Runtime)**: Reset OTA state variables at WebManager startup for deterministic OTA upload behavior.

## Version 1.0.152
- **Feature (Web OTA Page)**: Added a dedicated OTA web page with firmware upload, live progress bar, and fixed-height inline status panel.
- **Feature (OTA API)**: Added `/api/ota/update` upload endpoint with firmware validation feedback and automatic reboot scheduling on success.
- **UX (OTA Flow)**: After successful OTA upload, the UI now auto-redirects to the main dashboard.

## Version 1.0.151
- **Feature (OTA)**: Added ArduinoOTA support with hostname binding (`WEB_MDNS_HOSTNAME`) and runtime handling in the main loop.
- **Diagnostics (OTA)**: Added OTA lifecycle logs (start/end/progress/error) plus late-start behavior when WiFi becomes available after boot.

## Version 1.0.150
- **Feature (OLED Weather Page)**: Added current weather text (from OpenWeatherMap, localized by configured `lang`) on the first OLED weather page, displayed under atmospheric pressure.
- **UI (OLED Readability)**: Added safe fallback and truncation for the current weather line to keep layout stable on 128x64 screens.

## Version 1.0.149
- **Refactor (OLED Naming)**: Renamed the OLED page module to `pages_oled.*` and updated OLED page/splash function names to neutral `_oled` suffixes.
- **Refactor (Display Class)**: Renamed display class to `OledDisplay` (with compatibility alias) to reflect U8g2 backend support for both SH1106 and SSD1306.
- **Cleanup (OLED Layout)**: Centralized key OLED geometry constants in the page renderer to reduce magic numbers and simplify maintenance.

## Version 1.0.148
- **Fix (SD Write Recovery)**: Improved SD history write path with explicit failure diagnostics (`mkdir` failure, `open` failure) and strict partial-write detection.
- **Resilience (SD Runtime)**: On `FILE_APPEND` open failure, the firmware now performs an immediate SD remount attempt and retries the write once.

## Version 1.0.147
- **Cleanup (OLED Migration)**: Archived obsolete OLED library evaluation guidance and aligned project docs with the current U8g2-based display baseline.
- **Documentation (Roadmap)**: Added dedicated bilingual TODO roadmap files for code and user-experience improvements (`docs/todo.md`, `docs/todo_fr.md`).


## Version 1.0.146
- **Fix (SD SPI Mapping)**: `SdManager` now uses SD-specific pin macros (`SD_SCK_PIN`, `SD_MISO_PIN`, `SD_MOSI_PIN`) from `board_config.h` for mount and low-level format init, avoiding silent SPI pin mismatches.
- **Diagnostics (SD Runtime)**: Added explicit SD pin mapping log (CS/SCK/MISO/MOSI) before mount retries to speed up wiring/runtime troubleshooting.

## Version 1.0.150
- **Feature (OLED Weather Page)**: Added current weather text (from OpenWeatherMap, localized by configured `lang`) on the first OLED weather page, displayed under atmospheric pressure.
- **UI (OLED Readability)**: Added safe fallback and truncation for the current weather line to keep layout stable on 128x64 screens.

## Version 1.0.149
- **Refactor (OLED Naming)**: Renamed the OLED page module to `pages_oled.*` and updated OLED page/splash function names to neutral `_oled` suffixes.
- **Refactor (Display Class)**: Renamed display class to `OledDisplay` (with compatibility alias) to reflect U8g2 backend support for both SH1106 and SSD1306.
- **Cleanup (OLED Layout)**: Centralized key OLED geometry constants in the page renderer to reduce magic numbers and simplify maintenance.

## Version 1.0.148
- **Fix (SD Write Recovery)**: Improved SD history write path with explicit failure diagnostics (`mkdir` failure, `open` failure) and strict partial-write detection.
- **Resilience (SD Runtime)**: On `FILE_APPEND` open failure, the firmware now performs an immediate SD remount attempt and retries the write once.

## Version 1.0.147
- **Cleanup (OLED Migration)**: Archived obsolete OLED library evaluation guidance and aligned project docs with the current U8g2-based display baseline.
- **Documentation (Roadmap)**: Added dedicated bilingual TODO roadmap files for code and user-experience improvements (`docs/todo.md`, `docs/todo_fr.md`).


## Version 1.0.146
- **Fix (SD SPI Mapping)**: `SdManager` now uses SD-specific pin macros (`SD_SCK_PIN`, `SD_MISO_PIN`, `SD_MOSI_PIN`) from `board_config.h` for mount and low-level format init, avoiding silent SPI pin mismatches.
- **Diagnostics (SD Runtime)**: Added explicit SD pin mapping log (CS/SCK/MISO/MOSI) before mount retries to speed up wiring/runtime troubleshooting.

## Version 1.0.145
- **Documentation**: Updated all user documents (EN/FR) to version `1.0.145`, synchronized display-selection wording (build-time environment + OLED controller/address config), and aligned bilingual README links/sections.
- **Improvement (Manual OLED Config)**: Added explicit `OLED_I2C_ADDRESS` setting in `config.h` to support SH1106/SSD1306 modules using `0x3C` or `0x3D` without code edits.
- **OLED Init Logging**: Driver init logs now include configured I2C address for easier wiring/compatibility diagnostics.

## Version 1.0.144
- **Change (OLED Strategy)**: Removed OLED AUTO detection/hot-plug switching logic and switched to explicit manual controller selection in `config.h` (`OLED_CONTROLLER`).
- **Simplification (OLED Driver)**: OLED wrapper now initializes only the configured controller (SH1106 or SSD1306), reducing runtime ambiguity.

## Version 1.0.143
- **Fix (OLED Auto Switch Runtime)**: On OLED reconnect, AUTO mode now retries initialization with the opposite driver first, then fallback, to support real SH1106/SSD1306 hot-swaps.
- **Stability (Hot-Plug Recovery)**: Added dedicated reconnect reinit strategy instead of always reusing the previous driver.

## Version 1.0.142
- **Fix (SH1106 Hot-Plug Noise)**: Hardened SH1106 reinitialization/clear sequence and added runtime hot-plug recovery to avoid noisy/garbled pixels after unplug/replug.
- **Stability (OLED Runtime)**: Added I2C presence checks in display flush path and automatic driver reinit when an OLED module reconnects.

## Version 1.0.141
- **Fix (OLED Auto Switch)**: AUTO mode now prefers SSD1306 initialization first, with SH1106 fallback, to improve hot-swap compatibility when replacing SH1106 by SSD1306.
- **Fix (SSD1306 Ghosting)**: Hardened SSD1306 init/clear sequence (`resetDisplay` + extra clear/display) to reduce residual ghost pixels after module switch.

## Version 1.0.140
- **Feature (OLED Compatibility)**: Added dual OLED driver support (SH1106/SSD1306) in OLED environment with automatic I2C address detection and configurable driver mode.
- **Improvement (Display Abstraction)**: OLED display wrapper now uses a generic `OLEDDisplay` backend and runtime initialization path to improve compatibility across different OLED modules.

## Version 1.0.139
- **Fix (OLED Rendering Regression)**: Removed forced intermediate `show()` during UI context transitions to avoid visible blank-frame flicker and degraded OLED rendering.
- **Stability (OLED Cleanup)**: Kept context-change buffer clear while deferring frame flush to normal page render path.

## Version 1.0.138
- **Fix (OLED Page Artifacts)**: Added a forced clear+flush when UI screen context changes (page/menu/confirmation) to prevent leftover pixels from previous screens.
- **UI Stability**: Added render-state tracking in `UiManager` to detect context transitions and trigger deterministic full-screen cleanup.

## Version 1.0.137
- **Fix (SD Robustness)**: Hardened SD availability checks with automatic remount attempts and cooldown-based reconnect logic when the card becomes unavailable.
- **Fix (SD Mounting)**: Unified mount retries in `SdManager` and ensured `/history` directory creation on successful remount to reduce "SD absent/error" failures.

## Version 1.0.136
- **UI (Alert Card Size)**: Reduced alert card height by 25% to compact dashboard layout while keeping details access.
- **UI (Overflow Handling)**: Added vertical scroll behavior for long alert text/modal content and ensured the parchment details icon remains visible at bottom-right.

## Version 1.0.135
- **Fix (Stats Trend UI)**: Stats page now renders trend rows from `/api/stats` so the trend section no longer stays on "Chargement..."
- **Feature (Global Weather Trend)**: Added `trend.global_label_fr` in `/api/stats` with a synthetic global tendency label (e.g., "Vers beau temps", "Vers pluie").

## Version 1.0.134
- **Fix (Short Alert Language)**: Added a fallback for unmatched English alert event names so web short alert titles remain in French.
- **UI (Alert Details Trigger)**: Replaced the large "Voir détails complets" button with a compact parchment icon anchored at the bottom-right of the alert card.

## Version 1.0.133
- **Fix (Alert Detail Language)**: Alert detail text returned by web APIs is now a deterministic French summary, avoiding any raw English provider wording.
- **Clarification (API Behavior)**: `/api/alert` and `/api/live` continue exposing French alert fields while using provider `lang` parameter for forecast requests.

## Version 1.0.132
- **Optimization (Alert Refresh)**: Web UI alert polling now runs every 15 minutes (`ALERT_REFRESH_MS`) instead of every 5 seconds to reduce unnecessary requests.
- **Behavior Update (Dashboard)**: Alert card is no longer refreshed from `/api/live`; alert content now updates only through dedicated `/api/alert` refresh scheduling.

## Version 1.0.131
- **Fix (Alert Details Language)**: Web alert details now prioritize French-only description fields and use a French fallback summary when raw provider text cannot be translated reliably.
- **Fix (Dashboard Stability)**: Alert card now uses a fixed height with constrained text blocks to prevent page layout jumps during refreshes.

## Version 1.0.130
- **Fix (Web Alert Language)**: Added a French-translated alert description field (`description_fr`) in `/api/alert` and `alert_description_fr` in `/api/live`.
- **Fix (Web Alert Details Modal)**: Dashboard now prioritizes French alert description fields for both card details and modal content.

## Version 1.0.129
- **Feature (Web Alert Context)**: Added alert validity window fields (`start_unix`, `end_unix`) and `alert_level_label_fr` in web APIs to provide direct French severity labels and timing context.
- **Feature (Web Alert UX)**: Dashboard alert card now shows validity period and includes a "Voir détails complets" button opening a modal with long description and safety guidance.
- **Feature (Live Data Trust)**: Added a visible "Capteur invalide" badge on dashboard when `sensor_valid=false` to prevent misleading interpretation of live cards.

## Version 1.0.128
- **Fix (Web Alerts)**: Added dedicated `/api/alert` endpoint and ensured web alert title uses French translation (`event_fr`) to match OLED language.
- **Improvement (Web Alert UX)**: Dashboard alert card now changes background/text colors by severity with accessible contrast, and shows richer alert details (source + description).
- **Fix (Live Sensor Values)**: `/api/live` now returns real values from `SensorManager` (`temp`, `hum`, `pres`) instead of placeholders, so dashboard cards display actual sensor readings.
- **Integration**: `WebManager` now receives `SensorManager` at startup to provide live sensor data to web APIs.

## Version 1.0.127
- **Fix (Web Alert Language)**: Web API `/api/live` now exposes weather alert fields from `ForecastManager`, including a French-translated alert label (`alert_event_fr`) for UI consistency with OLED.
- **Feature (Web Dashboard)**: Added a weather alert card on the main web page and wired live refresh in `data/app.js` to display active alerts in French with severity/sender.
- **Fix (Integration)**: `WebManager` now receives `ForecastManager` reference at startup so alert data is available to Web UI endpoints.

## Version 1.0.126
- **Fix (Documentation Consistency)**: Removed unresolved merge markers and fully synchronized troubleshooting/FAQ docs (`docs/*.md` and `docs/*_fr.md`).
- **Optimization (History Load)**: Optimized oversized history trimming in `HistoryManager::loadRecent()` using single-range erase instead of repeated front erases.

## Version 1.0.125
- **Fix (History CSV Integrity)**: Fixed undefined behavior when writing SD CSV history timestamps by using a 64-bit-safe format (`%lld`) and explicit buffer-length validation before write. This prevents corrupted binary fragments in CSV files.
- **Improvement (History SD Write)**: Switched to explicit `File.write()` with exact byte count for each formatted line to improve write robustness.

## Version 1.0.124
- **Fix (Web Footer)**: Removed hardcoded project name/version from `data/footer.js`; footer now reads `project_name` and `project_version` from `/api/system` (sourced from PlatformIO build flags).
- **Fix (Web UI)**: Updated `data/app.js` to read `project_version` from `/api/system` with backward-compatible fallback.

## Version 1.0.123
- **Fix (Build)**: Corrected escaped JSON literals in `/api/history` (`web_manager.cpp`) after interval aggregation update, restoring C++ compilation.

## Version 1.0.122
- **Fix (Web Graphs)**: `/api/history` now supports explicit `interval` bucketing and server-side averaging to reduce payload size and CPU usage while keeping smooth chart lines.
- **Feature (Dashboard Graph)**: The main dashboard now requests exactly the last 2 hours with a 5-minute step (`window=7200`, `interval=300`).
- **Feature (24H History Graph)**: The long-term page now requests exactly the last 24 hours with a 30-minute step (`window=86400`, `interval=1800`) to reduce memory and rendering load.
- **UI Rendering**: Chart datasets explicitly use non-stepped monotone curves to keep smooth point-to-point interpolation.

## Version 1.0.121
- **Fix (Build)**: Corrected JSON string escaping in `web_manager.cpp` for `/api/history` response generation, fixing C++ compilation errors in `esp32-s3-oled` and restoring firmware build.

## Version 1.0.120
- **Fix (Web Performance)**: Refactored `data/app.js` to load only page-relevant data (history only on Dashboard/Long-term, stats only on Stats page), removing unnecessary heavy API calls and reducing UI freezes.
- **Fix (History API)**: Optimized `/api/history` with `window` and `points` parameters to limit server-side time range and returned points, significantly reducing response time and CPU load.
- **Improvement (Web UX)**: Lowered refresh rate of heavy endpoints (`history`, `stats`) to smooth UI behavior and avoid load spikes.

## Version 1.0.119
- **Fix (SD)**: Hardened SD formatting with multi-retry attempts at decreasing SPI speeds (4MHz, 1MHz, 400kHz), low-level reinit between attempts, and automatic remount/validation after formatting.
- **Improvement (SD)**: Hardened SD mount at startup with multi-frequency retries (8MHz, 4MHz, 1MHz) to better tolerate unstable cards and sensitive wiring setups.

## Version 1.0.118
- **Fix (Web UI)**: Added the `/menu.js` server route in `web_manager.cpp` to serve the new shared navigation script embedded in firmware. The shared menu now appears correctly after firmware build and browser hard refresh.

## Version 1.0.117
- 1) **Web UI (Shared menu)**: Added a common navigation menu injected by `data/menu.js` and used across all web pages for consistent, low-maintenance navigation.
- 2) **Web UI (Main page)**: The dashboard chart now displays only the last 2 hours, with continuous automatic refresh and smoother rendering (monotone curves without sharp angles).
- 3) **Web UI (Long-term history)**: The long-term history page now displays only the last 24 hours.
- 4) **Web UI (Startup & reload)**: On UI load, historical data is immediately reloaded from the API, then refreshed periodically so restored startup history is displayed using the new time windows (2h / 24h).

## Version 1.0.116
- **Fix (Web Server)**: Implemented server-side downsampling for the history API endpoint (`/api/history`). This drastically reduces the amount of data sent for graphs, definitively fixing watchdog reboots with large datasets and improving scalability.
- **Fix (Documentation)**: Corrected corrupted text and added missing information in the French FAQ (`faq_fr.md`).

## Version 1.0.115
- **Documentation**: Comprehensive update of all user-facing documents (User Guide, FAQ, Troubleshooting, Architecture) to reflect features and fixes up to v1.114, ensuring full bilingual consistency.

## Version 1.0.114
- **Fix (Web Server)**: Added a small delay in the `/api/history` data streaming loop to yield control to the OS, definitively preventing "Watchdog Timeout" reboots when loading very large history datasets.

## Version 1.0.113
- **Fix (Compilation)**: Fixed syntax error in `web_manager.cpp` (lambda capture) that prevented compilation of the `/api/system` endpoint.

## Version 1.0.112
- **Fix (Build)**: Moved `system_info` utility files to the correct `src/utils/` directory to resolve compilation errors. Cleaned up misplaced files in `docs/`.

## Version 1.0.111
- **Refactor**: Created a centralized `system_info` utility.
- **Feature (Web API)**: The `/api/system` endpoint now provides comprehensive system information (CPU, memory, filesystems, network).

## Version 1.0.110
- **Fix (Web Server)**: Optimized the `/api/history` endpoint to send data in chunks, preventing a "Watchdog Timeout" reboot when loading history graphs with a large number of data points.

## Version 1.0.109
- **Fix (Web Server)**: Fixed a critical bug causing a "Watchdog Timeout" reboot when downloading large files (history, logs) from the web UI. The file download handler now uses a non-blocking streaming method instead of loading the entire file into memory.

## Version 1.0.108
- **Documentation**: Improved the troubleshooting guide for "Watchdog Timeout" crashes, clarifying that the definitive solution is to use file streaming in the web server code to avoid blocking operations.

## Version 1.0.107
- **Documentation**: Synchronized English documentation (`maintenance_and_troubleshooting.md`, `faq.md`) with the French version to fix numbering, add missing sections, and update outdated information regarding graph loading.

## Version 1.0.106
- **Documentation**: Added a specific troubleshooting entry for Web UI crashes (Watchdog Timeout) caused by unstable SD card communication (CRC errors).

## Version 1.0.105
- **Project**: Added `.cursorrules` to establish global AI coding guidelines and project context.

## Version 1.0.104
- **Documentation**: Complete revision of user guides and FAQ to accurately reflect the hybrid storage strategy (SD primary, LittleFS cache) and current Web UI features.

## Version 1.0.103
- **Documentation**: Global update and verification of user documentation (Guides, FAQ, Architecture) to ensure consistency with latest features (Web UI, SD, Logs).

## Version 1.0.102
- **Web UI**: Unified footer across all pages with a single "Floppy" icon for the file manager and "Scroll" icon for logs.
- **UI (OLED)**: Added log scrolling functionality on the OLED screen using the "Confirm" button.

## Version 1.0.101
- **Refactoring (Web)**: Centralized footer code into `footer.js` for easier maintenance and consistency across all pages.
- **Feature (Web)**: Added buttons in the file manager to easily switch between internal memory and SD card.

## Version 1.0.100
- **Web UI (Logs)**: New `/logs` page with syntax highlighting (Info/Warn/Error) and auto-refresh.
- **Web UI (Files)**: Major file manager upgrade with folder navigation, breadcrumbs, and full SD card support.
- **Web UI (Access)**: Added a "Floppy Disk" icon for direct SD file access in the footer.

## Version 1.0.99
- **Feature (Web)**: Added a `/logs` page accessible via a footer icon to view system logs in real-time.
- **Fix (SD)**: Automatic check for `/history` folder existence before writing to prevent errors after formatting or card swapping.

## Version 1.0.98
- **UI (Safety)**: Added confirmation screens for "Clear Logs" and "Clear History" actions in the system menu to prevent accidental deletions.
- **Web UI**: Simplified SD card access shortcut to `/sd` (redirects to `/files.html?fs=sd`).

## Version 1.0.97
- **Improvement (Web)**: Added a `/sd-files` shortcut redirecting to the file manager in SD mode (`/files.html?fs=sd`) and logged the link at startup.
- **Documentation**: Updated power supply advice (3.3V vs 5V for SD module).

## Version 1.0.96
- **Documentation**: Added a troubleshooting note regarding potential interference between the SD card (power spikes) and I2C sensors, explaining the source of outlier values.

## Version 1.0.95
- **Fix (Sensors)**: Added filtering of outlier values (temperature, humidity, pressure) before saving to history to prevent data pollution from read errors.
- **Improvement (Web)**: Added a `/files` redirect to `/files.html` and logged the file manager URL at startup for easier access.

## Version 1.0.94
- **Fix (Compilation)**: Synchronized the `web_manager.h` header file with its implementation to resolve compilation errors related to SD card management in the web interface.

## Version 1.0.93
- **Feature (Storage)**: The SD card is now the primary storage for high-resolution history, with daily CSV files (`/history/YYYY-MM-DD.csv`).
- **Feature (Web UI)**: The web file manager now allows browsing and managing files on both the SD card and the internal storage.
- **Stability**: LittleFS archiving is kept as a 24h cache for fast reboots.
## Version 1.0.92
- **Documentation**: Added a troubleshooting section for I2C errors (`i2cRead returned Error -1`) and unexpected reboots, highlighting likely hardware causes (wiring, power supply).

## Version 1.0.91
- **UI**: Inverted rotary encoder direction for menu navigation for a more intuitive experience (clockwise = up).

## Version 1.0.90
- **Fix (SD Crash)**: Complete rewrite of the format function to be self-contained and robust, manually handling SPI bus initialization and memory allocation to prevent "Guru Meditation Error" crashes.

## Version 1.0.89
- **Fix (SD Crash)**: Used low-level `sdcard_init` functions for formatting, preventing the crash caused by `SD.begin` mount failure.

## Version 1.0.88
- **Fix (Crash)**: Resolved "Guru Meditation Error" crash during SD card formatting by forcing a hardware re-initialization before the operation.
- **Fix (UI)**: Simplified and stabilized the rotary encoder logic for consistent menu and page navigation on both OLED and LCD.

## Version 1.0.87
- **Fix (Compilation)**: Definitive fix for `getHeight` error in `ui_manager.cpp` by replacing the method call with a local constant defined by the environment (OLED/LCD).

## Version 1.0.86
- **Fix (Compilation)**: Resolved `'getHeight' is not a member of 'DisplayInterface'` compilation error by using environment-specific screen height constants.

## Version 1.0.85
- **Fix (UI)**: Fixed the SD card formatting process, which no longer reboots the device and now displays a success or failure message.
- **Feature (UI)**: The system menu is now scrollable on the OLED screen, making the "Format SD" option accessible.
- **Fix (UI)**: Enabled the "Clear Logs" function in the system menu.

## Version 1.0.84
- **Fix (Compilation)**: Resolved multiple compilation errors caused by the missing `BUTTON_GUARD_MS` definition and missing page headers (`pages_sh1106.h`, `pages_st7789.h`) in `main.cpp` and `ui_manager.cpp`.

## Version 1.0.83
- **Fix (Compilation)**: Fixed `display_interface.h` include path in `ui_manager.h` to resolve "No such file or directory" compilation error.

## Version 1.0.82
- **Feature (Maintenance)**: Added a "Format SD" option to the system menu to format the SD card (as FAT32) directly from the device. A confirmation step has been added to prevent accidental formatting.

## Version 1.0.78
- **Feature (Hardware)**: Added initial SD card support via `SdManager`.
- **Configuration**: Usage of `SD_CS_PIN`, `SPI_SCK_PIN`, `SPI_MISO_PIN`, `SPI_MOSI_PIN` definitions from `board_config.h`.

## Version 1.0.77
- **Documentation**: Added explanations on filesystem corruption (causes, prevention) and alternative storage methods (SD card, Cloud) in the FAQ and Maintenance guides.

## Version 1.0.76
- **Fix (System)**: Added filesystem (LittleFS) recovery procedure. Holding the BOOT button at startup now allows formatting memory in case of corruption.
- **Stability**: Explicit initialization of LittleFS and creation of `/history` folder at startup to prevent write errors and reboot loops (WDT).

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
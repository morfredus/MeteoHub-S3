# [1.0.176] – 2026-03-01
1. Fixed `esp32-dev-oled` build failure by enabling OLED display symbols for ESP32 Dev Module compile path.
2. Unified OLED compile guards so `main`, `ui_manager`, and OLED pages are built for both `esp32-s3-oled` and `esp32-dev-oled`.
3. Kept runtime functionality unchanged while restoring successful cross-environment compilation intent.
- Minimum valid version: 1.0.176

# [1.0.175] – 2026-03-01
1. Added a second PlatformIO environment `esp32-dev-oled` for ESP32 Dev Module builds with `default.csv` partition mapping for flash compatibility.
2. Added safe ESP32 Dev Module pin mapping in `include/board_config.h` while preserving existing ESP32-S3 mapping and runtime behavior.
3. Updated English/French documentation to describe the two board environments and board selection steps.
- Minimum valid version: 1.0.175

# [1.0.174] – 2026-02-27
- Hardened SD card management for ESP32-S3 3.3V modules with adaptive SPI mount retries and improved recovery flow.
- Added periodic SD health checks and safer reconnect backoff to reduce unstable card states.
- Improved history CSV writes with guarded directory creation, flush, and retry after remount.
- Minimum valid version: 1.0.174

# [1.0.173] – 2026-02-26
- Added OLED diagnostics API (FPS, render time, I2C errors) exposed in `/api/system`.
- System page in web UI now displays live OLED diagnostics.
- Minimum valid version: 1.0.173
# [1.0.172] – 2026-02-25
- Added and cross-linked beginner documentation (EN/FR) in all user-facing docs.
- All guides, FAQ, configuration, and index now reference beginner onboarding.
- Minimum valid version: 1.0.172

# [1.0.171] – 2026-02-25
- Added advanced scale management for temperature, humidity and pressure graphs.
- Three modes: fixed, dynamic, mixed (with configurable expansion).
- Interactive controls on web UI to select mode and percentage.
- Contextual help below the graph.
- Automatic synchronization between config.h and web UI.

# [1.0.170] - 2026-02-24
### Fixed
- Applied the graph safe-zone layout scheme to other OLED pages.
- Kept page titles in the top band and moved forecast/logs content start below SSD1306 reserved top area.
- Aligned log line spacing to avoid top-content overlap in yellow-band SSD1306 variants.

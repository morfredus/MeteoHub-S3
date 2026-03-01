# [1.0.179] – 2026-03-01
1. Reworked SD SPI bus handling to use a dedicated VSPI instance on ESP32 Dev Module, reducing bus-side side effects and improving SD command stability.
2. Aligned SD init/format low-level calls on the same selected SPI instance and added explicit SPI bus naming in diagnostics.
3. Extended ultra-safe SD init to include 100kHz fallback for difficult modules/wiring.
- Minimum valid version: 1.0.179

# [1.0.178] – 2026-03-01
1. Updated ESP32 Dev Module SD pin mapping to a safer CS pin (`GPIO13`) to avoid strap-related instability on `GPIO5`.
2. Added detailed SD diagnostics (card type, SPI pin states, per-attempt frequency/index, SPI bus init logs).
3. Added `esp32-dev-oled-ultra-safe` PlatformIO environment enabling `SD_ULTRA_SAFE_DEBUG` (very low SPI speeds and conservative drive strength) for SD troubleshooting.
- Minimum valid version: 1.0.178

# [1.0.177] – 2026-03-01
1. Changed `esp32-dev-oled` partition table from `default.csv` to `huge_app.csv` to provide enough flash space for the firmware binary.
2. Kept existing project functionality unchanged; only build memory layout for ESP32 Dev Module was adjusted.
- Minimum valid version: 1.0.177

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

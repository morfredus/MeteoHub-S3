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

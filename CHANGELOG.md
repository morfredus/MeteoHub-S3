# [1.0.174] – 2026-03-07
- Rewrote SdManager to use SPIClass(FSPI) at 40 MHz with format_if_fail=true, matching the validated reference implementation.
- Removed complex multi-frequency retry loop, low-level sdcard_init/sdcard_uninit FatFS calls, and manual GPIO drive-strength configuration.
- Replaced raw SPIClass pointer with std::unique_ptr<SPIClass> member in SdManager.
- Simplified format() to use SD.begin with format_if_fail=true instead of low-level f_mkfs.
- Minimum valid version: 1.0.174

# [1.0.173] – 2026-03-07
- Updated SD card SPI pin mapping to match physical wiring: CLK=9, MISO=10, MOSI=11, CS=12, DET=14.
- Added SD_DET_PIN support in SdManager: physical card presence detection via pull-up input (LOW = card inserted).
- SdManager now skips mount attempts when no card is physically detected, and immediately marks unavailable on hot removal.
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

# [1.0.177] – 2026-03-05
- Fixed compilation errors introduced by the hybrid SD backend alias (`SD`) by isolating SPI `::SD` usage in `sd_manager.cpp`.
- Added backend-specific storage metric accessors in `SdManager` (`cardSizeBytes`, `totalBytes`, `usedBytes`).
- Updated `system_info.cpp` to use `SdManager` metrics instead of `fs::FS` methods unavailable on the generic FS interface.
- Minimum valid version: 1.0.177

# [1.0.176] – 2026-03-05
- Added automatic SD backend selection: SPI is now attempted first, then SDIO (4-bit then 1-bit fallback).
- Updated SD manager internals to expose a unified `SD` filesystem alias over both SPI and SDIO backends.
- Updated SD wiring docs to describe pin labels used by 3.3V SPI/SDIO modules and the complete retry strategy.
- Updated `board_config.h` SD comments to match hybrid SPI/SDIO pin usage.
- Minimum valid version: 1.0.176

# [1.0.175] – 2026-03-04
- All alert states now make the Neopixel blink (red, orange, yellow).
- Only the 'no alert' state leaves the Neopixel steady.
- Improved orange (255,140,0) and vivid yellow (255,220,0) colors for better visibility.
- Neopixel brightness is now configurable in config.h (default: 15%).
- Minimum valid version: 1.0.175

# [1.0.173] – 2026-03-04
- Replaced SPI SD card module with native SDMMC 4-bit 3.3V module (pins: CLK, CMD, DAT0, DAT1, DAT2, DAT3/CD).
- Updated `board_config.h`: replaced SD_CS/SCK/MOSI/MISO pins with SD_CLK/CMD/DAT0/DAT1/DAT2/DAT3 (GPIOs 9, 10, 11, 12, 13, 14).
- Rewrote `sd_manager` to use ESP32 native SDMMC driver (`SD_MMC.h`) instead of SPI (`SD.h`).
- Added robust 4-bit → 1-bit fallback with multi-frequency retry logic for average-quality modules.
- Used `esp_vfs_fat_sdcard_format` for explicit user-triggered card formatting.
- Updated `system_info.cpp` to use `SD_MMC.h` include.
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

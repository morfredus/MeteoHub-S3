# [1.0.180] – 2026-03-03
- Switched SD support from legacy SPI module to microSD Sniffer (SD_MMC 1-bit mode).
- Added SD_MMC pin mapping (CLK/CMD/DAT0/CD) and migrated SD manager/history/system info to SD_MMC APIs.
- Minimum valid version: 1.0.180

# [1.0.179] – 2026-03-03
- Fixed encoder build issue robustly by replacing rotation debounce member constant with a single constexpr helper method (no duplicate declaration path).
- Kept one detent = one page/line change and per-controller debounce values (SH1106: 8ms, SSD1306: 4ms).
- Minimum valid version: 1.0.179

# [1.0.178] – 2026-03-03
- Fixed encoder compile error by consolidating rotation debounce constant into a single declaration in `encoder.h`.
- Kept per-hardware debounce values (SH1106/EC11: 8ms, SSD1306/KY-040: 4ms) with one notch per page/line change.
- Minimum valid version: 1.0.178

# [1.0.177] – 2026-03-03
- Fixed encoder detent mapping per hardware profile so both SH1106+EC11 and SSD1306+KY-040 now change page/line on a single detent.
- Encoder profile selection now follows OLED controller configuration (SH1106 vs SSD1306).
- Minimum valid version: 1.0.177

# [1.0.176] – 2026-03-03
- Tuned encoder step scaling per hardware profile: EC11 keeps existing behavior, KY-040 now requires only one detent to change one page/line.
- Minimum valid version: 1.0.176

# [1.0.175] – 2026-03-03
- Added EC11-specific hardening (HalfQuad mode + strong encoder filter) to reduce interrupt storms seen on SH1106 all-in-one modules.
- Added software debounce/rate-limit for encoder rotation events to prevent OLED/UI overload and freezes during knob manipulation.
- Minimum valid version: 1.0.175

# [1.0.174] – 2026-03-02
- Fixed remaining UI freezes/artifacts while turning the rotary encoder by throttling OLED redraw cadence.
- Reduced I2C contention by caching sensor reads for a short interval, avoiding repeated sensor polling during rapid page changes.
- Minimum valid version: 1.0.174

# [1.0.173] – 2026-03-02
- Fixed intermittent freezes by reducing blocking NeoPixel writes (cached color updates) and adding cooperative loop yielding.
- Reduced heap fragmentation risk by replacing log vector erase/push with a fixed circular buffer.
- Hardened OLED init by forcing normal orientation (no mirror/flip) and setting a stable I2C clock.
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

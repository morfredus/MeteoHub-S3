# Changelog

Minimum valid version: 1.0.167

## [1.0.167] - 2026-02-24
### Fixed
- Fixed OLED page compilation by explicitly including `config.h` where OLED controller macros are used.
- Replaced constexpr ternary on controller macros with preprocessor constants to satisfy strict compile-time evaluation.
- Kept SSD1306 safe top-zone behavior to avoid overlap with yellow-band panels.

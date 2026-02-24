# Changelog

Minimum valid version: 1.0.166

## [1.0.166] - 2026-02-24
### Fixed
- Added a safe top rendering offset for SSD1306 to avoid overlap with yellow band variants.
- Shifted all OLED pages (header, lines, graphs, forecast, logs, boot screens) to respect the reserved top area on SSD1306 panels.

### Notes
- OLED-only target remains unchanged.

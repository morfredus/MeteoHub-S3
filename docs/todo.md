# OLED and UX TODO roadmap

Minimum valid version: 1.0.154

## Code TODO
- [x] Rename `Sh1106Display` to a neutral name (`OledDisplay`) to better reflect the current U8g2 backend that supports both SH1106 and SSD1306.
- [x] Centralize OLED geometry constants (`width`, `height`, margins) to remove magic numbers in `pages_oled.cpp`.
- [ ] Add a small display diagnostics API (current FPS, last render duration, I2C error counter) exposed in `/api/system`.
- [ ] Add an optional build flag to compile only one OLED controller backend (`SH1106` or `SSD1306`) to reduce binary size.
- [ ] Add host-side unit tests for pure helper functions in `pages_oled.cpp` (formatting, title generation, alert translation).

## User experience TODO
- [ ] Add a quick OLED settings page (contrast + I2C address preview) directly on-device.
- [ ] Add a first-boot OLED wizard that validates wiring (SDA/SCL/address) and displays clear next steps when no panel is detected.
- [ ] Add a non-blocking toast/status line when SD reconnect attempts are running, so users understand temporary data-save failures.
- [ ] Add configurable page transition speed and graph density for better readability on different use cases.
- [ ] Add context-sensitive help on long press (legend/icons/help hints per screen).

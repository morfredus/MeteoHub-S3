# OLED and UX TODO roadmap

Minimum valid version: 1.0.160

## Code TODO
- [x] Rename `Sh1106Display` to a neutral name (`OledDisplay`) to better reflect the current U8g2 backend that supports both SH1106 and SSD1306.
- [x] Centralize OLED geometry constants (`width`, `height`, margins) to remove magic numbers in `pages_oled.cpp`.
- [ ] Add a small display diagnostics API (current FPS, last render duration, I2C error counter) exposed in `/api/system`.
- [ ] Add an optional build flag to compile only one OLED controller backend (`SH1106` or `SSD1306`) to reduce binary size.
- [ ] Add host-side unit tests for pure helper functions in `pages_oled.cpp` (formatting, title generation, alert translation).
- [x] Replace blocking UI waits (`delay(1000/2000)`) in OLED flows with a non-blocking state machine based on `millis()`.
- [x] Introduce a shared cooperative yield helper/macro (`delay(0)` every N iterations) and use it consistently in long history/files loops.
- [ ] Reduce frequent `std::string` allocations in OLED rendering paths by using reusable formatting buffers for hot pages.
- [ ] Add smart refresh policies by page type (refresh only when values change on static pages, faster on live pages).

## OLED UTF-8 Rendering

From version 1.0.155, the OLED backend (`OledDisplay`) supports UTF-8 rendering for all text, including accented characters and special symbols. The display module is now named `OledDisplay` to reflect its neutrality and compatibility with both SH1106 and SSD1306 controllers.

## User experience TODO
- [ ] Add a quick OLED settings page (contrast + I2C address preview) directly on-device.
- [ ] Add a first-boot OLED wizard that validates wiring (SDA/SCL/address) and displays clear next steps when no panel is detected.
- [ ] Add a non-blocking toast/status line when SD reconnect attempts are running, so users understand temporary data-save failures.
- [ ] Add configurable page transition speed and graph density for better readability on different use cases.
- [ ] Add context-sensitive help on long press (legend/icons/help hints per screen).
- [ ] Add persistent on-screen navigation hints (`OK/Menu`, `Back`, `Rotate`) on compact OLED pages.
- [ ] Pause auto-rotation of forecast views for a short period after user input to improve readability.
- [ ] Improve `Ciel:` rendering with a richer abbreviation dictionary and truncation on whole words.
- [ ] Add a dedicated OLED quick status page (`Wi-Fi`, `NTP`, `SD`, API health) for field diagnostics without web access.
- [ ] Define and document a "OLED minimal + web advanced" product profile to keep OLED concise and move detailed flows to web UI.

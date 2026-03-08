# TODO

**Beginner?** See [Beginner's Guide](../beginner/index.md) or [Guide Débutant](../beginner/index_fr.md)

Minimum valid version: 1.0.155
Minimum valid version: 1.0.181
## Code TODO
- [x] Rename `Sh1106Display` to a neutral name (`OledDisplay`) to better reflect the current U8g2 backend that supports both SH1106 and SSD1306.
- [x] Centralize OLED geometry constants (`width`, `height`, margins) to remove magic numbers in `pages_oled.cpp`.
- [ ] Add a small display diagnostics API (current FPS, last render duration, I2C error counter) exposed in `/api/system`.
- [ ] Add an optional build flag to compile only one OLED controller backend (`SH1106` or `SSD1306`) to reduce binary size.
- [ ] Add host-side unit tests for pure helper functions in `pages_oled.cpp` (formatting, title generation, alert translation).

## OLED UTF-8 Rendering

From version 1.0.155, the OLED backend (`OledDisplay`) supports UTF-8 rendering for all text, including accented characters and special symbols. The display module is now named `OledDisplay` to reflect its neutrality and compatibility with both SH1106 and SSD1306 controllers.

## User experience TODO
- [ ] Add configurable page transition speed and graph density for better readability on different use cases.
- [ ] Add context-sensitive help on long press (legend/icons/help hints per screen).

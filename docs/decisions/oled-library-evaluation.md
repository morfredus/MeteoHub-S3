# OLED library evaluation: SH1106/SSD1306Wire vs U8g2

## Short answer
For **MeteoHub-S3 today**, a full migration to U8g2 is **not mandatory** and should be treated as an optimization/feature project, not an urgent fix.

## Why staying on the current stack is reasonable now
- The current code already supports both SH1106 and SSD1306 controllers with explicit compile-time selection.
- OLED initialization is deterministic with configurable I2C address (`0x3C` / `0x3D`), which solves the reliability issue this project recently addressed.
- Existing UI rendering code is built around the current API; switching libraries would touch drawing, text layout, and potentially startup/init flows.

## What U8g2 would improve
- Broader controller support and long-term portability.
- Better typography options (fonts) and mature text rendering tools.
- A very common ecosystem choice for monochrome displays.

## Costs/risks of migration
- Non-trivial refactor of display wrapper and UI code.
- Flash/RAM impact can change depending on selected fonts and drawing mode.
- Regression risk on rendering timing/flicker and on constrained targets.

## Recommendation
1. Keep the current display backend for the upcoming patch releases.
2. If needed, plan a dedicated migration branch to benchmark U8g2 with:
   - identical screens,
   - same update frequency,
   - memory footprint measurements,
   - boot-to-first-frame timing.
3. Migrate only if measurements show clear wins (fonts, compatibility, maintainability) without harming performance.

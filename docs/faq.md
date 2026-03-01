# FAQ

**Beginner?** See [Beginner's Guide](../beginner/index.md) or [Guide Débutant](../beginner/index_fr.md)

Minimum valid version: 1.0.170

## Which display is supported?
Only OLED (SH1106/SSD1306).

## Which PlatformIO environment should I use?
Use the environment that matches your board:
- `esp32-s3-oled` for ESP32-S3 DevKitC-1
- `esp32-dev-oled` for ESP32 Dev Module
- `esp32-dev-oled-ultra-safe` for deep SD debugging on ESP32 Dev Module

## Is SD card mandatory?
No, it is optional for long-term archival.

## My SSD1306 has a yellow top band. Can UI overlap it?
The page title stays on the top line (yellow band area), while the page content area is kept below the reserved SSD1306 top zone to avoid overlap.

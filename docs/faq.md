## Weather Alert Description
The API /api/alert now returns the full alert description (in French when available). The web dashboard displays this text for maximum clarity and localization.
# FAQ

Minimum valid version: 1.0.154

## Is internet access required?
Yes, for weather forecast and alert updates. Local sensor pages still work without internet.

## Can I use only one sensor?
Yes. The project handles partial sensor availability and applies fallback behavior.

## Why are graphs empty after a reboot?
This should no longer happen in normal operation. The system reloads recent history at startup, so graphs are immediately available on OLED, LCD, and Web UI.

## What is the forecast refresh rate?
About every 30 minutes.

## How do I access the Web UI?
Connect to the same Wi-Fi network and open `http://meteohub.local`. If it does not resolve, use the device IP shown on the Network page.

## What is the history capacity?
- **Short-term (graphs):** about 24 hours of high-resolution data (1 point/min) in RAM with persistence.
- **Long-term:** practically unlimited (depends on SD card size) in daily CSV files.

## How do I download my data?
From the Web UI, click the floppy icon 💾 in the footer and download CSV files from `/history/YYYY-MM-DD.csv`.

## Can I change page refresh speed?
Yes, edit `DASHBOARD_REFRESH_MS` in `include/config.h`.

## How do I change LCD resolution (240x240 vs 240x320)?
Set `LCD_WIDTH` and `LCD_HEIGHT` in `include/config.h`. The UI adapts automatically after reboot.

## Can I change OLED contrast?
Yes, set `OLED_CONTRAST` in `include/config.h`.

## How can I prevent data corruption or history loss?
Filesystem corruption is rare but can happen after abrupt power loss.
1. **Stable power first:** use a reliable USB power supply and cable.
2. **Emergency recovery (LittleFS format):** if the device boot-loops, unplug power, hold **BOOT**, plug power back in, wait for the maintenance message, then release. The device formats internal storage and reboots (history is lost).
3. **Maximum robustness (advanced):** store history on SD card and/or forward data to cloud services (MQTT, ThingSpeak, etc.).

## Does the project support both OLED and LCD displays?
No runtime auto-detection is required: choose `esp32-s3-oled` or `esp32-s3-lcd` at build time, then flash the firmware.

## What are the key differences between OLED and LCD environments?
- **OLED (SH1106):** monochrome, 128x64, rotary/button navigation specific to OLED layout.
- **LCD (ST7789):** color, 240x240/320, adapted navigation/layout for LCD.

## Where can I change GPIO mapping?
In `include/board_config.h`.

## What if upload fails repeatedly?
Try another USB cable, verify COM/serial port, close serial monitor, then retry upload.

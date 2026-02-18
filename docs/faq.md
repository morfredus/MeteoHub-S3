# FAQ

Minimum valid version: 1.0.104

## Is internet required?
Yes, for forecast and alert updates. Local sensor pages still work without internet.

## Can I use only one sensor?
Yes. The project supports partial sensor availability and fallback behavior.

## Why are graphs empty after reboot?
History needs time samples and valid system time (NTP) to record entries.

## How often is forecast updated?
Approximately every 30 minutes.

## How many history points are stored?
128 points are kept in memory and persisted in LittleFS.

## Can I change page refresh speed?
Yes, adjust `DASHBOARD_REFRESH_MS` in `include/config.h`.

## How can I prevent data corruption or history loss?
Filesystem corruption is rare but can happen during a power outage.
1.  **Prevention #1: Stable Power.** Use a high-quality USB power supply and a reliable cable. This is the most common source of issues.
2.  **Recovery:** If corruption occurs, version 1.0.76+ includes a recovery mode. Hold the BOOT button at startup to format the partition and start fresh (history will be lost).
3.  **Maximum Robustness (Advanced):** For critical applications, the safest alternatives are to store data on an **SD card** (requires a hardware modification) or send it to a **Cloud service** (e.g., ThingSpeak, MQTT broker), which protects it from any local hardware failure.

The firmware already uses robust writing techniques (appending data rather than full rewrites) to minimize wear and risks.



## Does the project support both OLED and LCD displays?
Yes. The firmware automatically detects and adapts to either an SH1106 OLED (128x64, monochrome) or a TFT ST7789 LCD (240x240, color) at startup. No code change is needed.

## What are the main differences between OLED and LCD environments?
- **OLED (SH1106)**: 2 buttons + rotary encoder, monochrome, 128x64, navigation = 2 detents per page.
- **LCD (ST7789)**: 3 buttons + rotary encoder, color, 240x240, navigation = 1 detent per page.
The UI and page layout adapt automatically to the detected display.

## How does navigation differ between OLED and LCD?
On OLED, you must turn the encoder two detents to change page; on LCD, one detent is enough. Button mapping also differs (see user guide and hardware wiring).

## Can I switch between OLED and LCD without changing the code?
Yes. The firmware auto-detects the connected display at each boot.

## Can I change OLED contrast?
Yes, adjust `OLED_CONTRAST` in `include/config.h`.

## Where do I change board pin mapping?
In `include/board_config.h`.

## What if upload fails repeatedly?
Try another USB cable, verify COM port, close serial monitor, then retry upload.

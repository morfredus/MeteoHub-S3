# FAQ

Minimum valid version: 1.0.115

## Is internet required?
Yes, for forecast and alert updates. Local sensor pages still work without internet.

## Can I use only one sensor?
Yes. The project supports partial sensor availability and fallback behavior.

## Why are graphs empty after reboot?
This issue is resolved. The system now reloads the last 24 hours of history at startup, making the graphs immediately available on all screens (OLED, LCD, and Web).

## How often is forecast updated?
Approximately every 30 minutes.

## How many history points are stored?
- **Short-term (for graphs)**: ~24 hours of high-resolution data (1 point/min) are kept in RAM and backed up.
- **Long-term**: Unlimited (depending on SD card size) as daily CSV files.

## How do I retrieve my data?
Go to the web interface, click the floppy disk icon 💾 at the bottom of the page. You can browse the SD card and download the CSV files (`/history/YYYY-MM-DD.csv`).

## Can I change page refresh speed?
Yes, adjust `DASHBOARD_REFRESH_MS` in `include/config.h`.

## How do I change the LCD resolution (240x240 vs 240x320)?
Modify the `LCD_WIDTH` and `LCD_HEIGHT` constants in `include/config.h`. The UI will automatically adapt to the new resolution on the next boot.

## Can I change OLED contrast?
Yes, adjust `OLED_CONTRAST` in `include/config.h`.

## How can I prevent data corruption or history loss?
Filesystem corruption is rare but can happen during a power outage.
1.  **Prevention #1: Stable Power.** Use a high-quality USB power supply and a reliable cable. This is the most common source of issues.
2.  **Easy recovery (emergency format):**
	- If the device is stuck or keeps rebooting, you can force an emergency format of the internal memory (LittleFS) without a computer: unplug the power, hold the **BOOT** button, plug the power back in, wait for the maintenance message on the screen, then release the button. The device will erase the memory and reboot cleanly (all history data will be lost).
	- This procedure is accessible to any user, even beginners.
3.  **Maximum Robustness (Advanced):** For critical applications, the safest alternatives are to store data on an **SD card** (requires a hardware modification) or send it to a **Cloud service** (e.g., ThingSpeak, MQTT broker), which protects it from any local hardware failure.

The firmware already uses robust writing techniques (appending data rather than full rewrites) to minimize wear and risks.



## Does the project support both OLED and LCD displays?
Yes. The firmware automatically detects and adapts to either an SH1106 OLED (128x64, monochrome) or a TFT ST7789 LCD (240x240, color) at startup. No code change is needed.

## What are the main differences between OLED and LCD environments?
- **OLED (SH1106)**: 2 buttons + rotary encoder, monochrome, 128x64, navigation = 2 detents per page.
- **LCD (ST7789)**: 3 buttons + rotary encoder, color, 240x240, navigation = 1 detent per page.
The UI and page layout adapt automatically to the detected display.

## How does navigation differ between OLED and LCD?
On OLED, you must turn the encoder two detents to change a page; on LCD, one detent is enough. Button mapping also differs (see user guide and hardware wiring).

## Where do I change board pin mapping?
In `include/board_config.h`.

## What if upload fails repeatedly?
Try another USB cable, verify COM port, close serial monitor, then retry upload.

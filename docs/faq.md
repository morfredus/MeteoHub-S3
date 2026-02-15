# FAQ

Minimum valid version: 1.0.22

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

## Can I change OLED contrast?
Yes, adjust `OLED_CONTRAST` in `include/config.h`.

## Where do I change board pin mapping?
In `include/board_config.h`.

## What if upload fails repeatedly?
Try another USB cable, verify COM port, close serial monitor, then retry upload.

# User Guide

Minimum valid version: 1.0.30

## Goal
Learn how to use the dashboard once firmware is running.


## OLED vs LCD: Navigation and Display
- **OLED (SH1106)**: Monochrome, 128x64, 2 buttons + rotary encoder. Navigation: 2 detents = 1 page.
- **LCD (TFT ST7789)**: Color, 240x240, 3 buttons + rotary encoder. Navigation: 1 detent = 1 page.
- The UI and page layout adapt automatically to the detected display.

## Input controls
- **Encoder rotation**: navigate pages
- **Encoder click**: context action (forecast view switch or menu action)
- **Back button**: previous page or close menu
- **Confirm button**: open menu

## Boot sequence
At startup, OLED shows progress steps:
1. Boot
2. Sensors init
3. Wi-Fi connection
4. Time sync (NTP)
5. Ready

## Main pages
The UI manager cycles through these pages:
- Weather
- Forecast
- Graph Temp
- Graph Humidity
- Graph Pressure
- Network
- System (LCD: position 7)
- Logs (LCD: position 8)

### Graph scale behavior
- Graph min/max labels are computed from the history currently displayed on screen.
- Temperature and Pressure labels can show decimals to keep visible separation when values are close.

## Forecast page behavior
- Auto-cycles every ~5 seconds between:
  - Today
  - Tomorrow
  - Alerts
- Manual cycle possible with encoder click.

## Alert logic (NeoPixel)
- **Orange blinking**: active weather alert
- **Red blinking**: local high temperature alert (> 28°C)
- **Green steady**: Wi-Fi connected and no alert
- **Red steady**: Wi-Fi disconnected and no weather alert

## Weather alert selection behavior
- The system reviews all weather alerts returned by the API and keeps the most critical one for display.
- When two alerts have equivalent severity, flood/rising-water related alerts are prioritized over generic rain alerts.

## Menu actions
Menu contains:
- Return
- Reboot
- Clear logs
- Clear history

## Data persistence
- Last page is stored in Preferences (NVS).
- Sensor history is stored in LittleFS (`/history.dat`).

## Reading weather values and interpretation
### Temperature (Temp)
- Indicates local ambient air temperature around the device.
- Fast rises during the day are normal with sun exposure; compare with humidity and pressure trend before concluding a weather change.
- For comfort, combine with humidity: warm + humid usually feels hotter than the raw temperature value.

### Humidity (Hum)
- Relative humidity (%) shows how close air is to saturation.
- High humidity (often > 70%) can indicate muggy conditions and a higher chance of fog/drizzle when temperatures drop.
- Very low humidity (often < 35%) usually corresponds to drier air and larger day/night temperature swings.

### Pressure (Pres) — key indicator
- Pressure trend is usually more informative than a single value.
- Falling pressure over several hours often suggests weather deterioration (wind/rain risk increasing).
- Rising pressure over several hours often suggests improving and more stable weather.
- Stable pressure usually indicates limited short-term weather change.

### Quick pressure reference (approximate)
- Around 1013 hPa: neutral baseline.
- Clearly below baseline (for example < 1005 hPa): unsettled/depression-like context more likely.
- Clearly above baseline (for example > 1020 hPa): more stable anticyclonic context more likely.
- These thresholds are indicative and should be interpreted together with local altitude and trend.

### Practical method for this dashboard
- First read the pressure graph trend over the last 1–2 hours.
- Then confirm with humidity direction (rising or falling) and current temperature behavior.
- Finally cross-check with Forecast/Alerts page before making a decision.

## Good usage practices
- Let the system run a few minutes before judging graph trends.
- Keep stable Wi-Fi for reliable forecast refresh.
- Review logs page when something looks wrong.

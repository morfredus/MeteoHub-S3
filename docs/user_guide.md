# User Guide

Minimum valid version: 1.0.104

## Goal
Learn how to use the dashboard once the firmware is running.

## OLED vs LCD: Navigation and Display
- **OLED (SH1106)**: Monochrome, 128x64, 2 buttons + rotary encoder. Navigation: 2 detents = 1 page.
- **LCD (TFT ST7789)**: Color, 240x240 or 240x320, 3 buttons + rotary encoder. Navigation: 1 detent = 1 page.
- The interface and page layout adapt automatically to the detected display.

## User Controls
- **Encoder Rotation**: Navigate between pages.
- **Encoder Click**: Open menu or validate an option.
- **Back Button**: Previous page or close menu.
- **Confirm Button**: Contextual action (e.g., cycle forecast views) or validation.

## Boot Sequence
At boot, the screen displays a "morfredus" Splash Screen followed by a progress bar:
1. Splash Screen (Studio Logo & Project)
2. Sensor Initialization
3. Wi-Fi Connection
4. Time Synchronization (NTP)
5. Ready

## Web Interface
MeteoHub S3 features a complete Web interface accessible from any browser on the same Wi-Fi network.

### Access
- Address: `http://meteohub.local` (or via the IP address displayed on the screen at startup).

### Web Features
- **Dashboard**: Real-time display of Temperature, Humidity, and Pressure.
- **Graphs**: Interactive visualization of the 3 curves over the last 24h (high resolution).
- **Statistics**: Dedicated page displaying Min/Avg/Max for the current period.
- **Long Term History**: View archived data over months/years.
- **File Manager**: Accessible via the floppy disk icon 💾 in the footer. Allows browsing internal memory and SD card, downloading or deleting files.
- **System Logs**: Accessible via the scroll icon 📜 in the footer. Displays system events in real-time.

## Main Pages
The UI manager cycles through these pages:
- Weather
- Forecast
- Temperature Graph
- Humidity Graph
- Pressure Graph
- Network
- System (LCD: position 7)
- Logs (LCD: position 8)

### Graph Scaling Behavior
- Graph min/max labels are computed from the history currently displayed on screen.
- Temperature and Pressure graphs may display decimals to keep a visible difference when values are close.

## Forecast Page Behavior
- Automatic cycling every ~5 seconds between:
  - Today
  - Tomorrow
  - Alerts
- Manual cycling possible via the **Confirm** button.

## Alert Logic (NeoPixel)
- **Blinking Orange**: Weather alert active.
- **Blinking Red**: Local high temperature alert (> 28°C).
- **Solid Green**: Wi-Fi connected, no alert.
- **Solid Red**: Wi-Fi disconnected, no weather alert.

## Weather Alert Selection Behavior
- The system evaluates all weather alerts returned by the API and keeps the most critical one for display.
- At equivalent severity, flood/rising-water alerts are prioritized over generic rain alerts.

## Menu Actions
The menu contains:
- Back
- Reboot
- Clear Logs
- Clear History
- Format SD (if present)

## Data Persistence
- The last active page is stored in Preferences (NVS).
- **Short Term History**: The last 24h (1 point/min) are in RAM and continuously saved to `/history/recent.dat`. The system reloads this data at startup for immediate availability.
- **Long Term History**: If an SD card is present, data is archived daily in CSV format (`/history/YYYY-MM-DD.csv`). This is the recommended storage mode for archiving.

## Reading Weather Values and Interpretation
### Temperature (Temp)
- Represents the local air temperature around the device.
- A rapid rise during the day can be normal with sunlight; cross-check with humidity and pressure trend before concluding a weather change.

### Humidity (Hum)
- Relative humidity (%) indicates how close the air is to saturation.
- High humidity (often > 70%) can signal heavy air and increased risk of mist/light rain when temperature drops.
- Low humidity (often < 35%) usually corresponds to drier air and more marked day/night differences.

### Pressure (Pres) — Main Indicator
- Pressure trend is generally more useful than an instantaneous value.
- Pressure dropping over several hours often indicates weather deterioration (increasing wind/rain risk).
- Pressure rising over several hours often indicates improvement and more stable weather.
- Stable pressure suggests little short-term weather change.

### Quick Pressure Reference (Approximate)
- Around 1013 hPa: Neutral reference.
- Significantly below (e.g., < 1005 hPa): Likely disturbed/low-pressure context.
- Significantly above (e.g., > 1020 hPa): Likely stable/high-pressure context.
- These thresholds are indicative and must be interpreted with local altitude and trend.

### Practical Method on this Dashboard
- First read the pressure graph trend over the last 1-2 hours.
- Then confirm with humidity evolution (rise/fall) and temperature behavior.
- Finally check the Forecast/Alerts page before deciding.

## Good Usage Practices
- Let the system run for a few minutes before interpreting graph trends.
- Keep a stable Wi-Fi for reliable forecast updates.
- Check the logs page in case of abnormal behavior.
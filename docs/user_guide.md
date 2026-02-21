## Weather Alert Description

The API /api/alert now returns the full alert description (in French when available). The dashboard web displays this text for maximum clarity and localization. If description is empty, the event name is used as fallback.

## Data Display: Dashboard vs History

### Dashboard
The dashboard graph shows only the last 2 hours of sensor data (temperature, humidity, pressure). This allows for real-time monitoring and quick trend visualization.

### History Page
The history page graph displays the last 24 hours of data, useful for long-term analysis and comparison.

This separation ensures the dashboard remains fast and readable, while the history page provides detailed context.

## Weather Alert Card (Dashboard)

### What is the Weather Alert?
The dashboard always displays a weather alert card at the top. This card shows the current alert (if any) or "No alert". The color of the card changes depending on the severity of the alert (e.g., yellow for moderate, red for severe, green for no alert).

### How does it work?
- The alert card is always visible, even if there is no alert.
- The information is updated in real time from the `/api/alert` endpoint.
- If there is no alert, the card shows "No alert" and is green.
- If an alert is active, the card displays the sender, event, and severity.

### Example (API response)
```
GET /api/alert
{
  "active": true,
  "sender": "Meteo France",
  "event": "Storm Warning",
  "severity": "severe"
}
```
If no alert:
```
GET /api/alert
{
  "active": false
}
```

## Live Sensor Data (API)

The `/api/live` endpoint returns the latest sensor values (temperature, humidity, pressure) in real time. This is used for the dashboard and statistics.

### Example (API response)
```
GET /api/live
{
  "temp": 22.5,
  "hum": 45.0,
  "pres": 1013.2,
  "wifi_rssi": -60,
  "uptime": 123456
}
```

## Strict Project Compliance

All API routes are declared only inside the `_setupApi()` method. No forbidden comments or placeholders exist anywhere in the codebase. The documentation is always synchronized with the program and is beginner-friendly.

# User Guide

Minimum valid version: 1.0.119

## Goal
Learn how to use the dashboard once the firmware has started.

## OLED vs. LCD: Navigation and Display
- **OLED (SH1106)**: Monochrome, 128x64, 2 buttons + rotary encoder. Navigation: 2 detents = 1 page.
- **LCD (TFT ST7789)**: Color, 240x240 or 240x320, 3 buttons + rotary encoder. Navigation: 1 detent = 1 page.
- The interface and page layout automatically adapt to the detected display.

## User Controls
- **Encoder Rotation**: Navigate between pages.
- **Encoder Click**: Open the menu or confirm an option.
- **Back Button**: Go to the previous page or close the menu.
- **Confirm Button**: Contextual action (e.g., cycle through forecasts, scroll logs on OLED) or confirmation.

## Boot Sequence
On boot, the screen displays a "morfredus" Splash Screen followed by a progress bar:
1. Splash Screen (Studio & Project Logo)
2. Initializing sensors
3. Connecting to Wi-Fi
4. Synchronizing time (NTP)
5. Ready

## Web Interface
The MeteoHub S3 features a comprehensive web interface accessible from any browser on the same Wi-Fi network.

### Access
- Address: `http://meteohub.local` (or via the IP address displayed on the screen at startup).

### Web Features
- **Dashboard**: Real-time display of Temperature, Humidity, and Pressure.
- **Graphs**: Interactive visualization of the 3 curves over the last 24 hours (high resolution).
- **Statistics**: Dedicated page showing Min/Avg/Max for the current period.
- **Long-Term History**: View archived data over several months/years.
- **File Manager**: Accessible via the floppy disk icon 💾 at the bottom of the page. Allows browsing internal memory and the SD card, downloading, or deleting files.
- **System Logs**: Accessible via the scroll icon 📜 at the bottom of the page. Displays real-time system events.

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

### Graph Scale Behavior
- The min/max labels on the graphs are calculated from the history currently displayed on the screen.
- Temperature and Pressure graphs may display decimals to maintain a visible difference when values are close.

## Forecast Page Behavior
- Automatically cycles every ~5 seconds between:
  - Today
  - Tomorrow
  - Alerts
- Manual cycling is possible with the **Confirm** button.

## Alert Logic (NeoPixel)
- **Blinking Orange**: Active weather alert.
- **Blinking Red**: Local high-temperature alert (> 28°C).
- **Solid Green**: Wi-Fi connected with no alerts.
- **Solid Red**: Wi-Fi disconnected with no weather alerts.

## Weather Alert Selection Behavior
- The system analyzes all weather alerts returned by the API and keeps the most critical one for display.
- At equal severity, alerts related to floods/rising water are prioritized over generic rain alerts.

## Menu Actions
The menu contains:
- Back
- Reboot
- Clear Logs
- Clear History
- Format SD Card (if present)

## Data Persistence
- The last active page is stored in Preferences (NVS).
- **Short-Term History**: The last 24 hours (1 point/min) are kept in RAM and continuously saved to `/history/recent.dat`. The system reloads this data at startup for immediate availability.
- **Long-Term History**: If an SD card is present, data is archived daily in CSV format (`/history/YYYY-MM-DD.csv`). This is the recommended storage mode for archiving.

## Reading and Interpreting Weather Values
### Temperature (Temp)
- Represents the local air temperature around the device.
- A rapid increase during the day can be normal with sunlight; it should be cross-referenced with humidity and pressure trends before concluding a weather change.
- For perceived temperature, combine with humidity: hot + humid is often perceived as warmer than the raw value.

### Humidity (Hum)
- Relative humidity (%) indicates how close the air is to saturation.
- High humidity (often > 70%) can signal a heavy atmosphere and an increased risk of mist/light rain when the temperature drops.
- Low humidity (often < 35%) generally corresponds to drier air and more significant day/night temperature differences.

### Pressure (Pres) — The Main Indicator
- The pressure trend is generally more useful than an instantaneous value.
- A pressure drop over several hours often indicates deteriorating weather (increased risk of wind/rain).
- A pressure rise over several hours often indicates an improvement and more stable weather.
- Stable pressure suggests little short-term weather change.

### Quick Pressure Benchmarks (Approximate)
- Around 1013 hPa: Neutral reference.
- Significantly below this reference (e.g., < 1005 hPa): More disturbed/low-pressure context likely.
- Significantly above (e.g., > 1020 hPa): More stable/high-pressure context likely.
- These thresholds are indicative and should be interpreted with local altitude and the trend.

### Practical Method on this Dashboard
- First, read the pressure graph trend over the last 1 to 2 hours.
- Then, confirm with the evolution of humidity (rise/fall) and temperature behavior.
- Finally, check the Forecast/Alerts page before making a decision.

## Best Practices
- Let the system run for a few minutes before interpreting graph trends.
- Maintain a stable Wi-Fi connection for reliable forecast updates.
- Check the logs page in case of abnormal behavior.
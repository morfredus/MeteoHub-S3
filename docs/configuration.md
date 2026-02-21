# Configuration

Minimum valid version: 1.0.127

## New Features (since 1.0.127)
- **Weather Alert Card**: The dashboard always displays a weather alert card (via `/api/alert`).
- **Live Sensor Data**: The `/api/live` endpoint returns real sensor values (temperature, humidity, pressure).
- **Dashboard vs History**: The dashboard graph shows the last 2 hours; the history page shows 24 hours.
- **Strict Project Compliance**: All API routes are declared only inside `_setupApi()`. No forbidden comments or placeholders exist in the codebase.


## Goal
Configure network, weather API, and runtime constants safely.

## Files you may edit
### 1) `include/secrets.h`
Create this file from `include/secrets_example.h`.

Use this file for private data:
- Wi-Fi credentials array (`WIFI_CREDENTIALS`)
- OpenWeatherMap API key (`OWM_API_KEY`)
- Coordinates (`OWM_LAT`, `OWM_LON`)
- Units and language (`OWM_UNITS`, `OWM_LANG`)

### 2) `include/config.h`
Use this file for functional tuning:
- Dashboard refresh interval (`DASHBOARD_REFRESH_MS`)
- Wi-Fi retry delay (`WIFI_RETRY_DELAY_MS`)
- OLED contrast (`OLED_CONTRAST`)
- Ping test flag (`ENABLE_PING_TEST`)


## OLED/LCD environments
The firmware supports both SH1106 OLED and ST7789 LCD. Display type, pin mapping, and navigation are auto-detected at boot. See user guide and hardware wiring for details.

## File you should not edit casually
### `include/board_config.h`
This file defines GPIO mapping and should only be changed after hardware decision.

## Platform version metadata
Project name and version are injected from `platformio.ini` using build flags:
- `PROJECT_NAME`
- `PROJECT_VERSION`

Do not hardcode project name/version elsewhere.

## Secure setup tips
- `include/secrets.h` must be created from `include/secrets_example.h`.
- Never push `include/secrets.h` to GitHub.
- Never share `include/secrets.h` outside your trusted local environment.

## Quick configuration sanity check
- Build works (`platformio run`)
- Wi-Fi connects at boot
- Forecast page populates data
- Logs page shows network/weather events

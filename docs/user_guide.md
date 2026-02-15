# User Guide

Minimum valid version: 1.0.20

## Goal
Learn how to use the dashboard once firmware is running.

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
- Logs
- System

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

## Menu actions
Menu contains:
- Return
- Reboot
- Clear logs
- Clear history

## Data persistence
- Last page is stored in Preferences (NVS).
- Sensor history is stored in LittleFS (`/history.dat`).

## Good usage practices
- Let the system run a few minutes before judging graph trends.
- Keep stable Wi-Fi for reliable forecast refresh.
- Review logs page when something looks wrong.

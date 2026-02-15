# Maintenance and Troubleshooting

Minimum valid version: 1.0.22

## Goal
Provide practical recovery steps when the dashboard does not behave as expected.

## Build and upload checks
- Build: `platformio run`
- Upload: `platformio run --target upload`
- Clean build cache: `platformio run --target clean`

## Typical issues
### 1) Wi-Fi never connects
Check:
- `WIFI_CREDENTIALS` values in `include/secrets.h`
- Router availability
- Signal quality near board

### 2) Forecast page is empty
Check:
- `OWM_API_KEY` validity
- Internet connectivity
- Latitude/longitude values
- Free API quota limits

### 3) OLED is blank
Check:
- SDA/SCL wiring (GPIO 15/16)
- OLED power and ground
- Correct SH1106 display type

### 4) Sensors invalid or missing
Check:
- AHT20/BMP280 wiring
- I2C pull-up quality
- Sensor power stability

### 5) Graphs do not show values
Check:
- NTP synchronization completed
- Device has been running long enough to record samples
- History not recently cleared

## Maintenance actions available in UI menu
- Reboot
- Clear logs
- Clear history

## Safe update workflow
1. Save changes.
2. Build locally.
3. Upload and observe boot sequence.
4. Verify pages and LED statuses.
5. Check logs page for anomalies.

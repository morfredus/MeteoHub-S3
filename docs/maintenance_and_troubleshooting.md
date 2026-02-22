# Maintenance and Troubleshooting

Minimum valid version: 1.0.139

## Goal
Provide practical recovery steps when the dashboard does not behave as expected.

## Build and upload checks
- Build: `platformio run`
- Upload: `platformio run --target upload`
- Clean cache: `platformio run --target clean`

## Typical issues
### 1) Wi-Fi never connects
Check credentials in `include/secrets.h`, router availability, and signal quality.

### 2) Forecast page is empty
Check OpenWeather key, internet access, coordinates, and API quota.

### 3) OLED is blank
Check SDA/SCL wiring, OLED power, and SH1106 compatibility.

### 4) Sensors invalid or missing
Check AHT20/BMP280 wiring, I2C stability, and power quality.

### 5) Graphs show no values
History should reload automatically at boot. If not, clear history from the system menu and reboot.

### 6) Web UI not reachable
- Ensure same Wi-Fi network.
- Try direct IP if `meteohub.local` fails.
- Check startup logs for LittleFS mount errors.

### 7) Boot loop / startup freeze (watchdog)
Symptoms: repeated reboot, `Task watchdog got triggered`, or file open errors.

**Cause:** often filesystem inconsistency after abrupt power loss.

**Beginner-friendly emergency recovery:**
1. Unplug USB power.
2. Hold **BOOT**.
3. Plug USB power back in while still holding BOOT.
4. Wait for maintenance/format message, then release.
5. Device formats internal storage and reboots.

### 8) I2C errors (`i2cRead returned Error -1`) and spikes
Likely hardware/power issue:
- verify SDA/SCL/VCC/GND,
- test another USB cable/supply,
- if using SD module, prefer stable 3.3V supply and add decoupling capacitor.

## Maintenance actions available in UI menu
- Reboot
- Clear logs
- Clear history
- Format SD (if present)

## Safe update workflow
1. Save config/code changes.
2. Build firmware.
3. Upload and monitor startup logs.
4. Verify pages and status indicators.
5. Verify Web UI, logs, and history behavior.

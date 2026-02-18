# Maintenance and Troubleshooting

Minimum valid version: 1.0.103

## Goal
Provide practical recovery steps when the dashboard does not behave as expected.

## Build and upload checks
- Build: `platformio run`
- Upload: `platformio run --target upload`
- Clean build cache: `platformio run --target clean`

## OLED/LCD environments: auto-detection and navigation
The firmware automatically detects and adapts to either an SH1106 OLED (128x64, monochrome) or a TFT ST7789 LCD (240x240, color) at startup. Navigation and button mapping differ between environments (see user guide and hardware wiring).

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

### 7) I2C Errors (`i2cRead returned Error -1`) and Unexpected Reboots
**Symptoms**: The logs show `i2cRead returned Error -1` and/or `Bus already started in Master Mode` errors. The device may reboot unexpectedly, sometimes with a `Reason: 8 - ASSOC_LEAVE` message in the Wi-Fi logs.

**Causes and Solutions**:
- **`Bus already started...`**: This is a normal and harmless warning. It appears because multiple components are trying to initialize the I2C bus. It can be safely ignored.
- **`i2cRead returned Error -1`**: This is a hardware error. It means a sensor (AHT20 or BMP280) is not responding.
    - **Check the wiring** of the SDA, SCL, VCC, and GND pins for your sensors.
    - **Check the power supply**: A weak USB power supply or a poor-quality cable can cause voltage drops that lead to these errors. Try a different USB port, a different cable, or a dedicated wall adapter.
- **Unexpected Reboot (`ASSOC_LEAVE`)**: The reboot is often a consequence of the instability caused by the hardware issue above. An unstable power supply can disrupt both the sensors and the Wi-Fi module, leading to a disconnection and a reboot. **Solving the I2C hardware problem usually solves the rebooting issue.**

### 9) Outlier Sensor Values (Sudden Spikes or Drops)
**Symptoms**: Graphs show sudden, unrealistic spikes (e.g., temperature jumping to -140°C or +80°C) or humidity briefly hitting 0% or 100%.

**Probable Cause**: Electrical interference, often linked to adding a power-hungry module like an **SD card**.
- SD card writes generate current spikes.
- These spikes can cause brief voltage drops on the 3.3V rail.
- I2C sensors are sensitive to these fluctuations and may return corrupted data.

**Solutions**:
1. **3.3V Power**: If your SD module supports it, power it with 3.3V instead of 5V. This bypasses the module's internal regulator (often noisy) and matches the ESP32 logic levels.
2. **Capacitor**: Add a decoupling capacitor (e.g., 100µF) on the 3.3V supply near the sensors or the SD module.
3. **Software**: The firmware (v1.0.95+) includes a software filter to ignore these outlier values.

### 7) Boot Loops or System Freeze at Startup (Watchdog Triggered)
**Symptoms**: The device reboots in a loop, logs show `Task watchdog got triggered` and/or errors like `vfs_api.cpp:105] open(): ... does not exist, no permits for creation`. The web interface is unresponsive.

**Cause**: The filesystem (LittleFS) is most likely corrupted. This typically happens after a power loss or an abrupt USB disconnection while the device was writing data (history, logs).

**Recovery Solution**:
1.  Upload firmware version 1.0.76 or newer.
2.  Unplug the device.
3.  Plug it back in while **holding down the BOOT button (GPIO 0)**.
4.  The screen will display a maintenance message. Keep holding the button for 3 seconds until formatting begins.
5.  Release the button. The device will format the memory (which erases corrupted data, including history) and reboot cleanly.

## Safe update workflow
1. Save changes.
2. Build locally.
3. Upload and observe boot sequence.
4. Verify pages and LED statuses.
5. Check logs page for anomalies.

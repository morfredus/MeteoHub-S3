# Maintenance and Troubleshooting

Minimum valid version: 1.0.115

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
This behavior is no longer normal. The system reloads history at startup. If graphs remain empty, it may indicate a problem with the LittleFS file system. Try clearing the history via the system menu to force a reset.

### 6) The Web UI is inaccessible
Check:
- That you are on the same Wi-Fi network.
- Try the direct IP address (e.g., `192.168.1.x`) instead of `meteohub.local` (mDNS can be tricky on some networks/OSes).
- Check that the LittleFS partition is mounted correctly (see logs at startup).

## Maintenance actions available in UI menu
- Reboot
- Clear logs
- Clear history
- Format SD (if present)

### 7) Boot Loops or System Freeze at Startup (Watchdog Triggered)
**Symptoms**: The device reboots in a loop, logs show `Task watchdog got triggered` and/or errors like `vfs_api.cpp:105] open(): ... does not exist, no permits for creation`. The web interface is unresponsive.


> **Tip for beginners:** If your MeteoHub S3 is stuck or keeps rebooting, you can force an emergency format of the internal memory (LittleFS) without a computer, simply by holding the BOOT button at startup.

**Step-by-step procedure:**
1. Unplug the device from USB power.
2. Press and hold the **BOOT** button (usually labeled "BOOT" or connected to GPIO 0).
3. While holding BOOT, plug the USB power back in.
This recovery mode is designed to be accessible to any user, even beginners, and requires no software or external tools.

### 8) I2C Errors (`i2cRead returned Error -1`) and Unexpected Reboots
- **`i2cRead returned Error -1`**: This is a hardware error. It means a sensor (AHT20 or BMP280) is not responding.
    - **Check the wiring** of the SDA, SCL, VCC, and GND pins for your sensors.
    - **Check the power supply**: A weak USB power supply or a poor-quality cable can cause voltage drops that lead to these errors. Try a different USB port, a different cable, or a dedicated wall adapter.
**Symptoms**: Graphs show sudden, unrealistic spikes (e.g., temperature jumping to -140°C or +80°C) or humidity briefly hitting 0% or 100%.

**Probable Cause**: Electrical interference, often linked to adding a power-hungry module like an **SD card**.

**Solutions**:
1. **3.3V Power**: If your SD module supports it, power it with 3.3V instead of 5V. This bypasses the module's internal regulator (often noisy) and matches the ESP32 logic levels.
2. **Capacitor**: Add a decoupling capacitor (e.g., 100µF) on the 3.3V supply near the sensors or the SD module.
3. **Software**: The firmware (v1.0.95+) includes a software filter to ignore these outlier values.

=======

**Cause and Solution (Fixed in v1.114+)**: This issue was caused by a blocking operation in the web server that monopolized the CPU when handling large files or data streams (like history graphs). This triggered a safety watchdog timeout.

<<<<<<< HEAD
As of version 1.114, the web server uses a fully non-blocking streaming method that yields CPU time during long operations. This definitively resolves the issue. If you are experiencing this bug, please update your firmware to v1.114 or newer.
=======
**Beginner-friendly recovery (emergency format):**

> **Tip for beginners:** If your MeteoHub S3 is stuck or keeps rebooting, you can force an emergency format of the internal memory (LittleFS) without a computer, simply by holding the BOOT button at startup.

**Step-by-step procedure:**
1. Unplug the device from USB power.
2. Press and hold the **BOOT** button (usually labeled "BOOT" or connected to GPIO 0).
3. While holding BOOT, plug the USB power back in.
4. Keep holding BOOT for about 3 seconds. The screen will show a maintenance message and indicate that formatting will start.
5. Release the BOOT button when the formatting message appears. The device will erase the internal memory (all history data will be lost) and reboot automatically.

This recovery mode is designed to be accessible to any user, even beginners, and does not require any software or special tools.

**Recovery Solution**:
1.  Upload firmware version 1.0.76 or newer.
2.  Unplug the device.
3.  Plug it back in while **holding down the BOOT button (GPIO 0)**.
4.  The screen will display a maintenance message. Keep holding the button for 3 seconds until formatting begins.
## Safe update workflow
1. Save changes.
3. Upload and observe boot sequence.
4. Verify pages and LED statuses.
5. Check logs page for anomalies.
>>>>>>> main

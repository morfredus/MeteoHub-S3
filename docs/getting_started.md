# Getting Started

Minimum valid version: 1.0.103

## Goal
This guide helps a beginner start MeteoHub S3 from scratch up to the first successful upload.

## Supported Environments
- **OLED (SH1106, I2C)**: Monochrome, 128x64, rotary encoder + 2 buttons, NeoPixel, AHT20/BMP280 sensors.
- **LCD (TFT ST7789, SPI)**: Color, 240x240 or 240x320, rotary encoder (HW-040) + 3 buttons, AHT20/BMP280 sensors.

The firmware automatically detects the display type at startup and adapts the interface and navigation. See Hardware Wiring for pin details.

## Prerequisites
- A Windows PC
- Visual Studio Code
- The PlatformIO extension in VS Code
- An ESP32-S3-DevKitC-1-N16R8V board
- A data-compatible USB cable

## 1) Open the Project
1. Open VS Code.
2. Open the folder: `Dashboard-IoT`.
3. Wait for PlatformIO to index the project.

## 2) Prepare the Secrets File
1. Create `include/secrets.h` by copying `include/secrets_example.h`.
2. Fill in Wi-Fi credentials (`WIFI_CREDENTIALS`).
3. Fill in OpenWeatherMap API key (`OWM_API_KEY`).
4. Verify coordinates (`OWM_LAT`, `OWM_LON`) and units (`OWM_UNITS`).
5. (Optional) For LCD, adjust `LCD_WIDTH` and `LCD_HEIGHT` in `include/config.h` if using a 240x320 screen.

## Secrets Security Rule (Mandatory)
- `include/secrets.h` is a private file.
- Never push `include/secrets.h` to GitHub.
- Never share `include/secrets.h` (chat, screenshot, email, repository archive).

## 3) Compile the Firmware
Run in terminal:
- `platformio run`

Expected result:
- Compilation ends with `[SUCCESS]`.

## 4) Upload to the Board
Run:
- `platformio run --target upload`

Expected result:
- Firmware is flashed without upload error.

## 5) Verify Behavior
After reboot, the OLED screen should display the boot phases:
- Splash Screen "morfredus"
- Sensor Initialization
- Wi-Fi Connection
- Time Synchronization
- System Ready

## Common First-Boot Issues
- **No Wi-Fi**: Check SSID/password in `include/secrets.h`.
- **No Forecast**: Check `OWM_API_KEY` and internet connection.
- **No Display**: Check SDA/SCL wiring and OLED power.
- **Upload Failed**: Check USB cable and selected COM port.

## Next Step
Continue with Hardware Wiring and Configuration.
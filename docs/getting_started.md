# Getting Started

Minimum valid version: 1.0.22

## Goal
This guide helps a beginner run MeteoHub S3 from zero to first successful upload.

## Prerequisites
- A Windows PC
- Visual Studio Code
- PlatformIO extension in VS Code
- ESP32-S3-DevKitC-1-N16R8V board
- USB cable with data support

## 1) Open the project
1. Open VS Code.
2. Open folder: `Dashboard-IoT`.
3. Wait for PlatformIO to index the project.

## 2) Prepare secrets file
1. Create `include/secrets.h` by copying `include/secrets_example.h`.
2. Fill Wi-Fi credentials (`WIFI_CREDENTIALS`).
3. Fill OpenWeatherMap API key (`OWM_API_KEY`).
4. Keep your coordinates (`OWM_LAT`, `OWM_LON`) and units (`OWM_UNITS`).

## Secrets security rule (mandatory)
- `include/secrets.h` is a private file.
- Never push `include/secrets.h` to GitHub.
- Never share `include/secrets.h` (chat, screenshot, email, or repository archive).

## 3) Build the firmware
Run in terminal:
- `platformio run`

Expected result:
- Build completes with `[SUCCESS]`.

## 4) Upload to board
Run:
- `platformio run --target upload`

Expected result:
- Firmware is flashed without upload errors.

## 5) Verify runtime behavior
After reset, you should see boot phases on OLED:
- Booting
- Sensor initialization
- Wi-Fi connection
- Time synchronization
- System ready

## Common first-run issues
- **No Wi-Fi**: check SSID/password in `include/secrets.h`.
- **No forecast**: check `OWM_API_KEY` and internet connectivity.
- **No display**: check SDA/SCL wiring and OLED power.
- **Upload fails**: check USB cable quality and selected COM port.

## Next step
Continue with [Hardware Wiring](hardware_wiring.md) and [Configuration](configuration.md).

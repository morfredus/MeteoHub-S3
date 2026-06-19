# MeteoHub S3

> **Minimum valid version: 1.1.3**

## Full Documentation
- [Documentation Index](docs/index.md)

## Beginner Documentation
- [Beginner's Guide (English)](docs/beginner/index.md)
- [Guide Débutant (Français)](docs/beginner/index_fr.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project focused on an OLED dashboard (SH1106/SSD1306 via U8g2). It displays local sensor values, weather forecast data, logs, and system status.

## Required Hardware
- OLED display (SH1106 or SSD1306, I2C)
- HW-040 rotary encoder module (encoder + center button)
- Back and Confirm buttons
- AHT20 and BMP280 sensors
- Optional SD card for long-term history (Recommended: FAT32 format, 4-32 GB)

## Build
- Install PlatformIO in VS Code
- Select environment: `esp32-s3-oled`
- Build: `platformio run`
- Upload: `platformio run --target upload`

## Main features (New in v1.1.x)
- **Enhanced SD Stability (v1.1.2)**: Secure writes with explicit `flush()` and Mutex protection against file corruption.
- **C++ Standard Engine (v1.1.0)**: Complete web core refactor to use `std::string`, ensuring better memory stability.
- **Advanced File Management**: Upload, download, and delete files via web interface.
- **Robust OTA Updates**: Secure Over-The-Air update system.
- **Advanced Graphing**: Three scale modes (Fixed, Dynamic, Mixed) for temperature, humidity, and pressure.

## Usage
See docs/user_guide.md for details on scale modes and file management.

### SD Card Note
To prevent corruption, ensure that:
1. The card is formatted to **FAT32** (allocation unit size 32 KB).
2. The system is properly shut down before removing the card (although `flush()` protects data on every write).

---

Any change in config.h is injected into the web UI at build time.
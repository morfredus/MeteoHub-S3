# MeteoHub S3

> **Minimum valid version: 1.0.174**

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
- Optional SD card for long-term history

## Build
- Install PlatformIO in VS Code
- Select environment: `esp32-s3-oled`
- Build: `platformio run`
- Upload: `platformio run --target upload`

## Main features
- Temperature, humidity, pressure graph display
- Three scale modes for graphs:
	1. Fixed: bounds defined in configuration (config.h), ideal for multi-day comparison.
	2. Dynamic: bounds calculated from displayed data, highlights recent variations.
	3. Mixed: combines both, expands dynamic scale by a percentage (configurable), but stays within fixed bounds.
- Control scale mode and expansion percentage on web UI
- Automatic parameter synchronization between config.h and web UI
- Contextual help below the graph (mode explanations)
- Robust SD management for ESP32-S3 3.3V modules: adaptive SPI frequency, automatic remount, health checks, and safe history writes

## Usage

See docs/user_guide.md for details on scale modes.

---

Any change in config.h is injected into the web UI at build time.



# MeteoHub S3

Minimum valid version: 1.0.120

## Full Documentation
- [Documentation Index](docs/index.md)

## Overview
MeteoHub S3 is an ESP32-S3 PlatformIO project that displays local sensor data, weather forecast information, logs, and system status on a color LCD (TFT ST7789) or SH1106 OLED dashboard. The firmware auto-detects the display type and adapts the UI and navigation accordingly.


## Required Hardware and Differences

- **OLED (SH1106, I2C)**:
	- HW-040 complete module: PCB with integrated OLED, rotary encoder (center button), and Back/Confirm buttons soldered directly.
	- Navigation: 1 detent = 1 page.
	- Integrated NeoPixel.
	- AHT20/BMP280 sensors.

- **LCD (TFT ST7789, SPI)**:
	- EC11 module only: rotary encoder (no Back/Confirm buttons on PCB, must be wired separately).
	- Accepted definitions: 240x240 or 240x320 (configurable in `include/config.h`).
	- Navigation: 2 detents = 1 page.
	- AHT20/BMP280 sensors.

Both environments are auto-detected at startup and the interface adapts accordingly.

## Project Architecture
- `src/modules/`: hardware/display/page modules (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `st7789_display`, `pages`)
- `src/managers/`: orchestration and state managers (`wifi_manager`, `ui_manager`, `history`, `sd_manager`, `web_manager`)
- `src/utils/`: reusable utility code (`logs`, `system_info`)
- `include/`: reserved configuration headers only (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Build
- Install PlatformIO in VS Code.
- Select environment `esp32-s3-lcd` (TFT) or `esp32-s3-oled` (OLED) as needed.
- Build command: `platformio run`
- Upload command: `platformio run --target upload`

## Key Features (v1.0.104)

- **Advanced SD Card Management**
	- SD card is now the primary storage for long-term history (daily CSV files)
	- File manager in the Web UI allows browsing, downloading, uploading, and deleting files on both SD and internal storage
	- **SD card formatting** directly from the device menu (with confirmation and visual feedback)
	- Automatic creation and maintenance of `/history` folder on SD

- **Robust Data Backup & History**
	- 24h circular RAM buffer, persisted to LittleFS (internal flash) for fast recovery
	- Automatic archiving to SD card for long-term storage
	- Outlier filtering for sensor values before saving
	- Selective clearing of logs and history (with confirmation)

- **Enhanced Web Interface**
	- Real-time dashboard, interactive graphs, statistics, and long-term history
	- System logs page with auto-refresh and color highlighting (Info/Warn/Error)
	- Unified footer with direct access to file manager and logs
	- Multi-storage file manager (LittleFS/SD)

- **User Experience & Safety**
	- Unified navigation and button logic for OLED and LCD
	- Confirmation screens for critical actions (format SD, clear logs/history)
	- Visual feedback for all maintenance operations

- **Documentation**
	- All user-facing documentation (EN/FR) is strictly synchronized with the code and features
	- Minimum valid version is always indicated

## LCD vs OLED: Key Differences
- **Navigation**: On OLED, one detent = one page (HW-040 module, buttons + OLED integrated); on LCD, 2 detents = one page (EC11 module only).
- **Display**: LCD is color, higher resolution, and supports more advanced layouts (axes, graphs, etc.).
- **Wiring**: See [Hardware Wiring](docs/hardware_wiring.md) for exact pinout per environment.

For detailed setup, usage, and troubleshooting, see the user documentation in the `docs/` folder.

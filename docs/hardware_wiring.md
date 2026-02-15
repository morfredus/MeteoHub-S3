# Hardware Wiring

Minimum valid version: 1.0.22

## Overview
This page explains the exact GPIO mapping used by the firmware.

## Pin mapping source
The authoritative mapping is defined in:
- `include/board_config.h`

## Current GPIO map
### OLED (I2C SH1106)
- `OLED_SDA` = GPIO 15
- `OLED_SCL` = GPIO 16

### Rotary encoder (EC11)
- `ENC_A` = GPIO 4
- `ENC_B` = GPIO 5
- `ENC_BTN` = GPIO 6

### Dedicated buttons
- `BTN_BACK` = GPIO 7
- `BTN_CONFIRM` = GPIO 8

### Status LED
- `NEOPIXEL_PIN` = GPIO 48

## Electrical notes
- Connect board `3V3` and `GND` correctly.
- OLED and sensors must share common ground.
- Use short and stable I2C wires to reduce communication errors.

## Supported sensors in this project
- AHT20 (temperature/humidity)
- BMP280 (pressure + optional fallback temperature)

## Validation checklist
- OLED displays boot screen
- Encoder rotates and clicks
- Back/Confirm buttons react
- NeoPixel shows status colors
- Sensor values appear in weather page

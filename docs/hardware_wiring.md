# Hardware Wiring

Minimum valid version: 1.0.154

## Overview
This page explains the exact GPIO mapping used by the firmware.


## OLED/LCD environments
The firmware supports both SH1106 OLED and ST7789 LCD. Pin mapping and button layout differ between environments. See below for details. The correct mapping is always defined in:
- `include/board_config.h`


## Current GPIO map

### OLED environment (I2C SH1106)
- `I2C_SDA` = GPIO 15
- `I2C_SCL` = GPIO 16
- Rotary encoder (EC11):
	- `ENC_A` = GPIO 4
	- `ENC_B` = GPIO 5
	- `ENC_BTN` = GPIO 6
- Dedicated buttons:
	- `BTN_BACK` = GPIO 7
	- `BTN_CONFIRM` = GPIO 8
- Status LED:
	- `NEOPIXEL_PIN` = GPIO 48

### LCD environment (TFT ST7789)
- Display:
	- `DISPLAY_CS_PIN` = GPIO 13
	- `DISPLAY_MOSI_PIN` = GPIO 11
	- `DISPLAY_SCK_PIN` = GPIO 12
	- `DISPLAY_DC_PIN` = GPIO 10
	- `DISPLAY_RST_PIN` = GPIO 9
	- `DISPLAY_BL_PIN` = GPIO 8
	- `DISPLAY_MISO_PIN` = -1 (not used)
- Rotary encoder (HW-040):
	- `ROTARY_CLK` = GPIO 47
	- `ROTARY_DT` = GPIO 45
	- `ROTARY_SW` = GPIO 40
- Buttons:
	- `BUTTON_BOOT` = GPIO 0
	- `BUTTON_1` = GPIO 38
	- `BUTTON_2` = GPIO 39
- I2C sensors:
	- `I2C_SDA` = GPIO 15
	- `I2C_SCL` = GPIO 16

## Electrical notes
- Connect board `3V3` and `GND` correctly.
- OLED and sensors must share common ground.
- Use short and stable I2C wires to reduce communication errors.

## Supported hardware
- OLED environment: SH1106 OLED display, EC11 rotary encoder, 2 push buttons, NeoPixel LED, AHT20, BMP280
- LCD environment: TFT ST7789 display, HW-040 rotary encoder, 3 push buttons, AHT20, BMP280

## Validation checklist
- OLED: boot screen, encoder, buttons, NeoPixel, sensor values
- LCD: TFT display, encoder, buttons, sensor values

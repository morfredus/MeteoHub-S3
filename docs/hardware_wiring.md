# Hardware Wiring


Minimum valid version: 1.1.3

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings


Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional SD card (SPI)

## Pin Mapping (from board_config.h)

| Function                | GPIO | Notes                                 |
|-------------------------|------|---------------------------------------|
| I2C SDA                 | 15   | OLED, AHT20, BMP280                   |
| I2C SCL                 | 16   | OLED, AHT20, BMP280                   |
| Button BOOT             | 0    |                                       |
| Button BACK             | 7    |                                       |
| Button CONFIRM          | 8    |                                       |
| Neopixel                | 48   | Onboard LED                           |
| Encoder A               | 4    | EC11/HW-040                           |
| Encoder B               | 5    | EC11/HW-040                           |
| Encoder Button          | 6    | EC11/HW-040                           |
| SD CLK (SCK)            | 9    | SPI SD                                |
| SD MISO (DAT0/SO)       | 10   | SPI SD                                |
| SD MOSI (CMD/SI)        | 11   | SPI SD                                |
| SD CS (DAT3/CS)         | 12   | SPI SD                                |
| SD DAT2                 | 13   | SPI SD (not always used)              |
| SD DETECT               | 14   | LOW=present, HIGH=not present (configurable polarity) |

> See board_config.h for authoritative mapping and SD_DET_ACTIVE_LEVEL for polarity.

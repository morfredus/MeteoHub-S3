# Hardware Wiring

Minimum valid version: 1.0.178

This project targets an OLED setup only.

Main references:
- `include/board_config.h` for pin mapping
- `include/config.h` for runtime display settings

Connected peripherals:
- OLED (I2C)
- Rotary encoder + buttons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Optional SD card (3.3V module, SPI + SDIO compatible)

## SD Card Module (3.3V, SPI + SDIO compatible)

The SD manager supports configurable bus mode via `SD_BUS_MODE` in `board_config.h`:
- `0`: AUTO (SPI first, then SDIO)
- `1`: SPI only
- `2`: SDIO only

Current default for this board is `SPI only` to avoid unnecessary SDIO retries on SPI-wired modules.

| Module pin | GPIO | Description             |
|------------|------|-------------------------|
| CLK        | 12   | Clock                   |
| CMD / S1   | 11   | SDIO CMD / SPI MOSI (DI) |
| D0 / SO    | 13   | SDIO DAT0 / SPI MISO (DO) |
| D1         | 14   | SDIO DAT1               |
| DAT2       | 10   | SDIO DAT2               |
| D3 / CS    | 9    | SDIO DAT3 / SPI CS      |
| VCC        | 3.3V | Power supply            |
| GND        | GND  | Ground                  |

> **Note:** The same GPIO mapping is reused for both buses.
> In SPI mode, D3/CS is the chip-select pin.

### Compatibility and robustness

The driver attempts mounting in the following order:
1. SPI at 1 MHz
2. SPI at 4 MHz
3. SPI at 10 MHz
4. SPI at 20 MHz
5. SDIO 4-bit at 400 kHz
6. SDIO 4-bit at 1 MHz
7. SDIO 4-bit at 4 MHz
8. SDIO 4-bit at 20 MHz
9. SDIO 4-bit at 40 MHz
10. SDIO 1-bit fallback at 400 kHz
11. SDIO 1-bit fallback at 1 MHz
12. SDIO 1-bit fallback at 4 MHz

This strategy keeps broad compatibility with SPI-labeled SD modules while still benefiting from SDIO when all data lines are wired.

## Other peripherals

| Peripheral   | Pin(s)                        |
|-------------|-------------------------------|
| I2C SDA     | GPIO 15                       |
| I2C SCL     | GPIO 16                       |
| NeoPixel    | GPIO 48                       |
| Encoder A   | GPIO 4                        |
| Encoder B   | GPIO 5                        |
| Encoder btn | GPIO 6                        |
| Button BACK | GPIO 7                        |
| Button OK   | GPIO 8                        |

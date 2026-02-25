# Configuration

Minimum valid version: 1.0.170


## Editable files
- `include/secrets.h` (credentials, API key, coordinates)
- `include/config.h` (all user-modifiable parameters, see below)

## User parameters in config.h

### Graph parameters
- `GRAPH_SCALE_MODE` : Scale mode for graphs (0=fixed, 1=dynamic, 2=mixed)
- `GRAPH_SCALE_MARGIN_PCT` : Expansion percentage for mixed mode
- `GRAPH_TEMP_MIN` / `GRAPH_TEMP_MAX` : Min/max temperature for graphs
- `GRAPH_HUM_MIN` / `GRAPH_HUM_MAX` : Min/max humidity for graphs
- `GRAPH_PRES_MIN` / `GRAPH_PRES_MAX` : Min/max pressure for graphs

### OLED parameters
- `OLED_CONTRAST` : OLED display contrast
- `OLED_CONTROLLER` : OLED controller type (SH1106/SSD1306)
- `OLED_I2C_ADDRESS` : OLED I2C address

### Network parameters
- `WEB_MDNS_HOSTNAME` : mDNS hostname for local access
- `WIFI_RETRY_DELAY_MS` : WiFi retry delay (ms)
- `ENABLE_PING_TEST` : Enable ping test (1=enabled)

### System parameters
- `DASHBOARD_REFRESH_MS` : Dashboard refresh rate (ms)
- `BUTTON_GUARD_MS` : Debounce time for buttons (ms)

---
Reserved file:
- `include/board_config.h` (hardware pin mapping)

Metadata:
Project name/version are injected from `platformio.ini` with build flags.

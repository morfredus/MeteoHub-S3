# User Guide

**Beginner?** See [Beginner's Guide](../beginner/index.md) or [Guide Débutant](../beginner/index_fr.md)

Minimum valid version: 1.1.3

- Rotate encoder to navigate pages.
- Encoder click opens the menu.
- Back exits menu/confirmation.
- Confirm validates contextual actions.


Core pages:
- Weather
- Forecast
- Graphs (temperature, humidity, pressure)
- Network
- System
- Logs

---

## Advanced features and web API

### Web interface
- Access the web UI via http://<name>.local (mDNS) or local IP.
- Real-time data, history, graphs, logs, file management, and OTA update.

### File management (LittleFS/SD)
- Browse, download, delete, upload files via the web UI (Files tab).
- SD card files accessible if a card is inserted and detected.
- API: `/api/files/list`, `/api/files/download`, `/api/files/delete`, `/api/files/upload` (parameter `fs=sd` or `fs=littlefs`).

### History and statistics
- Graphical view of history (temperature, humidity, pressure).
- API: `/api/history` (parameters `window`, `interval`, `points` for aggregation).
- 24h statistics via `/api/stats` (min, max, average, trends).

### Logs and diagnostics
- System logs via the web UI (Logs tab) and API `/api/logs`.
- System diagnostics via `/api/system` (hardware info, firmware, SD, uptime, etc.).

### OTA (firmware update)
- Firmware update via the web UI (OTA tab).
- Upload the binary file, automatic reboot after success.
- API: `/api/ota/update`.

### Weather alerts and trends
- Weather alerts (level, type, description, color) on the web UI and OLED.
- Automatic alert translation (English → French).
- Weather trend summary (improving, rain, disturbance, etc.).

### Advanced maintenance
- Long press BOOT button at startup: formats internal filesystem (LittleFS) and reboots.

For more details on each API, see the technical documentation or the source code (web_manager.cpp).

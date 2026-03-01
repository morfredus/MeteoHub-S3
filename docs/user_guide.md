# User Guide

**Beginner?** See [Beginner's Guide](../beginner/index.md) or [Guide Débutant](../beginner/index_fr.md)

Minimum valid version: 1.0.174

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


Storage strategy:
- LittleFS is used as a rolling local cache (24-72h), not as an infinite archive.
- Use an external collector polling `/api/history?window=...&interval=...` every X minutes.
- Store long-term history in InfluxDB (recommended), then visualize with Grafana or Home Assistant.

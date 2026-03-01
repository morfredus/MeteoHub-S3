# [1.0.174] – 2026-03-01
- Fixed OLED freezes and display glitches when rotating the encoder by removing periodic LittleFS cache rewrites from the hot path.
- History cache now keeps a lighter 48h rolling window (`MAX_RECENT_RECORDS=2880`) to reduce runtime load while keeping short-term resilience.
- LittleFS compaction is now performed only at startup when an oversized cache file is detected.
- Minimum valid version: 1.0.174

# [1.0.173] – 2026-03-01
- Added an external collector script (`scripts/history_collector.py`) that polls `/api/history?window=...&interval=...` and pushes data to InfluxDB.
- Updated history cache policy: LittleFS retained as a bounded 72h rolling cache (not an infinite archive).
- Added EN/FR documentation for external collection, retention strategy, and dashboard integration (Grafana/Home Assistant).
- Minimum valid version: 1.0.173

# [1.0.172] – 2026-02-25
- Added and cross-linked beginner documentation (EN/FR) in all user-facing docs.
- All guides, FAQ, configuration, and index now reference beginner onboarding.
- Minimum valid version: 1.0.172

# [1.0.171] – 2026-02-25
- Added advanced scale management for temperature, humidity and pressure graphs.
- Three modes: fixed, dynamic, mixed (with configurable expansion).
- Interactive controls on web UI to select mode and percentage.
- Contextual help below the graph.
- Automatic synchronization between config.h and web UI.

# [1.0.170] - 2026-02-24
### Fixed
- Applied the graph safe-zone layout scheme to other OLED pages.
- Kept page titles in the top band and moved forecast/logs content start below SSD1306 reserved top area.
- Aligned log line spacing to avoid top-content overlap in yellow-band SSD1306 variants.

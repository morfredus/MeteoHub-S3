# External History Collector

Minimum valid version: 1.0.174

## Goal
Use MeteoHub S3 as a short-term edge cache and push time-series data to a long-term backend.

- Keep **LittleFS** as a local rolling cache only (24 to 72 hours).
- Poll `GET /api/history?window=...&interval=...` from an external collector every X minutes.
- Store data on a server (recommended: **InfluxDB**) and build dashboards with **Grafana** or **Home Assistant**.

## Recommended architecture
1. MeteoHub S3 exposes `/api/history`.
2. A Python collector (running on NAS/Raspberry Pi/server) polls the endpoint.
3. Collector writes points to InfluxDB.
4. Grafana/Home Assistant reads from InfluxDB for long-term views.

## Collector script
A ready-to-run collector is available in `scripts/history_collector.py`.

Example run:

```bash
python3 scripts/history_collector.py \
  --meteo-url http://meteohub-s3.local \
  --window 3600 \
  --interval 60 \
  --period-min 5 \
  --influx-url http://127.0.0.1:8086 \
  --influx-org my_org \
  --influx-bucket meteo \
  --influx-token <TOKEN>
```

## Retention strategy
- **Device (LittleFS):** 24–72h rolling cache for resiliency and quick reboot recovery.
- **Server (InfluxDB):** long retention (months/years).
- **Dashboards:** use InfluxDB as the source of truth for history analytics.

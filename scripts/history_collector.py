#!/usr/bin/env python3
"""External collector for MeteoHub S3 history API.

Polls `/api/history?window=...&interval=...` every X minutes and writes points to InfluxDB.
"""

import argparse
import json
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from datetime import datetime, timezone


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Collect MeteoHub history and push to InfluxDB")
    parser.add_argument("--meteo-url", required=True, help="Base URL of MeteoHub, e.g. http://meteohub-s3.local")
    parser.add_argument("--window", type=int, default=3600, help="History window in seconds")
    parser.add_argument("--interval", type=int, default=60, help="Aggregation interval in seconds")
    parser.add_argument("--period-min", type=int, default=5, help="Polling period in minutes")
    parser.add_argument("--measurement", default="meteo_history", help="InfluxDB measurement name")
    parser.add_argument("--device", default="meteohub_s3", help="InfluxDB device tag")
    parser.add_argument("--influx-url", required=True, help="InfluxDB base URL, e.g. http://localhost:8086")
    parser.add_argument("--influx-org", required=True, help="InfluxDB organization")
    parser.add_argument("--influx-bucket", required=True, help="InfluxDB bucket")
    parser.add_argument("--influx-token", required=True, help="InfluxDB API token")
    parser.add_argument("--timeout", type=int, default=15, help="HTTP timeout in seconds")
    return parser.parse_args()


def fetch_history(base_url: str, window: int, interval: int, timeout: int) -> list[dict]:
    params = urllib.parse.urlencode({"window": window, "interval": interval})
    url = f"{base_url.rstrip('/')}/api/history?{params}"
    req = urllib.request.Request(url, headers={"Accept": "application/json"})
    with urllib.request.urlopen(req, timeout=timeout) as response:
        payload = json.loads(response.read().decode("utf-8"))
    return payload.get("data", [])


def build_line_protocol(measurement: str, device: str, records: list[dict]) -> str:
    lines = []
    for row in records:
        ts_s = int(row["t"])
        ts_ns = ts_s * 1_000_000_000
        temp = float(row["temp"])
        hum = float(row["hum"])
        pres = float(row["pres"])
        lines.append(
            f"{measurement},device={device} temp={temp},hum={hum},pres={pres} {ts_ns}"
        )
    return "\n".join(lines)


def push_influx(
    influx_url: str,
    org: str,
    bucket: str,
    token: str,
    line_protocol: str,
    timeout: int,
) -> None:
    query = urllib.parse.urlencode({"org": org, "bucket": bucket, "precision": "ns"})
    url = f"{influx_url.rstrip('/')}/api/v2/write?{query}"
    data = line_protocol.encode("utf-8")
    req = urllib.request.Request(
        url,
        data=data,
        method="POST",
        headers={
            "Authorization": f"Token {token}",
            "Content-Type": "text/plain; charset=utf-8",
            "Accept": "application/json",
        },
    )
    with urllib.request.urlopen(req, timeout=timeout) as response:
        if response.status not in (204, 200):
            raise RuntimeError(f"InfluxDB write failed: HTTP {response.status}")


def main() -> int:
    args = parse_args()
    period_s = max(60, args.period_min * 60)
    print(
        f"[{datetime.now(timezone.utc).isoformat()}] Collector started: period={args.period_min} min, "
        f"window={args.window}s, interval={args.interval}s"
    )

    while True:
        try:
            records = fetch_history(args.meteo_url, args.window, args.interval, args.timeout)
            if not records:
                print(f"[{datetime.now(timezone.utc).isoformat()}] No history points returned")
            else:
                payload = build_line_protocol(args.measurement, args.device, records)
                push_influx(
                    args.influx_url,
                    args.influx_org,
                    args.influx_bucket,
                    args.influx_token,
                    payload,
                    args.timeout,
                )
                print(
                    f"[{datetime.now(timezone.utc).isoformat()}] Sent {len(records)} points to InfluxDB"
                )
        except (urllib.error.URLError, urllib.error.HTTPError, ValueError, RuntimeError) as exc:
            print(f"[{datetime.now(timezone.utc).isoformat()}] Collector error: {exc}", file=sys.stderr)

        time.sleep(period_s)


if __name__ == "__main__":
    raise SystemExit(main())

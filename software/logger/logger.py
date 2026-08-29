import time
import csv
import sys
import os
from pathlib import Path

import serial
from serial.tools import list_ports

BAUD = 115200
OUT = Path.cwd() / time.strftime("kart_log_%Y%m%d_%H%M%S.csv")


def pick_port():
    ports = list(list_ports.comports())
    if not ports:
        print("No serial ports found. Plug in the bridge ESP32 and try again.")
        sys.exit(1)

    print("Available ports:")
    for i, p in enumerate(ports):
        print(f"  [{i}] {p.device}  {p.description}")

    try:
        choice = int(input(f"Select port [0-{len(ports)-1}] (Enter=0): ") or "0")
    except Exception:
        choice = 0

    return ports[choice].device


def ensure_header(file_path: Path):
    if not file_path.exists() or file_path.stat().st_size == 0:
        with open(file_path, "w", newline="", encoding="utf-8") as f:
            w = csv.writer(f)
            w.writerow([
                "wallclock_s",
                "esp_ms",
                "type",
                "a",
                "b",
                "c",
                "payload",
            ])


def main():
    port = pick_port()
    print(f"Logging to: {OUT}")
    ensure_header(OUT)

    ser = serial.Serial(port, BAUD, timeout=1)
    print("Listening on", port, "@", BAUD)

    rows = 0

    with open(OUT, "a", newline="", encoding="utf-8") as f:
        w = csv.writer(f)

        while True:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if not line:
                continue

            print(line)
            parts = line.split(",")
            now = time.time()

            if len(parts) >= 6 and parts[0] == "F":
                # F,<t_ms>,<id>,<ext>,<dlc>,<hex_bytes>
                t_ms = parts[1]
                can_id = parts[2]
                ext = parts[3]
                dlc = parts[4]
                data_hex = ",".join(parts[5:])
                w.writerow([now, t_ms, "FRAME", can_id, ext, dlc, data_hex])

            elif len(parts) >= 4 and parts[0] == "T":
                # T,<t_ms>,<key>,<value>
                t_ms = parts[1]
                key = parts[2]
                val = parts[3]
                w.writerow([now, t_ms, key, val, "", "", ""])

            else:
                w.writerow([now, "", "RAW", "", "", "", line])

            f.flush()
            os.fsync(f.fileno())
            rows += 1

            if rows % 25 == 0:
                print(f"[wrote {rows} rows → {OUT}]")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nStopped.")

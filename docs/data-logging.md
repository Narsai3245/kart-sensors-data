# Data Logging

## Python / pyserial

The bridge ESP32 presents the telemetry stream to the computer as a USB serial device. Python reads it with `pyserial` and stores the received records in CSV format.

The logger should also print received lines to the console during development so the team can verify the live data stream while logging.

## CSV records

The logger supports the current raw CAN (`F`) and decoded telemetry (`T`) formats and can preserve other received lines as raw records.

CSV files are intentionally ignored by Git because test runs can generate large amounts of data. Store important example datasets separately if they need to be committed later.

## Testing goal

During kart testing, the intended workflow is:

`CAN → ESP32 → ESP-NOW → bridge ESP32 → USB serial → Python → CSV`

The CSV becomes the persistent record for later analysis, while the live console/display provides immediate feedback.

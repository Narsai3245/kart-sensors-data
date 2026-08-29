# Kart Sensors & Data

Data acquisition system for the electric kart.

## Current architecture

```text
BMS / Sevcon
     │
     │ CAN
     ▼
SN65HVD230
     │
     ▼
ESP32 Sender
     │
     │ ESP-NOW
     ▼
ESP32 Bridge
     │
     │ USB Serial
     ▼
Laptop / Raspberry Pi
     │
     │ pyserial
     ▼
Python Data Logger
     │
     ▼
CSV Log
```

## Current proof of concept

- ESP32 successfully receives CAN frames from the BMS using TWAI.
- BMS Pack SOC was decoded from CAN ID `0x001`, byte 0, using `0.5 %/LSB`.
- ESP-NOW successfully transfers telemetry from the kart ESP32 to a bridge ESP32.
- The bridge forwards received data over USB serial.
- Python/pyserial receives the serial stream and writes a CSV data log.
- ESP-NOW heartbeat packets (`PING,<millis>`) have been verified end-to-end.

## Current issue

The ESP-NOW and Python logging path is working, but the sender is currently seeing no CAN frames in the end-to-end test. CAN bitrate, transceiver wiring, bus termination, and CAN-bus activity are the current troubleshooting targets.

## Future work

1. Reliably acquire all required BMS CAN frames.
2. Investigate and decode the Sevcon Gen4 CANopen interface.
3. Identify useful motor-controller PDO/SDO data.
4. Add additional BMS and motor-controller signals.
5. Build live telemetry visualization.
6. Develop the Raspberry Pi data collection/display system.
7. Add robust logging for kart testing.

## Hardware

- ESP32 CAN node
- SN65HVD230 3.3 V CAN transceiver
- ESP32 ESP-NOW bridge
- Laptop/Raspberry Pi for logging

## Wireless bridge

The current bridge ESP32 STA MAC address is:

`38:18:2B:B2:21:60`

This address is configured as the ESP-NOW peer on the sender.

## Data formats

### Heartbeat

```text
PING,<esp32_millis>
```

### Raw CAN frame

```text
F,<esp32_millis>,<id>,<ext>,<dlc>,<hex bytes>
```

### Decoded telemetry

```text
T,<esp32_millis>,<key>,<value>
```

Example:

```text
T,124002,SOC,24.5
```

## Notes

This repository documents the development/proof-of-concept stage. Hardware-specific CAN identifiers and signal meanings should only be added once verified from the relevant BMS or motor-controller documentation/testing.

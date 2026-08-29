# System Architecture

## Data path

The current proof-of-concept data path is:

1. BMS or motor controller produces CAN frames.
2. SN65HVD230 converts between CAN bus signaling and the ESP32's digital CAN/TWAI interface.
3. The kart ESP32 receives CAN frames.
4. The kart ESP32 sends compact telemetry/raw-frame text over ESP-NOW.
5. A second ESP32 acts as a bridge and forwards the received data over USB serial.
6. A laptop or Raspberry Pi reads the USB serial stream with Python/pyserial.
7. Python writes the received telemetry to a CSV log for later analysis.

## ESP-NOW bridge

Bridge STA MAC:

`38:18:2B:B2:21:60`

The sender uses this as its ESP-NOW peer address.

## Current status

The wireless and serial logging path has been verified using heartbeat packets. BMS CAN decoding has also been demonstrated independently. The next integration task is to reliably move live CAN frames through the entire path.

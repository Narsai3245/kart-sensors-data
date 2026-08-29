# CAN Bus

## ESP32 CAN interface

The project uses the ESP32 TWAI peripheral with an external SN65HVD230 CAN transceiver.

Current example pins:

- CAN TX: GPIO 5
- CAN RX: GPIO 4

The CAN bitrate must match the physical CAN network. During development, 500 kbit/s has been used in the ESP-NOW sender example, while earlier BMS testing also used 1 Mbit/s. Do not assume a bitrate without verifying the bus configuration.

## CAN frame acquisition

The ESP32 reads received frames and can inspect:

- CAN identifier
- Standard vs. extended frame
- DLC
- Individual data bytes

A received BMS frame demonstrated that the ESP32 can successfully acquire and decode CAN data.

## BMS SOC proof of concept

Verified BMS signal:

- CAN ID: `0x001`
- Signal: Pack State of Charge
- Byte: 0
- Scaling: `0.5 %/LSB`

Example: byte 0 `0x31` = decimal 49, therefore SOC = `49 × 0.5 = 24.5%`.

## Troubleshooting checklist

If no frames are received:

1. Verify CAN bitrate.
2. Verify CANH/CANL wiring.
3. Verify common ground.
4. Verify SN65HVD230 power and logic connections.
5. Verify the transceiver is not in standby.
6. Verify CAN bus termination. A normal bus has termination at its two physical ends; an extra 120/121 ohm resistor on a breakout may need to be disabled if it creates a third terminator.
7. Confirm that another node is actually transmitting.

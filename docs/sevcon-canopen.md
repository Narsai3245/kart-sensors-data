# Sevcon Gen4 / CANopen

The Sevcon Gen4 motor controller is a separate decoding task from the BMS CAN proof of concept.

The controller uses CANopen, so the team should research the relevant CANopen device/profile documentation rather than assuming that arbitrary CAN identifiers use the same signal layout as the BMS.

Potential areas for investigation include:

- CANopen network management (NMT)
- Service Data Objects (SDO)
- Process Data Objects (PDO)
- CANopen device/profile conventions relevant to motor controllers
- Sevcon-specific object dictionary and PDO mappings

Useful candidate objects should be treated as hypotheses until verified against the actual Gen4 documentation/configuration and bus traffic.

## Important distinction

CAN is the underlying communication protocol. CANopen is a higher-level protocol/profile that defines conventions for how CAN frames are used. Therefore, learning the CAN frame format and receiving frames with TWAI is not the same thing as knowing what every Sevcon frame means.

# ESP-NOW

ESP-NOW is used as the wireless link between the kart ESP32 and a second ESP32 near the laptop.

## Nodes

### Sender

Located on the kart. It reads CAN frames and transmits compact text packets over ESP-NOW.

### Bridge

Located near the laptop. It receives ESP-NOW packets and forwards the payload to the computer through its USB serial connection.

## Peer address

Current bridge STA MAC:

`38:18:2B:B2:21:60`

This must be configured in the sender's `PEER_MAC` array.

## Data formats

Raw CAN:

`F,<t_ms>,<id>,<ext>,<dlc>,<hex bytes>`

Decoded telemetry:

`T,<t_ms>,<key>,<value>`

Heartbeat:

`PING,<t_ms>`

## Debugging

A successful heartbeat such as `PING,400001` appearing in the Python console proves that the sender, ESP-NOW wireless link, bridge, USB serial connection, and Python serial reader are functioning together. It does not prove that CAN frames are being received.

#include <ESP32-TWAI-CAN.hpp>

#define CAN_TX 5
#define CAN_RX 4

CanFrame rxFrame;

#define SOC_OUT_PIN        2
#define SOC_THRESHOLD_PCT  50.0f

void printFrame(const CanFrame& f) {
  Serial.print("Received frame ID: 0x");
  Serial.print(f.identifier, HEX);
  Serial.print("  DLC: ");
  Serial.print(f.data_length_code);
  Serial.print("  Data: ");
  for (uint8_t i = 0; i < f.data_length_code; i++) {
    if (f.data[i] < 0x10) Serial.print('0');
    Serial.print(f.data[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Attempting to start CAN...");

  if (ESP32Can.begin(ESP32Can.convertSpeed(1000), CAN_TX, CAN_RX, 10, 10)) {
    Serial.println("CAN bus started!");
  } else {
    Serial.println("CAN bus failed to start!");
    while (1) {}
  }

  pinMode(SOC_OUT_PIN, OUTPUT);
  digitalWrite(SOC_OUT_PIN, LOW);
}

void loop() {
  if (ESP32Can.readFrame(rxFrame, 100)) {
    printFrame(rxFrame);

    if (rxFrame.identifier == 0x001 && rxFrame.data_length_code >= 1) {
      uint8_t soc_raw = rxFrame.data[0];
      float soc_pct = soc_raw * 0.5f;

      bool high = (soc_pct >= SOC_THRESHOLD_PCT);
      digitalWrite(SOC_OUT_PIN, high ? HIGH : LOW);

      Serial.print("Pack SOC: ");
      Serial.print(soc_pct, 1);
      Serial.print("%  -> Pin ");
      Serial.print(SOC_OUT_PIN);
      Serial.print(" = ");
      Serial.println(high ? "HIGH" : "LOW");
    }
  }
}

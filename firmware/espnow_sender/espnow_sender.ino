// SENDER: ESP32 on the kart (reads CAN, sends lines via ESP-NOW)
#include <WiFi.h>
#include <esp_now.h>
#include <ESP32-TWAI-CAN.hpp>

#define CAN_TX 5
#define CAN_RX 4

// Bridge ESP32 STA MAC
uint8_t PEER_MAC[6] = { 0x38, 0x18, 0x2B, 0xB2, 0x21, 0x60 };

// Arduino-ESP32 / ESP-IDF 5.x callback signature
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // Optional debug:
  // Serial.printf("ESPNOW send: %s\n", status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

bool espnow_init_and_add_peer() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) return false;
  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peer{};
  memcpy(peer.peer_addr, PEER_MAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  peer.ifidx = WIFI_IF_STA;

  if (esp_now_add_peer(&peer) != ESP_OK) return false;
  return true;
}

CanFrame rx;

static void printFrameUSB(const CanFrame& f) {
  Serial.printf("RX 0x%03X %s DLC:%u   ",
                f.identifier, f.extd ? "EXT" : "STD", f.data_length_code);
  for (uint8_t i = 0; i < f.data_length_code; i++) {
    Serial.printf("%02X ", f.data[i]);
  }
  Serial.println();
}

void sendFrameESPNOW(const CanFrame& f) {
  char line[128];
  char bytes[3 * 8 + 1];
  int p = 0;

  for (uint8_t i = 0; i < f.data_length_code; i++) {
    p += snprintf(bytes + p, sizeof(bytes) - p, "%02X%s",
                  f.data[i], (i + 1 < f.data_length_code) ? " " : "");
    if (p >= (int)sizeof(bytes) - 1) break;
  }

  int n = snprintf(line, sizeof(line), "F,%lu,%u,%u,%u,%s\n",
                   (unsigned long)millis(),
                   (unsigned)f.identifier,
                   (unsigned)f.extd,
                   (unsigned)f.data_length_code,
                   bytes);

  esp_now_send(PEER_MAC, (const uint8_t*)line, n);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  if (!espnow_init_and_add_peer()) {
    Serial.println("ESP-NOW init/peer FAILED");
    while (true) {}
  }

  Serial.print("Sender ready. My MAC: ");
  Serial.println(WiFi.macAddress());

  if (!ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 20, 20)) {
    Serial.println("CAN start failed");
    while (true) {}
  }

  Serial.println("CAN started");
}

void loop() {
  while (ESP32Can.readFrame(rx, 2)) {
    printFrameUSB(rx);
    sendFrameESPNOW(rx);

    // BMS SOC: ID 0x001, Byte 0, 0.5 %/LSB
    if (rx.identifier == 0x001 && rx.data_length_code >= 1) {
      float soc = rx.data[0] * 0.5f;

      char line[64];
      int n = snprintf(line, sizeof(line), "T,%lu,SOC,%.1f\n",
                       (unsigned long)millis(), soc);
      esp_now_send(PEER_MAC, (const uint8_t*)line, n);
    }
  }
}

// BRIDGE: ESP32 near laptop (receives ESP-NOW, prints to USB)
// Arduino-ESP32 / ESP-IDF 5.x

#include <WiFi.h>
#include <esp_now.h>
#include <esp_idf_version.h>

#if ESP_IDF_VERSION_MAJOR >= 5
  #include <esp_mac.h>
#else
  #include <esp_system.h>
#endif

static String macToString(const uint8_t* mac) {
  char buf[18];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

#if ESP_IDF_VERSION_MAJOR >= 5
void onRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  const uint8_t* mac = info ? info->src_addr : nullptr;
  Serial.printf("# ESPNOW from %s len=%d\n",
                mac ? macToString(mac).c_str() : "<unknown>", len);
  Serial.write(data, len);
  if (len == 0 || data[len - 1] != '\n') Serial.println();
}
#else
void onRecv(const uint8_t* mac, const uint8_t* data, int len) {
  Serial.printf("# ESPNOW from %s len=%d\n",
                mac ? macToString(mac).c_str() : "<unknown>", len);
  Serial.write(data, len);
  if (len == 0 || data[len - 1] != '\n') Serial.println();
}
#endif

void setup() {
  Serial.begin(115200);
  delay(200);

  uint8_t sta_mac[6];
  esp_read_mac(sta_mac, ESP_MAC_WIFI_STA);
  Serial.print("# Bridge ready. My MAC: ");
  Serial.println(macToString(sta_mac));

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("# ESP-NOW init failed");
    while (true) {}
  }

  esp_now_register_recv_cb(onRecv);
}

void loop() {
  // Receive callback handles incoming packets.
}

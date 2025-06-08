#include <WiFi.h>

#define WIFI_SSID "DLink2G-AD55"
#define WIFI_PASSWORD "yrWvsj5NQm"

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected.");
  } else {
    Serial.println("WiFi Not Connected!");
  }

  delay(2000); // check every 2 seconds
}

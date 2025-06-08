#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <DHT.h>

#define DHT11_PIN D12
#define WIFI_SSID "Anfield"
#define WIFI_PASSWORD "Haca2635"

#define MQTT_HOST IPAddress(192, 168, 18, 155)  
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

DHT dht11(DHT11_PIN, DHT11);

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();  // No username/password since broker allows anonymous
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT!");
}

void onWiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xTimerStop(mqttReconnectTimer, 0);
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  dht11.begin();

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, 0, (TimerCallbackFunction_t)connectToMqtt);
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, 0, (TimerCallbackFunction_t)connectToWiFi);

  WiFi.onEvent(onWiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWiFi();
}

void loop() {
  float humi  = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  if (!isnan(humi) && !isnan(tempC)) {
    // Format JSON string
    String payload = "{\"node\": \"node01\", \"temperature\": ";
    payload += tempC;
    payload += ", \"humidity\": ";
    payload += humi;
    payload += "}";

    // Publish to MQTT topic
    mqttClient.publish("sensor/data", 0, false, payload.c_str());

    Serial.println("Published: " + payload);
  } else {
    Serial.println("Failed to read from DHT11 sensor!");
  }

  delay(5000);
}

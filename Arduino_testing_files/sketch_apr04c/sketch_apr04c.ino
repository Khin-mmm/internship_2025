#include <WiFi.h>
#include <AsyncMqttClient.h>

#define MOISTURE_PIN A0
#define RELAY_PIN D12

//#define WIFI_SSID "Anfield"
//#define WIFI_PASSWORD "Haca2635"

#define WIFI_SSID "eee-iot"
#define WIFI_PASSWORD "I0t@sep2025!"

#define MQTT_HOST IPAddress(172, 23, 2, 51)  // change with new ip of rasp
//#define MQTT_HOST IPAddress(192, 168, 18, 155) 
#define MQTT_PORT 1883

//bool waterStress = false;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.setCredentials("mqtt-user", "mqtt-password");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT!");

  mqttClient.publish(
    "homeassistant/sensor/esp32_temperature/config", 0, true,
    "{\"name\":\"ESP32 Temperature\",\"state_topic\":\"esp32/sensor/temperature\",\"unit_of_measurement\":\"°C\",\"device_class\":\"temperature\"}"
  );

  mqttClient.publish(
    "homeassistant/sensor/esp32_humidity/config", 0, true,
    "{\"name\":\"ESP32 Humidity\",\"state_topic\":\"esp32/sensor/humidity\",\"unit_of_measurement\":\"%\",\"device_class\":\"humidity\"}"
  );

  mqttClient.subscribe("esp32/relay", 1);
  mqttClient.subscribe("esp32/water_stress", 1);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String message = "";
  for (int i = 0; i < len; i++) {
    message += payload[i];
  }

  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

//  if (String(topic) == "esp32/water_stress") {
//    if (message == "on") {
//      waterStress = true;
//      Serial.println("Water Stress is ON – Auto watering disabled.");
//    } else if (message == "off") {
//      waterStress = false;
//      Serial.println("Water Stress is OFF – Auto watering enabled.");
//    }
//  }
    if (String(topic) == "esp32/relay") {
      Serial.println(message);
    if (message == "on") {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Pump turned ON");
    } else if (message == "off") {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Pump  e2ei turned OFF");
    }
  }
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

float reading_moisture(){
  int sensor_analog = analogRead(MOISTURE_PIN);
  float _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  return _moisture;
}
  
void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT); 
  digitalWrite(RELAY_PIN, LOW);
  
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, (TimerCallbackFunction_t)connectToMqtt);
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, (TimerCallbackFunction_t)connectToWiFi);

  WiFi.onEvent(onWiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWiFi();
  
}

void loop() {
  // read humidity
  float humi  = reading_moisture();

  // check whether the reading is successful or not
  if ( isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    mqttClient.publish("esp32/sensor/humidity", 0, false, String(humi).c_str());
  }
  delay(2000);
}

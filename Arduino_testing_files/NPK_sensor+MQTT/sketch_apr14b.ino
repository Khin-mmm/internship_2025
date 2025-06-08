#include <WiFi.h>
#include <AsyncMqttClient.h>

#define RXD1 16  // D10
#define TXD1 17  // D11
#define RELAY_PIN D12

HardwareSerial mod(1);  // UART1

const byte readAll[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x09};
byte values[30];

//#define WIFI_SSID "Anfield"
//#define WIFI_PASSWORD "Haca2635"

#define WIFI_SSID "eee-iot"
#define WIFI_PASSWORD "I0t@sep2025!"

#define MQTT_HOST IPAddress(172, 23, 2, 51)  // change with new ip of rasp
//#define MQTT_HOST IPAddress(192, 168, 18, 155) 
#define MQTT_PORT 1883

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
    "homeassistant/sensor/esp32_moisture/config", 0, true,
    "{\"name\":\"ESP32 Moisture\",\"state_topic\":\"esp32/sensor/moisture\",\"unit_of_measurement\":\"%\"}"
  );

  mqttClient.publish(
    "homeassistant/sensor/esp32_conductivity/config", 0, true,
    "{\"name\":\"ESP32 Conductivity\",\"state_topic\":\"esp32/sensor/conductivity\",\"unit_of_measurement\":\"μS/cm\",\"device_class\":\"conductivity\"}"
  );

  mqttClient.publish(
    "homeassistant/sensor/esp32_ph/config", 0, true,
    "{\"name\":\"ESP32 PH\",\"state_topic\":\"esp32/sensor/ph\"}"
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

void reading_data(float &moisture, float &temp, float &ec, float &ph){
  mod.write(readAll, 8);
  mod.flush();
  delay(200);

  int index = 0;
  while (mod.available() && index < sizeof(values)) {
    values[index++] = mod.read();
  }

  int start = -1;
  for (int i = 0; i < index - 2; i++) {
    if (values[i] == 0x01 && values[i + 1] == 0x03 && values[i + 2] == 0x08) {
      start = i;
      break;
    }
  }

  if (start >= 0 && (start + 11 < index)) {
    moisture = ((values[start + 3] << 8) | values[start + 4]) / 10.0;
    int16_t rawTemp = (values[start + 5] << 8) | values[start + 6];
    temp = rawTemp / 10.0;
    ec = (values[start + 7] << 8) | values[start + 8];
    ph = ((values[start + 9] << 8) | values[start + 10]) / 10.0;
  } else {
    moisture = temp = ec = ph = -1; // signal error
  }
}
  
void setup() {
  Serial.begin(9600);
  mod.begin(4800, SERIAL_8N1, RXD1, TXD1);

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
  // read sensor data
  float moisture, temp, ec, ph;
  reading_data(moisture, temp, ec, ph);

  if (moisture == -1) {
      Serial.println("Failed to read from Modbus sensor");
    } else {
      Serial.print("Moisture: "); Serial.println(moisture);
      Serial.print("Temp    : "); Serial.println(temp);
      Serial.print("EC      : "); Serial.println(ec);
      Serial.print("pH      : "); Serial.println(ph);
  
      // MQTT publishing
      mqttClient.publish("esp32/sensor/moisture", 0, false, String(moisture).c_str());
      mqttClient.publish("esp32/sensor/temperature", 0, false, String(temp).c_str());
      mqttClient.publish("esp32/sensor/conductivity", 0, false, String(ec).c_str());
      mqttClient.publish("esp32/sensor/ph", 0, false, String(ph).c_str());
    }
    
  delay(2000);
}

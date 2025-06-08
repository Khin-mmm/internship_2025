#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <DHT.h>

#define DHT11_PIN D12
#define LED_PIN D9

#define WIFI_SSID "DLink2G-AD55"
#define WIFI_PASSWORD "yrWvsj5NQm"

#define MQTT_HOST IPAddress(192, 168, 0, 2)  
#define MQTT_PORT 1883

bool waterStress = false;

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
  mqttClient.setCredentials("mqtt-user", "mqtt-password");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT!");
  mqttClient.publish("test/topic", 0, false, "Hello from ESP32");

  mqttClient.publish(
    "homeassistant/sensor/esp32_temperature/config", 0, true,
    "{\"name\":\"ESP32 Temperature\",\"state_topic\":\"esp32/sensor/temperature\",\"unit_of_measurement\":\"°C\",\"device_class\":\"temperature\"}"
  );

  mqttClient.publish(
    "homeassistant/sensor/esp32_humidity/config", 0, true,
    "{\"name\":\"ESP32 Humidity\",\"state_topic\":\"esp32/sensor/humidity\",\"unit_of_measurement\":\"%\",\"device_class\":\"humidity\"}"
  );

  mqttClient.subscribe("esp32/led", 1);
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

  if (String(topic) == "esp32/water_stress") {
    if (message == "on") {
      waterStress = true;
      Serial.println("Water Stress is ON – Auto watering disabled.");
    } else if (message == "off") {
      waterStress = false;
      Serial.println("Water Stress is OFF – Auto watering enabled.");
    }
  }
  if (String(topic) == "esp32/led") {
    if (message == "on") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED turned ON");
    } else if (message == "off") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED turned OFF");
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

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, LOW);
  
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, (TimerCallbackFunction_t)connectToMqtt);
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, (TimerCallbackFunction_t)connectToWiFi);

  WiFi.onEvent(onWiFiEvent);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWiFi();
  
  dht11.begin(); // initialize the DHT11 sensor
}

void loop() {
  // read humidity
  float humi  = dht11.readHumidity();
  // read temperature in Celsius
  float tempC = dht11.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht11.readTemperature(true);

  // check whether the reading is successful or not
  //if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
    //Serial.println("Failed to read from DHT11 sensor!");
  //} else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    mqttClient.publish("esp32/sensor/humidity", 0, false, String(humi).c_str());
    
    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    mqttClient.publish("esp32/sensor/temperature", 0, false, String(tempC).c_str());
    //Serial.print(tempF);
    //Serial.println("°F");
    if (humi > 70 && !waterStress) {
      digitalWrite(LED_PIN, HIGH);  
      mqttClient.publish("ha/led/control", 0, false, "on"); 
    }
    if (humi < 70 && !waterStress) {
      digitalWrite(LED_PIN, LOW);  
      mqttClient.publish("ha/led/control", 0, false, "off"); 
    }  
    if(waterStress){
      digitalWrite(LED_PIN, LOW);  
      mqttClient.publish("ha/led/control", 0, false, "off");
}

  //}

  // wait a 2 seconds between readings
  delay(2000);
}

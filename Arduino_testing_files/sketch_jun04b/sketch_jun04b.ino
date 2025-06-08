#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

//#include <DHT.h>
//#define DHT11_PIN D12

//DHT dht11(DHT11_PIN, DHT11);

// Create BME680 object with I2C
Adafruit_BME680 bme;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("BME680 Temperature and Humidity Reading"));

  // Use GPIO 21 for SDA, 22 for SCL
  Wire.begin(21, 22);

  // Begin using I2C address 0x77 
  if (!bme.begin(0x77)) {
    Serial.println(F("Could not find a valid BME680 sensor at 0x77!"));
    while (1);
//  dht11.begin();
  }

  // Setup sensor settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(0, 0); // Required even if not reading gas
}

void loop() {
  // Perform the reading
  if (!bme.performReading()) {
    Serial.println(F("Failed to perform reading :("));
    return;
  }

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.println();
  delay(2000);

//  float humi  = dht11.readHumidity();
//  // read temperature in Celsius
//  float tempC = dht11.readTemperature();
//  Serial.print("dht_Humidity: ");
//  Serial.print(humi);
//  Serial.print("%");
//
//  Serial.println();
//
//  Serial.print("dht_Temperature: ");
//  Serial.print(tempC);
//  Serial.print("°C ");
//
//  Serial.println();
//  delay(2000);
}

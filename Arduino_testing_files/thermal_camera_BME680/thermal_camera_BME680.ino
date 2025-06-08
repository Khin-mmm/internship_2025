#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AMG88xx.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_AMG88xx amg;           // Thermal camera
Adafruit_BME680 bme;            // Environmental sensor (I2C)

float pixels[64];               // 8x8 thermal pixel array

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL for ESP32

  // AMG8833 init
  if (!amg.begin()) {
    Serial.println("Could not find a valid AMG8833 sensor!");
    while (1);
  }

  // BME680 init
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor!");
    while (1);
  }

  // BME680 sensor settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(0, 0);  // Gas heater off
  delay(100);
}

void loop() {
  // --- Read AMG8833 ---
  amg.readPixels(pixels);
  Serial.println("THERMAL (AMG8833):");
  for (int i = 0; i < 64; i++) {
    Serial.print(pixels[i]);
    Serial.print(", ");
    if ((i + 1) % 8 == 0) Serial.println();
  }
  Serial.println();

  // --- Read BME680 ---
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println("Failed to start BME680 reading.");
    delay(1000);
    return;
  }

  delay(100); // Small wait for reading (non-blocking)

  if (!bme.endReading()) {
    Serial.println("Failed to complete BME680 reading.");
    delay(1000);
    return;
  }

  Serial.println("ENVIRONMENTAL (BME680):");
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000); // Wait before next loop
}

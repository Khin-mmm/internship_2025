#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("BME680 async test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

//  // Set up oversampling and filter initialization
//  bme.setTemperatureOversampling(BME680_OS_8X);
//  bme.setHumidityOversampling(BME680_OS_2X);
//  bme.setPressureOversampling(BME680_OS_4X);
//  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
//  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(F("Failed to begin reading :("));
}

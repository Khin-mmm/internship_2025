#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;
float pixels[64];

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // Adjust to your ESP32's I2C pins
  if (!amg.begin()) {
    Serial.println("AMG8833 not found");
    while (1);
  }
  delay(100);
}

void loop() {
  amg.readPixels(pixels);
  for (int i = 0; i < 64; i++) {
    Serial.print(pixels[i]);
    if (i < 63) Serial.print(",");  // CSV format
  }
  Serial.println();  // End of frame
  delay(200);
}

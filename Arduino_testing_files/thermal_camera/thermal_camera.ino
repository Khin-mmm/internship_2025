#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[64];  // 8x8 = 64 values

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  if (!amg.begin()) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1);
  }
  delay(100);
}

void loop() {
  amg.readPixels(pixels);
  for (int i = 0; i < 64; i++) {
    Serial.print(pixels[i]);
    Serial.print(", ");
    if ((i + 1) % 8 == 0) Serial.println(); // new line every 8 values
  }
  Serial.println(); // extra space between frames
  delay(500);
}

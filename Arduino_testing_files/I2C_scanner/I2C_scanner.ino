#include <Wire.h>

void setup() {
  Serial.begin(115200);
//  while (!Serial);

  // Explicitly set SDA and SCL
  Wire.begin(21, 22);  // SDA = 21, SCL = 22 for FireBeetle ESP32-E

  Serial.println("\nI2C Scanner");
  
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      Serial.println(address, HEX);
    }
    delay(10);
  }
}

void loop() {}

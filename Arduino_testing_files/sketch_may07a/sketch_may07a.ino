#define BATTERY_PIN 34 // GPIO34 is A0 and connected to battery divider
float batteryVoltage = 0.0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int analogValue = analogRead(BATTERY_PIN); // Read ADC value (0–4095)
  
  // FireBeetle ESP32-E uses a voltage divider of 1MΩ + 1MΩ (i.e. divide by 2)
  // ADC range: 0–3.3V mapped to 0–4095, so real voltage = (analog / 4095.0) * 3.3 * 2
  batteryVoltage = (analogValue / 4095.0) * 3.3 * 2;
  Serial.print(analogValue);
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.println(" V");

  delay(1000);
}

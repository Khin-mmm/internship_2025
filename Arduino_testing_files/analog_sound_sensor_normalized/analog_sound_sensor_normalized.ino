#define analogPin A0     // Analog output of the sensor
#define digitalPin D12   // Digital output (optional)
#define ledPin D9        // Onboard LED

void setup() {
    pinMode(analogPin, INPUT);
    pinMode(digitalPin, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);  // Recommend matching this with other sensors
    Serial.println("Sound Sensor Normalized Test");
}

void loop() {
    int analogValue = analogRead(analogPin);           // 0–4095
    float normalized = analogValue / 4095.0;            // Normalize to 0.0–1.0

    // LED indication using digital output (optional)
    int digitalValue = digitalRead(digitalPin);
    digitalWrite(ledPin, digitalValue);

    // Print normalized sound value
    Serial.print("Normalized Sound Level: ");
    Serial.println(normalized, 3);  // 3 decimal places

    delay(1000);  // 1-second delay between readings
}

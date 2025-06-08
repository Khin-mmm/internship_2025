#define analogPin A0  // Analog output of the sensor
#define digitalPin D12   // Digital output of the sensor
#define ledPin D9    // Onboard LED

void setup() {
    pinMode(analogPin, INPUT);
    pinMode(digitalPin, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
    Serial.println("KY-037 Sound Detection Module Test");
}

void loop() {
    int analogValue = analogRead(analogPin);
    int digitalValue = digitalRead(digitalPin);

    if (digitalValue == HIGH) {
        digitalWrite(ledPin, HIGH); // Turn ON the LED when sound is detected
    } else {
        digitalWrite(ledPin, LOW);  // Turn OFF the LED when no sound is detected
    }

    Serial.print("Analog Value: ");
    Serial.println(analogValue);
    delay(1000);
}

  

  

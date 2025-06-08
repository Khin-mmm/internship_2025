#define RELAY_PIN D12

void setup() {
  Serial.begin(9600);             // Start serial communication
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Start with relay OFF (active LOW)
  Serial.println("Send 1 to turn ON relay, 0 to turn OFF relay.");
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();  // Read one character from Serial
    
    if (input == '1') {
      digitalWrite(RELAY_PIN, HIGH);  // Relay ON
      Serial.println("Relay turned ON");
    } else if (input == '0') {
      digitalWrite(RELAY_PIN, LOW); // Relay OFF
      Serial.println("Relay turned OFF");
    } else {
      Serial.println("Invalid input. Send 1 (ON) or 0 (OFF).");
    }
  }
}

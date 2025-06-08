#define RXD1 16  // D10
#define TXD1 17  // D11

HardwareSerial mod(1);  // UART1

const byte readAll[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x09};
byte values[30];

void setup() {
  Serial.begin(115200);
  mod.begin(4800, SERIAL_8N1, RXD1, TXD1);
  delay(500);
  Serial.println("Reading NPK sensor via RS485...");
}

void loop() {
  mod.write(readAll, 8);
  mod.flush();
  delay(200);

  int index = 0;
  while (mod.available() && index < sizeof(values)) {
    values[index++] = mod.read();
  }

  Serial.print("Raw Response: ");
  for (int i = 0; i < index; i++) {
    Serial.print(values[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  int start = -1;
  for (int i = 0; i < index - 2; i++) {
    if (values[i] == 0x01 && values[i + 1] == 0x03 && values[i + 2] == 0x08) {
      start = i;
      break;
    }
  }

  if (start >= 0 && (start + 11 < index)) {
    int moisture     = (values[start + 3] << 8) | values[start + 4];
    int rawTemp      = (values[start + 5] << 8) | values[start + 6];
    int conductivity = (values[start + 7] << 8) | values[start + 8];
    int ph_raw       = (values[start + 9] << 8) | values[start + 10];
    float pH = ph_raw / 100.0;

    // Temperature correction (signed)
    int temperature = (rawTemp > 32767) ? rawTemp - 65536 : rawTemp;

    // 🌿 Option 1: Direct scaling (0–1000 → 0–100%)
    float moisturePercent = (moisture / 1000.0) * 100;

    // 🌿 Option 2: Calibrated scaling (dry = 300, wet = 1000)
    // float moisturePercent = ((moisture - 300.0) / (1000.0 - 300.0)) * 100;

    // Clamp the result
    moisturePercent = constrain(moisturePercent, 0, 100);

    Serial.println("Sensor Readings:");
    Serial.print("Moisture     : "); Serial.print(moisturePercent, 1); Serial.println(" %");
    Serial.print("Temperature  : "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Conductivity : "); Serial.print(conductivity); Serial.println(" uS/cm");
    Serial.print("pH           : "); Serial.println(pH);
  } else {
    Serial.println("❌ Invalid or corrupted Modbus response");
  }

  delay(3000);
}

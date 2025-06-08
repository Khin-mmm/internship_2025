#define RXD1 16  // D10
#define TXD1 17  // D11

HardwareSerial mod(1);  // UART1

const byte readAll[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x09};
byte values[30];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mod.begin(4800, SERIAL_8N1, RXD1, TXD1);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  mod.write(readAll, 8);
  mod.flush();
  delay(200);

  int index = 0;
  while (mod.available() && index < sizeof(values)) {
    values[index++] = mod.read();
  }

//  Serial.print("Raw Response: ");
//  for (int i = 0; i < index; i++) {
//    Serial.print(values[i],HEX);
//    Serial.print(" ");
//  }
//  Serial.println();

  int start = -1;
  for (int i = 0; i < index - 2; i++) {
    if (values[i] == 0x01 && values[i + 1] == 0x03 && values[i + 2] == 0x08) {
      start = i;
      break;
    }
  }

  if (start >= 0 && (start + 11 < index)) {
    float moisture     = ((values[start + 3] << 8) | values[start + 4])/10;
    int16_t rawTemp = (values[start + 5] << 8) | values[start + 6]; // signed
    float temp = rawTemp / 10.0;
    float ec = (values[start + 7] << 8) | values[start + 8];
    float ph       = ((values[start + 9] << 8) | values[start + 10])/10;

//    int raw_moisture     = (values[start + 3] << 8) | values[start + 4];
//    int rawTemp      = (values[start + 5] << 8) | values[start + 6];
//    int conductivity = (values[start + 7] << 8) | values[start + 8];
//    int ph_raw       = (values[start + 9] << 8) | values[start + 10];
    Serial.print(moisture);
    Serial.print(",");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(ec);
    Serial.print(",");
    Serial.println(ph);
  }
}

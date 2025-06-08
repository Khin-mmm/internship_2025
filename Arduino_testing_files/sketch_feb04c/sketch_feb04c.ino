#include <SoftwareSerial.h>

#define ARDUINO_RX 10  // should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 11  // connect to RX of the module


#define DEV_TF            0X02

// MP3 Commands
#define CMD_SEL_DEV       0X09
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E

bool isMusicPlaying = false;

// Function prototype
void sendCommand(byte command, byte dat1 = 0, byte dat2 = 0);

void setup() {
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);

  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == '1') {
      toggleMusic();
    }
  }
}

void toggleMusic() {
  if (isMusicPlaying) {
    stopMusic();
    isMusicPlaying = false;
  } else {
    playMusic();
    isMusicPlaying = true;
  }
}

void playMusic() {
  sendCommand(CMD_PLAY);
}

void stopMusic() {
  sendCommand(CMD_PAUSE);
}

void sendCommand(byte command, byte dat1, byte dat2) {
  delay(20);
  byte Send_buf[8] = {0x7E, 0xFF, 0x06, command, 0x01, dat1, dat2, 0xEF};
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]);
  }
}

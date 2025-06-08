#include <SoftwareSerial.h>

#define ARDUINO_RX 10  // Connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 11  // Connect to RX of the module

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

bool isMusicPlaying = false;

void setup() {
  Serial.begin(9600);
  mp3.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
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
  mp3.write((uint8_t)0x7E); // Start byte
  mp3.write((uint8_t)0xFF); // Version
  mp3.write((uint8_t)0x06); // Command length
  mp3.write((uint8_t)0x0D); // Command (Play)
  mp3.write((uint8_t)0x00); // No feedback
  mp3.write((uint8_t)0x00); // Reserved
  mp3.write((uint8_t)0x00); // Reserved
  mp3.write((uint8_t)0xEF); // End byte
}

void stopMusic() {
  mp3.write((uint8_t)0x7E); // Start byte
  mp3.write((uint8_t)0xFF); // Version
  mp3.write((uint8_t)0x06); // Command length
  mp3.write((uint8_t)0x0E); // Command (Pause)
  mp3.write((uint8_t)0x00); // No feedback
  mp3.write((uint8_t)0x00); // Reserved
  mp3.write((uint8_t)0x00); // Reserved
  mp3.write((uint8_t)0xEF); // End byte
}

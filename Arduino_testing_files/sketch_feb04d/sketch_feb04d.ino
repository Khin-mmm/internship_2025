
//#include <avr/interrupt.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
// addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
// addr can be 0x3F or 0x27
int counterA = 0; //inialized CounterA as 0
void setup() {
Serial.begin(9600);
pinMode(4, INPUT_PULLUP);
PCICR |= B00000100; // Enable interrupts on PD port
PCMSK2 |= B00010000; // Trigger interrupts on pins D4 (Hit a)
lcd.init();
lcd.begin(16, 2);
lcd.backlight();
lcd.clear();
//interrupts();
}

void loop() {
lcd.setCursor(0, 0); //set cursor to left top corner
lcd.print("A score is:");
lcd.print(counterA);
if (counterA==4)
{
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("A is winner");
lcd.setCursor(0, 1);
lcd.print("Reset to restart");
}
while((!digitalRead(4)))
delay(500);
}
ISR (PCINT2_vect) {
if (!digitalRead(4))
counterA++;}

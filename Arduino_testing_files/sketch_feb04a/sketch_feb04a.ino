#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include "MAX30105.h"  //Get it here: http://librarymanager/All#SparkFun_MAX30105
MAX30105 particleSensor;
#include "heartRate.h"

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;           // Answer from the MP3.


String sanswer(void);
String sbyte2hex(uint8_t b);


/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_STOP_PLAY     0X16  // Stop playing continuously. 
#define CMD_SEL_DEV       0X09
#define DEV_TF            0X02
#define music_pin 4
#define heartrate_pin 2
void music(void);
void heartrateON_OFF(void);
void mp3(void);
void mp3STOP(void);
void heartrate(void);
int target_a_hit = 0;
int target_b_hit = 0;

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(music_pin, INPUT_PULLUP);
  pinMode(heartrate_pin, INPUT_PULLUP);
  PCICR |= B00000100; // Enable interrupts on PD port (D0 to D7)
  PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
 //interrupts();
  // Initialize sensor
   if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    lcd.setCursor(0,0);
    lcd.print("not found");
    while (1);
  }
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}
void loop() {
 /* Serial.print("A= ");
  Serial.println(target_a_hit); delay(1000);
  Serial.print("B= ");
  Serial.println(target_b_hit); delay(1000);*/
  //lcd.setCursor(5,0);
  //lcd.print("SP-BOT");
  music();
  heartrateON_OFF();

}


ISR(PCINT2_vect) {
  if (!digitalRead(music_pin))
   {
    target_a_hit++;
   // sendCommand(CMD_STOP_PLAY, 0, 4);
    //delay(10);
  
   }

else if (!digitalRead(heartrate_pin))
 { 
  target_b_hit++;
 }   
}


void music(){

 if(target_a_hit==1){
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Music is playing");
  //sendCommand(CMD_PLAY_W_INDEX, 0, 4);
  delay(500);
 }
 if(target_a_hit==2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Music stops");
  delay(1500);
  target_a_hit=0;
  lcd.clear();
 }
}

void heartrateON_OFF(){
//Serial.println(target_b_hit);

 if (target_b_hit==1){

  heartrate();
 }
 else if (target_b_hit == 2)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("heartrate(OFF)");
  delay(1500);
  target_b_hit=0;
  lcd.clear();
  }
}
void heartrate(){
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  if (irValue > 10000){
    if(beatAvg<60){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Testing!!!");
    //lcd.setCursor(0,1);
    //lcd.print("WaitFor A While");
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("BPM= ");
      lcd.println(beatAvg);
    }
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TouchUrFinger"); 
    //Serial.print(" No finger?");
  }
}
void mp3(){
  sendCommand(CMD_PLAY_W_INDEX, 0, 2);
  delay(10000);
}
void mp3STOP(){
  sendCommand(CMD_STOP_PLAY, 0, 2);
  delay(10000);
  sendCommand(CMD_STOP_PLAY, 0, 3);
  delay(10000);
  sendCommand(CMD_STOP_PLAY, 0, 4);
  delay(10000);
}
void sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat1, byte dat2){
  delay(20);
  Send_buf[0] = 0x7E;    //
  Send_buf[1] = 0xFF;    //
  Send_buf[2] = 0x06;    // Len
  Send_buf[3] = command; //
  Send_buf[4] = 0x01;    // 0x00 NO, 0x01 feedback
  Send_buf[5] = dat1;    // datah
  Send_buf[6] = dat2;    // datal
  Send_buf[7] = 0xEF;    //

  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.write(Send_buf[i]) ;
  }
}



/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                       */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}


/********************************************************************************/
/*Function: shex2int. Returns a int from an HEX string.                         */
/*Parameter: s. char *s to convert to HEX.                                      */
/*Parameter: n. char *s' length.                                                */
/*Return: int                                                                   */

int shex2int(char *s, int n){
  int r = 0;
  for (int i=0; i<n; i++){
     if(s[i]>='0' && s[i]<='9'){
      r *= 16; 
      r +=s[i]-'0';
     }else if(s[i]>='A' && s[i]<='F'){
      r *= 16;
      r += (s[i] - 'A') + 10;
     }
  }
  return r;
}

//Motors
#define left_motorA_IN2 6      //Connect to Arduino pin 6
#define left_motorA_IN1 9      //Connect to Arduino pin 
#define right_motorB_IN4 3      //Connect to Arduino pin 6
#define right_motorB_IN3 5
void speed_forward(void);
void Turn_right(void);
void Turn_left(void);
void brake(void);

//Ultrasound Sensor
#define TrigPin1 11   
#define EchoPin1 10   

//IR sensors
#define IR1Pin  A1

//LCD Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void reminder(int time);
void lcd_display(char text1[], char text2[] = NULL);
int breakfast =1;
int lunch = 2;
int dinner = 3;

//Temperature
#include <dht.h>
dht DHT;
#define DHT11_PIN A0

//Buzzer for alarm
#define buzzer 7
void alarm(void);

//Target Sensors
#define hitA 4
#define hitB 2
int IR1_Val;

//MP3 Player
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
String mp3Answer;           // Answer from the MP3.
String sanswer(void);
String sbyte2hex(uint8_t b);
#define CMD_PLAY_W_INDEX  0X03
#define CMD_SEL_DEV       0X09
#define CMD_STOP_PLAY     0X16 
#define DEV_TF            0X02


void setup() {
  pinMode(left_motorA_IN2, OUTPUT);  //Output Left Motor IN2
  pinMode(left_motorA_IN1, OUTPUT);   //Output Left Motor IN1
  pinMode(right_motorB_IN4, OUTPUT);  //Output Right Motor IN4
  pinMode(right_motorB_IN3, OUTPUT);  //Output Right Motor IN3
  pinMode(TrigPin1, OUTPUT);  //Output the range to EchoPin1
  pinMode(EchoPin1, INPUT);   //Input the range from TrigPin1
  lcd.init();  //LCD displayer
  lcd.begin(16, 2); //16 columns and 2 rows on LCD
  lcd.backlight();  //Turn backlight on
  PCICR |= B00000100; // Enable interrupts on PD port (D0 to D7)
  PCMSK2 |= B00000100; // Trigger interrupts on pin D2
  Serial.begin(9650);  
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
}

void loop() {
 Bedroom();
 Dinning_room();
 Living_room();
 Livingroom_to_Bedroom();

}

void Bedroom (){
int Distance1=ultrasound();
  speed_forward();
  if(Distance1>=10)
  {
    IR1_Val = digitalRead(A1);
    if(IR1_Val==1)  //Now, I am in the bedroom.
    {
      brake();
      while (digitalRead(hitB))
      {
        alarm(); //Alarming to wake up my master
      }
      lcd_display("GoodMorning");
      sendCommand(CMD_PLAY_W_INDEX, 0, 3);
      delay(3000); 
      //heartbeat function
      wait();
      reminder(breakfast); //Here, I remind to have breakfast
      lcd.clear();
      Turn_right(); //All the tasks in the bedroom are finished, I continue to go to dining room
      delay(650);
    }
    delay(500);
  }
  else{
  brake();
  }
}

void Dinning_room(){
  int Distance1=ultrasound();
  speed_forward();
  if(Distance1>=10)
  {
    IR1_Val = digitalRead(A1);
    if(IR1_Val==1)  //Now, SP-bot is in the Dining Room.
    {
      brake();
    /*Here, I am waiting for my master coming. Assuming the laser pointer 
    as the camera to show my master is coming to the dining room.*/
      wait();
      lcd_display("Playing Music");  //I play the pleasant music to have breakfast with fresh mind
      delay(800);
      lcd.clear();
      sendCommand(CMD_PLAY_W_INDEX, 0, 19); //Music Playing
      delay(20000); 
      Turn_right(); //I continue to go to the living room to provide companionship 
      delay(650);
    }
    delay(500);
  }
  else{
   brake();
  }
}

void Living_room(){
int Distance1=ultrasound();
speed_forward();
  if(Distance1>=10)
  { 
    IR1_Val = digitalRead(A1);
    if(IR1_Val==1) //Now, I am in the living room.
    {
      brake();
      wait();
      lcd_display("Quote of", "the Day"); //Display LCD.
      sendCommand(CMD_PLAY_W_INDEX, 0, 11); //Speak the motivational quote to value his life
      delay(6500); 
      wait();
      lcd_display("Good Afternoon"); //Display LCD. Assuming time is already 12pm.
      sendCommand(CMD_PLAY_W_INDEX, 0, 4);  //MP3 for Good Afternoon
      delay(2000); 
      reminder(lunch); //Remind for lunch
      delay(1000);
      lcd.clear();
      wait();
      lcd_display("Free time","TV"); //LCD Display. Suggestion to spend free time with interesting moments
      delay(1000);
      sendCommand(CMD_PLAY_W_INDEX, 0, 18); //MP3 for blinded person to spend free time with TV, news channels
      delay(8500);
      lcd_display("Good Evening"); //Display LCD to know the time. Assuming time is in the evening while watching TV
      sendCommand(CMD_PLAY_W_INDEX, 0, 5); //MP3 for blinded person to know time is around 4pm, evening.
      delay(2000); 
      lcd_display("Walking Time"); //Display LCD to suggest to walk or do physical exercises for good health and well being leading to SDG 3.
      sendCommand(CMD_PLAY_W_INDEX, 0, 12); //MP3
      delay(5500); 
      wait();
      reminder(dinner); //Remind for dinner on both LCD and MP3
      delay(1000);
      lcd.clear();
      wait();
      lcd_display("Game Time"); //LCD shows to play games in leisure to develop the brain memory
      delay(1000);
      sendCommand(CMD_PLAY_W_INDEX, 0, 16); //MP3 to play games
      delay(3500);
      lcd_display("G__D H__LT_","1"); //Question and count to answer
      delay(500);
      lcd_display("G__D H__LT_","2");  //Question and count to answer
      delay(500);
      lcd_display("G__D H__LT_","3");  //Question and count to answer
      delay(500);
      lcd_display("Answer is","GOOD HEALTH");  //Showing answer 
      delay(1000);
      lcd_display("Time to Sleep");  //LCD shows to sleep 
      delay(1000);
      Turn_right();
      delay(650);
    }
    delay(500);
  }
  else{
   brake();
  }
}

void Livingroom_to_Bedroom(){
int Distance1=ultrasound();
speed_forward();
  if(Distance1>=10)
  {
    IR1_Val = digitalRead(A1);
    if(IR1_Val==1) //Now, I am at my defined place. But, still need to go to the bedroom to play sleeping music for a perfect sleep.
    {
      brake();
      Turn_right();
      delay(650);
      speed_forward();
      if(Distance1>=10)
      {
        IR1_Val = digitalRead(A1);
        if(IR1_Val==1)  //Now, I am in the bedroom again.
        {
          brake();
          wait();
          lcd_display("Good Night","Sweet Dreams"); //Display LCD 
          delay(1000);
          sendCommand(CMD_PLAY_W_INDEX, 0, 17);  //MP3 to have a good-night sleep
          delay(4500); 
          lcd_display("Zzzz....");  //LCD to show sleeping-master
          sendCommand(CMD_PLAY_W_INDEX, 0, 2); //Playing sleeping music to get a perfect sleep
          delay(8000);
          lcd.clear();
          lcd_display("Finish My","Tasks");  //LCD shows about the completed all the tasks for a day
          Turn_right(); //Move to turn right to start going back to my defined place
          delay(650);
        }
        delay(500);
      }
      else{
       brake();
      }
    }
  delay(500);
  }
  else{
   brake();
  }
}
int ultrasound(void){
  long pulseDuration; //variable needed by the ultrasound sensor code
  int Distance1;       //Ultrasound distance in cm
  digitalWrite(TrigPin1, LOW);
  delayMicroseconds(1);
  digitalWrite(TrigPin1, HIGH);
  delayMicroseconds(3);
  digitalWrite(TrigPin1, LOW);
  pulseDuration = pulseIn(EchoPin1, HIGH);
  Distance1 = pulseDuration / 58;  //  Convert to cm
  delay(500);
  return(Distance1);
}

void wait(){
  while(digitalRead(hitA))
{ 
 lcd_display("SP-Bot");
 delay(500);
}
}

ISR(PCINT2_vect) 
{
  if (!digitalRead(hitB))
   {sendCommand(CMD_STOP_PLAY, 0, 19);
   delay(1000);
   }  
}

void lcd_display(char text1[], char text2[] = NULL) {
    lcd.clear();
    int i = 8 - (round(strlen(text1)) / 2);
    lcd.setCursor(i,0);
    lcd.print(text1);
    if (text2 != NULL) {
        int j = 9 - (round(strlen(text2)) / 2);
        lcd.setCursor(j,1);
        lcd.print(text2);
    }
}

void alarm(){
  lcd_display("Alarm's ringing");
//  for(int i=0;i<7;i++)// buzzer on
//    { 
//      digitalWrite(buzzer,HIGH);
//      tone(buzzer,100);
//      delay(50);
//      digitalWrite(buzzer,LOW);
//      noTone(buzzer);
//      delay(50); 
//    }
//  noTone(buzzer);
//  delay(200);
}

void reminder(int time)
{
  lcd.clear();
  read_temp ();
  delay(10000);
  if(time==breakfast)
  {
    lcd_display("Breakfast Time");
    sendCommand(CMD_PLAY_W_INDEX, 0, 13);
    delay(5000);
  }
  else if(time == lunch)
  {
    lcd_display("Lunch Time");
    sendCommand(CMD_PLAY_W_INDEX, 0, 6);
    delay(5000);
  }
  else if(time == dinner)
  {
    lcd_display("Dinner Time");
    sendCommand(CMD_PLAY_W_INDEX, 0, 7);
    delay(7000);
  }
}

void read_temp () {
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temp=");
  lcd.println(DHT.temperature);
  lcd.setCursor(10,0);
  lcd.print((char)223);
  lcd.print("C");

  if((DHT.temperature>24)&&(DHT.temperature<28)){
      lcd.setCursor(0,1);
      lcd.print("Normal Temp");
      sendCommand(CMD_PLAY_W_INDEX, 0, 15);
      delay(8000);
  }
  if(DHT.temperature>28){
      lcd.setCursor(0,1);
      lcd.print("DrinkMoreWater");
      sendCommand(CMD_PLAY_W_INDEX, 0, 9);
      delay(6000);
  }
  if(DHT.temperature<24){
      lcd.setCursor(0,1);
      lcd.print("Stay Warm");
      sendCommand(CMD_PLAY_W_INDEX, 0, 8);
      delay(6000);
  }
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
String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}
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

void speed_forward(){
 lcd_display("Forward");
 digitalWrite(left_motorA_IN2, LOW);
 analogWrite(left_motorA_IN1, 90);
 digitalWrite(right_motorB_IN4, LOW);
 analogWrite(right_motorB_IN3, 90); 
}

void brake(){
  lcd_display("Brake");
  digitalWrite(left_motorA_IN1, LOW);
  analogWrite(left_motorA_IN2, LOW);
  digitalWrite(right_motorB_IN4, LOW);
  analogWrite(right_motorB_IN3, LOW);
}

void Turn_right(){
  lcd_display("Turn_Right");
  digitalWrite(left_motorA_IN1, HIGH);
  analogWrite(left_motorA_IN2, 112); //motor A higher speed
  digitalWrite(right_motorB_IN3, LOW);
  analogWrite(right_motorB_IN4, LOW);// motor B lower speed
}
void Turn_left(){
  lcd_display("Turn_Left");
  digitalWrite(left_motorA_IN2, HIGH);
  analogWrite(left_motorA_IN1, 112); //motor A higher speed
  digitalWrite(right_motorB_IN4, LOW);
  analogWrite(right_motorB_IN3, LOW);// motor B lower speed
}

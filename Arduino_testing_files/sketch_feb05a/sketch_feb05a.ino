#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define motor_board_input_pin_IN2 6      //Connect to Arduino pin 6
#define motor_board_input_pin_IN1 9      //Connect to Arduino pin 
#define motor_board_input_pin_IN3 3      //Connect to Arduino pin 6
#define motor_board_input_pin_IN4 5 
void Full_speed_forward(void);
void Full_speed_backward(void);
void From_0_to_100_forward(void);
void From_100_to_0_backward(void);
void brake(void);
void Turn_right(void);
void Turn_left(void);

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t arrow[8] = {  0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

void setup() {
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.createChar(3, heart);
   lcd.createChar(7, arrow);
   pinMode(motor_board_input_pin_IN2, OUTPUT);
  pinMode(motor_board_input_pin_IN1, OUTPUT);
  pinMode(motor_board_input_pin_IN3, OUTPUT);
  pinMode(motor_board_input_pin_IN4, OUTPUT);
}

void loop() {
 /*Full_speed_forward();
 delay(1000); 
 lcd.clear();
 lcd.setCursor(7,0);
 lcd.print("Hi");
 lcd.setCursor(2,1);
 lcd.print("I am SP-Bot");
 lcd.printByte(3);
 brake();
 delay(10000);
 Full_speed_forward();
 delay(1000); 
 lcd.clear();
 lcd.setCursor(1,0);
 lcd.print("Let's Get Back");
 lcd.setCursor(0,1);
 lcd.print("To my History^-^");

 brake();
 delay(10000);
 Full_speed_forward();
 delay(1000);
 lcd.clear();
 lcd.setCursor(1,0);
 lcd.print("Let's Explore");
 lcd.setCursor(0,1);
 lcd.print("my Specialities");

 brake();
 delay(10000);
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("Outcomes ?");
 lcd.setCursor(2,1);
 lcd.print("We'll see =>");
  brake();
 delay(10000);*/
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("It's cold");

 lcd.setCursor(0,1);
 lcd.print("Stay Warm");
 lcd.printByte(3);
 delay(5000);
 lcd.clear();
 delay(2000);
 lcd.setCursor(0,0);
 lcd.print("It's hot");
 lcd.setCursor(0,1);
 lcd.print("Drink more Water");
 lcd.printByte(3);
 delay(5000);
}

void Full_speed_forward(){
  Serial.println("Full speed forward for 2s");
  digitalWrite(motor_board_input_pin_IN1, HIGH);
  digitalWrite(motor_board_input_pin_IN2, LOW); 
  digitalWrite(motor_board_input_pin_IN4, HIGH);
  digitalWrite(motor_board_input_pin_IN3, LOW);
  delay(100);
}
void Turn_right()
{
  digitalWrite(motor_board_input_pin_IN1, HIGH);
  analogWrite(motor_board_input_pin_IN2, 150); //motor A higher speed
  digitalWrite(motor_board_input_pin_IN3, LOW);
  analogWrite(motor_board_input_pin_IN4, 60);// motor B lower speed
  delay(350);
}
void Turn_left()
{
  digitalWrite(motor_board_input_pin_IN1, HIGH);
  analogWrite(motor_board_input_pin_IN2, 60);// motor A lower speed than motor B
  digitalWrite(motor_board_input_pin_IN3, LOW);
  analogWrite(motor_board_input_pin_IN4, 150);
  delay(350);
}
void brake(){
  Serial.println("Brake");
  digitalWrite(motor_board_input_pin_IN1, LOW);
  analogWrite(motor_board_input_pin_IN2, LOW);
  digitalWrite(motor_board_input_pin_IN3, LOW);
  analogWrite(motor_board_input_pin_IN4, LOW);
}

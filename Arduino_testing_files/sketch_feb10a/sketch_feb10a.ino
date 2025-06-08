
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;           // Answer from the MP3.


String sanswer(void);
String sbyte2hex(uint8_t b);

#define CMD_PLAY_W_INDEX  0X03
#define CMD_SEL_DEV       0X09
#define CMD_STOP_PLAY     0X16 

#define DEV_TF            0X02

#define audio_stop_pin 4

void setup() {
  Serial.begin(9600);
  PCICR |= B00000100; // Enable interrupts on PD port (D0 to D7)
  PCMSK2 |= B00000100; // Trigger interrupts on pins D2 and D4
  sendCommand(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
}

void loop() {
  sendCommand(CMD_PLAY_W_INDEX, 0, 1);
  delay(10000);

}

ISR(PCINT2_vect) {
  if (!digitalRead(audio_stop_pin))
   {
    sendCommand(CMD_STOP_PLAY, 0, 1);
    delay(3000);
   }
    
}

void sendCommand(byte command){
  sendCommand(command, 0, 0);
  delay(3000);
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

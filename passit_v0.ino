/* Main program
 *  - Created by Shmulik Debby 27/01/2017
 *  read key from file"mypass.txt"
 *  encrypt and write again.
 *  decrypt and wait for buttom pressed
 *  on button pressed- write string to HID keyboad
 *
 *   * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 8 (for MKRZero SD: SDCARD_SS_PIN)
 * Button digital input pin 2
 * Led digital output pin 7
*/

#include <SPI.h>
#include <SD.h>
#include <AESLib.h>

uint8_t AES_key[] = {'a',0,'b',3,'r',5,'j',2,'h',9,10,'q',12,11,'q',15}; // ***Decryption KEY
File myFile;
String FileName="mypass.txt";
char PasBuf[44];
int buttonPin =2;
int buttonState = 0;         // variable for reading the pushbutton status
// Keyboard define in decimal
#define KEY_A 4 // Keyboard a and A
#define KEY_B 5 // Keyboard b and B
#define KEY_C 6 // Keyboard c and C
#define KEY_D 7 // Keyboard d and D
#define KEY_E 8 // Keyboard e and E
#define KEY_F 9 // Keyboard f and F
#define KEY_G 10 // Keyboard g and G
#define KEY_H 11 // Keyboard h and H
#define KEY_I 12 // Keyboard i and I
#define KEY_J 13 // Keyboard j and J
#define KEY_K 14 // Keyboard k and K
#define KEY_L 15 // Keyboard l and L
#define KEY_M 16 // Keyboard m and M
#define KEY_N 17 // Keyboard n and N
#define KEY_O 18 // Keyboard o and O
#define KEY_P 19 // Keyboard p and P
#define KEY_Q 20 // Keyboard q and Q
#define KEY_R 21 // Keyboard r and R
#define KEY_S 22 // Keyboard s and S
#define KEY_T 23 // Keyboard t and T
#define KEY_U 24 // Keyboard u and U
#define KEY_V 25 // Keyboard v and V
#define KEY_W 26 // Keyboard w and W
#define KEY_X 27 // Keyboard x and X
#define KEY_Y 28 // Keyboard y and Y
#define KEY_Z 29 // Keyboard z and Z

#define KEY_1 30 // Keyboard 1 and !
#define KEY_2 31 // Keyboard 2 and @
#define KEY_3 32 // Keyboard 3 and #
#define KEY_4 33 // Keyboard 4 and $
#define KEY_5 34 // Keyboard 5 and %
#define KEY_6 35 // Keyboard 6 and ^
#define KEY_7 36 // Keyboard 7 and &
#define KEY_8 37 // Keyboard 8 and *
#define KEY_9 38 // Keyboard 9 and (
#define KEY_0 39 // Keyboard 0 and )

#define KEY_ENTER 40 // Keyboard Return (ENTER)
#define KEY_ESC 41 // Keyboard ESCAPE
#define KEY_BACKSPACE 42 // Keyboard DELETE (Backspace)
#define KEY_TAB 43 // Keyboard Tab
#define KEY_SPACE 44 // Keyboard Spacebar
#define KEY_MINUS 45 // Keyboard - and _
#define KEY_EQUAL 46 // Keyboard = and +
#define KEY_LEFTBRACE 47 // Keyboard [ and {
#define KEY_RIGHTBRACE 48 // Keyboard ] and }
#define KEY_BACKSLASH 49 // Keyboard \ and |
#define KEY_HASHTILDE 50 // Keyboard Non-US # and ~
#define KEY_SEMICOLON 51 // Keyboard ; and :
#define KEY_APOSTROPHE 52 // Keyboard ' and "
#define KEY_GRAVE 53 // Keyboard ` and ~
#define KEY_COMMA 54 // Keyboard , and <
#define KEY_DOT 55 // Keyboard . and >
#define KEY_SLASH 56 // Keyboard / and ?
#define KEY_CAPSLOCK 57 // Keyboard Caps Lock
#define KEY_LEFTSHIFT 225 // Keyboard Left Shift

uint8_t buf[8] = { 0 };   /* Keyboard report buffer */
char fileData;
boolean Debug=false; // if debug is true - Sireal messages will interfear the USB key strokes.
boolean EncryptFile=false;
int LedPin=7;

void setup() {
    pinMode(buttonPin, INPUT);
    pinMode(LedPin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 if (Debug)Serial.print("Initializing SD card...");

  if (!SD.begin(8)) {
    if (Debug)Serial.println("initialization failed!");
        digitalWrite(LedPin, LOW);
    return;
  } else{
    if (Debug)Serial.println("initialization done.");
    digitalWrite(LedPin, HIGH);
//read file
 myFile = SD.open(FileName);
  if (myFile) {
    char NewPasBuf[44];
//read file and deside if need to encrypt
//TODO fix this for multiline file
  while (myFile.available()) {
      myFile.read(PasBuf,44);
      if (strlen(PasBuf)!=16){ //check if string encrypt
        if (Debug){Serial.println("Not Encrypted,size : "+(String)strlen(PasBuf));}
        EncryptFile=true;
        aes128_enc_single(AES_key, PasBuf);// encrypt data
        if (Debug) {Serial.println((String)PasBuf); }
      } else {
        if (Debug){Serial.println("Encrypted! size:"+(String)strlen(PasBuf));}
        aes128_dec_single(AES_key, PasBuf);
        if (Debug){Serial.println("Decrypted:" +(String)PasBuf);}
      } //end else
  } //end while
  myFile.close();
  //Open file again and write
  if (EncryptFile) {
   SD.remove(FileName); //delete file
   myFile = SD.open(FileName, FILE_WRITE);
    if (myFile) {
      if (Debug)Serial.println("Writing encrypt data to file.");
      for (int i = 0; i < sizeof(PasBuf); i++) {
                myFile.print((char)PasBuf[i]);
            }
      // close the file:
      myFile.close();
      aes128_dec_single(AES_key, PasBuf); //decrypt string to save in memory
      if(Debug){Serial.println("DONE.");}
      FleshLed3();
    }else {
      if (Debug)Serial.println("error opening file");
    } //end else -error
  } //end if encrypt
  }// end if file
   
 } //end else initize
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) { //button pressed open file
      digitalWrite(LedPin, LOW);
      for(int i=0;i<strlen(PasBuf);i++) {
       if (Debug){
        Serial.write(PasBuf[i]);
        Serial.print(" ");
       }// just for printing
       switch (PasBuf[i]){
        case 'a':
          SendKey(KEY_A);
        break;
        case 'A':
          SendKeyWCaps(KEY_A);
        break;
        case 'b':
          SendKey(KEY_B);
        break;
        case 'B':
          SendKeyWCaps(KEY_B);
        break;
        case 'c':
          SendKey(KEY_C);
        break;
        case 'C':
          SendKeyWCaps(KEY_C);
        break;
        case 'd':
          SendKey(KEY_D);
        break;
        case 'D':
          SendKeyWCaps(KEY_D);
        break;
        case 'e':
          SendKey(KEY_E);
        break;
        case 'E':
          SendKeyWCaps(KEY_E);
        break;
        case 'f':
          SendKey(KEY_F);
        break;
        case 'F':
          SendKeyWCaps(KEY_F);
        break;
        case 'g':
          SendKey(KEY_G);
        break;
        case 'G':
          SendKeyWCaps(KEY_G);
        break;
        case 'h':
          SendKey(KEY_H);
        break;
        case 'H':
          SendKeyWCaps(KEY_H);
        break;
        case 'i':
          SendKey(KEY_I);
        break;
        case 'I':
          SendKeyWCaps(KEY_I);
        break;
        case 'j':
          SendKey(KEY_J);
        break;
        case 'J':
          SendKeyWCaps(KEY_J);
        break;
        case 'k':
          SendKey(KEY_K);
        break;
        case 'K':
          SendKeyWCaps(KEY_K);
        break;
       case 'l':
          SendKey(KEY_L);
        break;
        case 'L':
          SendKeyWCaps(KEY_L);
        break;
       case 'm':
          SendKey(KEY_M);
        break;
        case 'M':
          SendKeyWCaps(KEY_M);
        break;
       case 'n':
          SendKey(KEY_N);
        break;
        case 'N':
          SendKeyWCaps(KEY_N);
        break;
       case 'o':
          SendKey(KEY_O);
        break;
        case 'O':
          SendKeyWCaps(KEY_O);
        break;
       case 'p':
          SendKey(KEY_P);
        break;
        case 'P':
          SendKeyWCaps(KEY_P);
        break;
       case 'q':
          SendKey(KEY_Q);
        break;
        case 'Q':
          SendKeyWCaps(KEY_Q);
        break;
       case 'r':
          SendKey(KEY_R);
        break;
        case 'R':
          SendKeyWCaps(KEY_R);
        break;
       case 's':
          SendKey(KEY_S);
        break;
        case 'S':
          SendKeyWCaps(KEY_S);
        break;
       case 't':
          SendKey(KEY_T);
        break;
        case 'T':
          SendKeyWCaps(KEY_T);
        break;
       case 'u':
          SendKey(KEY_U);
        break;
        case 'U':
          SendKeyWCaps(KEY_U);
        break;
       case 'v':
          SendKey(KEY_V);
        break;
        case 'V':
          SendKeyWCaps(KEY_V);
        break;
       case 'w':
          SendKey(KEY_W);
        break;
        case 'W':
          SendKeyWCaps(KEY_W);
        break;
       case 'x':
          SendKey(KEY_X);
        break;
        case 'X':
          SendKeyWCaps(KEY_X);
        break;
       case 'y':
          SendKey(KEY_Y);
        break;
        case 'Y':
          SendKeyWCaps(KEY_Y);
        break;
       case 'z':
          SendKey(KEY_Z);
        break;
        case 'Z':
          SendKeyWCaps(KEY_Z);
        break;
       case '1':
          SendKey(KEY_1);
        break;
       case '2':
          SendKey(KEY_2);
        break;
       case '3':
          SendKey(KEY_3);
        break;
       case '4':
          SendKey(KEY_4);
        break;
        case '5':
          SendKey(KEY_5);
        break;
       case '6':
          SendKey(KEY_6);
        break;
        case '7':
          SendKey(KEY_7);
        break;
       case '8':
          SendKey(KEY_8);
        break;
        case '9':
          SendKey(KEY_9);
        break;
       case '0':
          SendKey(KEY_0);
        break;
              
       case '!':
          SendKeyWShift(KEY_1);
        break;
       case '@':
          SendKeyWShift(KEY_2);
        break;
       case '#':
          SendKeyWShift(KEY_3);
        break;
       case '$':
          SendKeyWShift(KEY_4);
        break;
        case '%':
          SendKeyWShift(KEY_5);
        break;
       case '^':
          SendKeyWShift(KEY_6);
        break;
        case '&':
          SendKeyWShift(KEY_7);
        break;
       case '*':
          SendKeyWShift(KEY_8);
        break;
        case '(':
          SendKeyWShift(KEY_9);
        break;
       case ')':
          SendKeyWShift(KEY_0);
        break;
       case ' ':
          SendKey(KEY_SPACE);
        break;
        case '-':
          SendKey(KEY_MINUS);
        break;
        case '_':
          SendKeyWShift(KEY_MINUS);
        break;
       case '=':
          SendKey(KEY_EQUAL);
        break;
         case '+':
          SendKeyWShift(KEY_EQUAL);
        break;
       case '[':
          SendKey(KEY_LEFTBRACE);
        break;
         case '{':
          SendKeyWShift(KEY_LEFTBRACE);
        break;
        case ']':
          SendKey(KEY_RIGHTBRACE);
        break;
        case '}':
        SendKeyWShift(KEY_RIGHTBRACE);
        break;
       case '|':
        SendKeyWShift(KEY_BACKSLASH);
        break;
        case ';':
        SendKey(KEY_SEMICOLON);
        break;
        case ':':
        SendKeyWShift(KEY_SEMICOLON);
        break;
        case '"':
        SendKeyWShift(KEY_APOSTROPHE);
        break;
        case '`':
        SendKey(KEY_GRAVE);
        break;
        case '~':
        SendKeyWShift(KEY_GRAVE);
        break;
        case '/':
        SendKey(KEY_SLASH);
        break;
        case '?':
        SendKeyWShift(KEY_SLASH);
        break;
        case '.':
        SendKey(KEY_DOT);
        break;
        case '>':
        SendKeyWShift(KEY_DOT);
        break;
        case ',':
        SendKey(KEY_COMMA);
        break;
        case '<':
        SendKeyWShift(KEY_COMMA);
        break;
        } //end switch
     } //end for loop
     SendKey(KEY_ENTER);
   digitalWrite(LedPin, HIGH);
   // } //end while
 /* } else {
    // if the file didn't open, print an error:
    Serial.println("error opening "+FileName);
  } */
        delay(1000);                  // waits for a second to fix 1 click
  } else {
//Button low
    //Serial.println("Button low");
  }
} //end loop

/** Send key with Shift */
void SendKeyWShift(int MyKey){
  uint8_t bufShift[8] = { 0 };   /* Keyboard report buffer */
      buf[2] = MyKey;
      bufShift[2] = KEY_LEFTSHIFT;
    Serial.write(bufShift, 8);
    Serial.write(buf, 8);
    delay(100);
    releaseKey();
    delay(100);
} //end SendKeyWShift

/** Send key with Caps lock (Upper corse) */
void SendKeyWCaps(int MyKey){
  SendKey(KEY_CAPSLOCK);
  SendKey(MyKey);
  SendKey(KEY_CAPSLOCK);
} // end SendKeyWCaps

/** Send key stroke */
void SendKey(int MyKey){
      buf[2] = MyKey;
    Serial.write(buf, 8);
    delay(100);
    releaseKey();
    delay(100);
} //end SendKey

/** Release key stroke */
void releaseKey()
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key
} //end releaseKey

/** Blink led 3 times*/
void FleshLed3(){
   for (int i = 0; i < 2; i++) {
    digitalWrite(LedPin, LOW);
    delay(500);
    digitalWrite(LedPin, HIGH);
   }
}

// C64 USB Keyboard mod 
// Original by Pyofer 
// See original thread @ 
// http://www.lemon64.com/forum/viewtopic.php?t=55650 
// 
// Modified to Support restore key & US/EU keymaps by dabone. 
// Wiring is as follows 
// 64 Keyboard connector has 20 Pins with a key Pin @ Pin 2. 
// Arduino Pro Micro Pinout 
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png 

// Board type should be Arduino Leonardo (or the Pro mini clones) 


//KeyBoard Arduino Pro Micro 
//  Pin     Pin     Label

//   20       5       2 - SDA
//   19       6       3 - SCL
//   18       7       4 - A6
//   17       8       5
//   16       9       6 - A7
//   15       10      7 - 
//   14       11      8 - A8 
//   13       12      9 - A9 
//   12       13      10 - A10 
//   11       14      16 - MOSI 
//   10       15      14 - MISO 
//   9        16      15 - SCLK 
//   8        17      18 - A0 
//   7        18      19 - A1 
//   6        19      20 - A2 
//   5        20      21 - A3 
//   4        N/C
//   3        1       1 - TX
//   2        N/C
//   1        0       0 - RX


// Keyboard Matrix Now Matches real C64 with one more column.
// Matrix can be found at 
// http://sta.c64.org/cbm64kbdlay.html

// The keyboard scancodes are defined in Keyboard.h
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h


#include <HID.h> 
#include <Keyboard.h>

// Comment out for PROD CODE!!!11
#define DEBUG

int inChar=0;
int keyPos=0;
int digitalread=0;
int keyDown[72];
long lastDebounceTime[72];
int debounceDelay=50;
int shift=0;
int outPinSet=0;
int windowsShift;
int DefaultKBMode=0;                              // Select 0 For Windows Mode On startup or 1 for C64 Mode
int USKeyboard=1;                                 // Select 1 for US Keyboard or 0 For EU
int HybridKeyboard=1;                             // Select 0 for normal or 1 for the left shift key allowing all f keys and cursor keys in windows mode. (Also has a shifted restore key)


char keyMapUS[216]={

212,176,215,200,194,196,198,217,0,                //  Del Return LR F7 F1 F3 F5 UD Null
51,119,97,52,122,115,101,129,0,                   //  3 W A 4 Z S E LSHFT Null
53,114,100,54,99,102,116,120,0,                   //  5 R D 6 C F T X Null
55,121,103,56,98,104,117,118,0,                   //  7 Y G 8 B H U V Null
57,105,106,48,109,107,111,110,0,                  //  9 I J Zero M K O N Null
43,112,108,45,46,58,64,44,0,                      //  + P L - . : @ , Null     (US Keyboard)
35,42,59,210,133,61,211,47,205,                   //  Pound * ; Home RSHFT = Pi / Restore
49,178,128,50,32,135,113,177,0,                   //  1 BS CTRL 2 SPC C= Q RunStop Null

209,176,216,201,195,197,199,218,0,                //  Del Return LR F8 F2 F4 F6 UD Null
92,87,65,36,90,83,69,129,0,                       //  # W A $ Z S E LSHFT Null
37,82,68,38,67,70,84,88,0,                        //  % R D & C F T X Null
39,89,71,40,66,72,85,86,0,                        //  ' Y G ( B H U V Null
41,73,74,48,77,75,79,78,0,                        //  ) I J Zero M K O N Null
43,80,76,95,62,91,96,60,0,                        //  + P L - > : @ < Null
35,42,93,210,133,61,214,63,205,                   //  Pound * ; Home RSHFT = Pi ? Restore
33,178,128,34,32,135,81,177,0,                    //  ! BS CTRL " SPC C= Q RS Null     (US Keyboard)

178,176,215,200,194,196,198,217,0,                //  Del Return LR F7 F1 F3 F5 UD Null
51,119,97,52,122,115,101,129,0,                   //  3 W A 4 Z S E LSHFT Null
53,114,100,54,99,102,116,120,0,                   //  5 R D 6 C F T X Null
55,121,103,56,98,104,117,118,0,                   //  7 Y G 8 B H U V Null
57,105,106,48,109,107,111,110,0,                  //  9 I J Zero M K O N Null
45,112,108,61,46,59,91,44,0,                      //  + P L - . : @ , Null
209,93,39,210,133,92,212,47,205,                  //  Pound * ; Home RSHFT = Pi / Restore
49,223,9,50,32,128,113,177,0,                     //  1 BS CTRL 2 SPC C= Q RunStop Null

};

char keyMapEU[216]={
212,176,215,200,194,196,198,217,0,                //  Del Return LR F7 F1 F3 F5 UD Null
51,119,97,52,122,115,101,129,0,                   //  3 W A 4 Z S E LSHFT Null
53,114,100,54,99,102,116,120,0,                   //  5 R D 6 C F T X Null
55,121,103,56,98,104,117,118,0,                   //  7 Y G 8 B H U V Null
57,105,106,48,109,107,111,110,0,                  //  9 I J Zero M K O N Null
43,112,108,45,46,58,34,44,0,                      //  + P L - . : @ , Null     (EU Keyboard)
35,42,59,210,133,61,211,47,205,                   //  Pound * ; Home RSHFT = Pi / Restore
49,178,128,50,32,135,113,177,0,                   //  1 BS CTRL 2 SPC C= Q RunStop Null

209,176,216,201,195,197,199,218,0,                //  Del Return LR F8 F2 F4 F6 UD Null
92,87,65,36,90,83,69,129,0,                       //  # W A $ Z S E LSHFT Null
37,82,68,38,67,70,84,88,0,                        //  % R D & C F T X Null
39,89,71,40,66,72,85,86,0,                        //  ' Y G ( B H U V Null
41,73,74,48,77,75,79,78,0,                        //  ) I J Zero M K O N Null
43,80,76,95,62,91,96,60,0,                        //  + P L - > : @ < Null
35,42,93,210,133,61,214,63,205,                   //  Pound * ; Home RSHFT = Pi ? Restore
33,178,128,64,32,135,81,177,0,                    //  ! BS CTRL " SPC C= Q RS Null     (EU Keyboard)

178,176,215,200,194,196,198,217,0,                //  Del Return LR F7 F1 F3 F5 UD Null
51,119,97,52,122,115,101,129,0,                   //  3 W A 4 Z S E LSHFT Null
53,114,100,54,99,102,116,120,0,                   //  5 R D 6 C F T X Null
55,121,103,56,98,104,117,118,0,                   //  7 Y G 8 B H U V Null
57,105,106,48,109,107,111,110,0,                  //  9 I J Zero M K O N Null
45,112,108,61,46,59,91,44,0,                      //  + P L - . : @ , Null
209,93,39,210,133,92,212,47,205,                  //  Pound * ; Home RSHFT = Pi / Restore
49,223,9,50,32,128,113,177,0,                     //  1 BS CTRL 2 SPC C= Q RunStop Null

};

char Hybridkeys[7] {                              // Hybrid Keys. These are the shifted values.
// LR           F8      F2      F4      F6      UD            Restore
KEY_LEFT_ARROW, KEY_F8, KEY_F2, KEY_F4, KEY_F6, KEY_UP_ARROW, KEY_F12,                      
};

void setup() {
  #ifndef DEBUG
  Keyboard.begin();// initialize control over the keyboard:
  #endif
  
  for (int i=0; i<64; i++) keyDown[i]=0; // Set all keys as up
  
  pinMode(2,OUTPUT);  // configure inputs and outputs
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(0,OUTPUT);

  pinMode(10,INPUT_PULLUP); // use internal pullups to hold pins high
  pinMode(16,INPUT_PULLUP);
  pinMode(15,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);
  
  digitalWrite(2,LOW);  // start with one active pin to detect '1'
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(0,HIGH);
  
  if (DefaultKBMode==1)
    if (!digitalRead(10)) windowsShift=1; else windowsShift=0; // detect if '1' is held on power up to swap mode
  
  if (DefaultKBMode==0)
    if (!digitalRead(10)) windowsShift=0; else windowsShift=1; // detect if '1' is held on power up to swap mod
  
  #ifdef DEBUG
    Serial.begin(9600); 
    while (!Serial) ; // Wait for the serial monitor
    Serial.println("Setup done");
  #endif
 }

void loop() // main keyboard scanning loop
{
  int outPinForRow[] = {9, 3, 4, 5, 6, 7, 8, 2, 0};
  int inPinForColumn[] = {10, 16, 14, A3, A0, A1, A2, 15, 1};
  for (int row = 0; row < 9; row++) {
    pinMode(2,INPUT);  // set unused (all) outputs to input to avoid ghosting
    pinMode(3,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    pinMode(9,INPUT);
    pinMode(0,INPUT);

    pinMode(outPinForRow[row], OUTPUT);
    digitalWrite(outPinForRow[row], LOW);
    outPinSet = outPinForRow[row];
    
    for (int column = 0; column < 9; column++) {
      keyPos = column + (row * 9); // calculate character map position
      if (row == 8) keyPos=70; // Evil Restore hack
      
      if (USKeyboard==1) {
        if (!windowsShift) inChar=keyMapUS[keyPos+shift]; // work out which key it is from the map and shift if needed
        else inChar=keyMapUS[keyPos+144];  // use "windows" keymap where shift is passed through
      }
      
      if (USKeyboard==0) {
        if (!windowsShift) inChar=keyMapEU[keyPos+shift]; // work out which key it is from the map and shift if needed
        else inChar=keyMapEU[keyPos+144];  // use "windows" keymap where shift is passed through
      }
      
      digitalread=1-digitalRead(inPinForColumn[column]);
 
      if (HybridKeyboard==1) {
        if ((millis()-lastDebounceTime[keyPos])>debounceDelay) {// debounce for each key individually
          if (digitalread==1 && keyDown[keyPos]==0) {// if a key is pressed and wasn't already down
            keyDown[keyPos]=inChar;        // put the right character in the keydown array
            if (keyDown[16]&&keyDown[2])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[0];}
            if (keyDown[16]&&keyDown[3])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[1];}
            if (keyDown[16]&&keyDown[4])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[2];}
            if (keyDown[16]&&keyDown[5])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[3];}
            if (keyDown[16]&&keyDown[6])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[4];}
            if (keyDown[16]&&keyDown[7])  {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[5];}
            if (keyDown[16]&&keyDown[62]) {releaseKey (keyDown[16]);keyDown[keyPos]=Hybridkeys[6];}
        
            if ((keyPos!=16&&keyPos!=58)||windowsShift==1) {// is it not-shift or in windows mode?
              // if so pass the key through
              lastDebounceTime[keyPos] = millis(); // reset the debounce delay
              pressKey(keyDown[keyPos]);    // pass the keypress to windows
            }
            else { lastDebounceTime[keyPos]=millis(); shift=72; } // reset keybounce delay and mark as shift press
          }
          if (digitalread==0 && keyDown[keyPos]!=0) {// key is up and a character is stored in the keydown position
            if ((keyPos!=16&&keyPos!=58)||windowsShift==1) {// not-shift or windows mode
              lastDebounceTime[keyPos] = millis();  // reset keybounce delay
              releaseKey(keyDown[keyPos]);    // pass key release to windows
            }
            else { lastDebounceTime[keyPos]=millis(); shift=0; } // reset keybounce delay and mark as un-shifted
            keyDown[keyPos]=0; // set keydown array position as up
          }
        }
      }
      if (HybridKeyboard==0) {
        if ((millis()-lastDebounceTime[keyPos])>debounceDelay) { // debounce for each key individually
          if (digitalread==1 && keyDown[keyPos]==0) {// if a key is pressed and wasn't already down
            keyDown[keyPos]=inChar;        // put the right character in the keydown array
            if ((keyPos!=16&&keyPos!=58)||windowsShift==1) {// is it not-shift or in windows mode?
              // if so pass the key through
              lastDebounceTime[keyPos] = millis(); // reset the debounce delay
              pressKey(keyDown[keyPos]);    // pass the keypress to windows
            }
            else { lastDebounceTime[keyPos]=millis(); shift=72; } // reset keybounce delay and mark as shift press
          }
          if (digitalread==0 && keyDown[keyPos]!=0) { // key is up and a character is stored in the keydown position
            if ((keyPos!=16&&keyPos!=58)||windowsShift==1) {// not-shift or windows mode
              lastDebounceTime[keyPos] = millis();  // reset keybounce delay
              releaseKey(keyDown[keyPos]);    // pass key release to windows
            }
            else { lastDebounceTime[keyPos]=millis(); shift=0; } // reset keybounce delay and mark as un-shifted
            keyDown[keyPos]=0; // set keydown array position as up
          }
        }
      }
    }
  digitalWrite(outPinSet,HIGH); // set output back to high
  }
}

void pressKey(char key) {
  #ifdef DEBUG
    Serial.print("Press: ");
    printChar(key);
  #else
    Keyboard.press(key);
  #endif 
}

void releaseKey(char key) {
  #ifdef DEBUG
    Serial.print("Release: ");
    printChar(key);
  #else
    Keyboard.release(key);    // pass key release to windows
  #endif
}

void printChar(char key) {
  if (key < ' ') {
    Serial.print('\\');
    Serial.println((unsigned char)key);
  } else {
    Serial.println(key);
  }
}


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
// We'll represent the matrix as a 9x9 matrix, where row 9 / col 9 is RESTORE.

// The keyboard scancodes are defined in Keyboard.h
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h

#include <HID.h>
#include <Keyboard.h>
#include "matrix.h"

// Comment out for prod code
//#define DEBUG

int mode = 0;
char keyDown[LAYER_SIZE];
long lastDebounceTime[LAYER_SIZE];
int debounceDelay = 50;

int outPinForRow[] = {9, 3, 4, 5, 6, 7, 8, 2};
int inPinForColumn[] = {10, 16, 14, A3, A0, A1, A2, 15};

// Restore's pins are handled separately
#define RESTORE_PIN_OUT 0
#define RESTORE_PIN_IN 1

void setup() {
#ifndef DEBUG
  Keyboard.begin();// initialize control over the keyboard:
#endif

  for (int i = 0; i < LAYER_SIZE; i++) keyDown[i] = 0; // Set all keys as up

  pinMode(2, OUTPUT); // configure inputs and outputs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(0, OUTPUT); // Restore

  pinMode(10, INPUT_PULLUP); // use internal pullups to hold pins high
  pinMode(16, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP); // Restore

#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) ; // Wait for the serial monitor
  Serial.println("Setup done");
#endif
}

void loop()
{
  bool isModifierDown = isRestoreDown();
  int modifierShift = isModifierDown ? LAYER_SIZE : 0;

  for (int row = 0; row < 8; row++) {
    // set unused (all) outputs to input to avoid ghosting
    setAllOutPinsToIn();

    pinMode(outPinForRow[row], OUTPUT);
    digitalWrite(outPinForRow[row], LOW);

    for (int column = 0; column < 8; column++) {
      int keyPos = column + (row * 8); // calculate character map position
      bool columnOn = digitalRead(inPinForColumn[column]) == 0;

      if ((millis() - lastDebounceTime[keyPos]) > debounceDelay) {
        if (columnOn && keyDown[keyPos] == 0) {
          keyDown[keyPos] = layers[keyPos + LAYER_SIZE * 2 * mode + modifierShift];
          if (!isModeChange(isModifierDown)) {
#ifdef DEBUG
            Serial.print("Key down at ");
            Serial.println(keyPos);
            Serial.print("With modifier ");
            Serial.println(isModifierDown);
#endif
            pressKey(keyDown[keyPos]);
          }
          lastDebounceTime[keyPos] = millis();
        }

        if (!columnOn && keyDown[keyPos] != 0) { // key is up and a character is stored in the keydown position
          releaseKey(keyDown[keyPos]);    // pass key release to windows
          keyDown[keyPos] = 0; // set keydown array position as up
          lastDebounceTime[keyPos] = millis();
        }
      }
    }
    digitalWrite(outPinForRow[row], HIGH); // set output back to high
  }
}

bool isRestoreDown() {
  // No need to worry about ghosting, as this key has its own line.
  digitalWrite(RESTORE_PIN_OUT, LOW);
  bool restoreDown = digitalRead(RESTORE_PIN_IN) == 0;
  digitalWrite(RESTORE_PIN_OUT, HIGH);
  return restoreDown;
}

void setAllOutPinsToIn() {
  for (int r = 0; r < 8; r++) pinMode(outPinForRow[r], INPUT);
}

bool isModeChange(bool isModifierDown) {
  bool modeChanged = false;
  if (isModifierDown && keyDown[POS_COMMODORE])
    if (keyDown[POS_F1]) {
      mode = 0;
      modeChanged = true;
    } else if (keyDown[POS_F3]) {
      mode = 1;
      modeChanged = true;
    }
#ifdef DEBUG
  if (modeChanged) {
    Serial.print("Mode changed to ");
    Serial.println(mode);
  }
#endif
  return modeChanged;
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


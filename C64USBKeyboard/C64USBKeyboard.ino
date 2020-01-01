// C64 USB Keyboard mod
// Original by Pyofer
// See original thread @
// http://www.lemon64.com/forum/viewtopic.php?t=55650

// Modified to Support restore key & US/EU keymaps by dabone.
// 64 Keyboard connector has 20 Pins with a key Pin @ Pin 2.
// Arduino Pro Micro Pinout
// https://cdn.sparkfun.com/assets/9/c/3/c/4/523a1765757b7f5c6e8b4567.png
// Board type should be Arduino Leonardo (or the Pro mini clones)

// Matrix can be found at
// http://sta.c64.org/cbm64kbdlay.html
// The keyboard scancodes are defined in Keyboard.h
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h

// Press RESTORE + C= + F1/F3 keys to switch between modes


#include <HID.h>
#include <Keyboard.h>
#include "matrix.h"

// Comment out for prod code
// Debug mode prints the keys & debug info to serial
//#define DEBUG

int mode = 1;
char keyDown[MATRIX_SIZE];
long lastDebounceTime[MATRIX_SIZE];
int debounceDelay = 50;

// C64 pin   1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20
// Arduino   TX0  N/C  RX1  N/C  2    3    4    5    6    7    8    9    A3   A2   A1   A0   15   14   16   10
// Matrix  |-----Restore-------|---------------Inputs-----------------|------------Outputs---------------------|
#define OUTPUT_COUNT 8
#define INPUT_COUNT 8
int outputPins[] = {A3, A2, A1, A0, 15, 14, 16, 10};  // Technically, these are columns (as per schematics)
int inputPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Technically, these are rows.

// Restore's pins are handled separately
#define RESTORE_PIN_OUT 1
#define RESTORE_PIN_IN 0

void setup() {
#ifndef DEBUG
  Keyboard.begin(); // Initialize control over the keyboard:
#endif

  for (int i = 0; i < MATRIX_SIZE; i++) keyDown[i] = 0; // Set all keys as up

  for (int i = 0; i < OUTPUT_COUNT; i++) pinMode(outputPins[i], OUTPUT);
  pinMode(RESTORE_PIN_OUT, OUTPUT);

  for (int i = 0; i < INPUT_COUNT; i++) pinMode(inputPins[i], INPUT_PULLUP);
  pinMode(RESTORE_PIN_IN, INPUT_PULLUP);

#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) ; // Wait for the serial monitor
  Serial.println("Setup done");
#endif
}

void loop() {
  bool isModifierDown = isRestoreDown();
  int modifierShift = isModifierDown ? MATRIX_SIZE : 0;

  for (int outPin = 0; outPin < OUTPUT_COUNT; outPin++) {
    // set unused (all) outputs to input to avoid ghosting
    setAllOutPinsToIn();

    pinMode(outputPins[outPin], OUTPUT);
    digitalWrite(outputPins[outPin], LOW);

    for (int inPin = 0; inPin < INPUT_COUNT; inPin++) {
      int keyPos = outPin + (inPin * OUTPUT_COUNT); // calculate character map position
      bool isColumnOn = digitalRead(inputPins[inPin]) == 0;

      if ((millis() - lastDebounceTime[keyPos]) > debounceDelay) {
        if (isColumnOn && keyDown[keyPos] == 0) {
          keyDown[keyPos] = layers[keyPos + MATRIX_SIZE * 2 * mode + modifierShift];
          if (!hasModeChanged(isModifierDown)) {
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

        if (!isColumnOn && keyDown[keyPos] != 0) { // key is up and a character is stored in the keydown position
          releaseKey(keyDown[keyPos]);    // pass key release to windows
          keyDown[keyPos] = 0; // set keydown array position as up
          lastDebounceTime[keyPos] = millis();
        }
      }
    }
    digitalWrite(outputPins[outPin], HIGH); // set output back to high
  }
}

bool isRestoreDown() {
  // No need to worry about ghosting, as this key has its own line.
  digitalWrite(RESTORE_PIN_OUT, LOW);
  bool restoreDown = digitalRead(RESTORE_PIN_IN) == 0;
  digitalWrite(RESTORE_PIN_OUT, HIGH);
  return restoreDown;
}

inline void setAllOutPinsToIn() {
  for (int r = 0; r < 8; r++) pinMode(outputPins[r], INPUT);
}

bool hasModeChanged(bool isModifierDown) {
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


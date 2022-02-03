#include <Arduino.h>

#ifndef LIGHTBARHEADERFILE_H
#define LIGHTBARHEADERFILE_H

// Types
struct AHTValues
{
  String temp;
  String humidity;
};

// Fn Prototypes
void fade(void);
void handleButton(void);
void handlePot(void);
void handleBT(void);
void getAHTVals(AHTValues *ahtVals);
void flushIncoming(void);

// Pins
const int RX_PIN = 2;
const int TX_PIN = 3;
const int POT_PIN = A0;
const int BTN_PIN = 4;
const int LIGHT_CTRL_PIN = 5;
const int SDA_PIN = A4;
const int SCL_PIN = A5;

// Potentiometer Vars
int potValue = -1;
boolean lightOn = true;
int currentBrightness;
int fadeJump = 5;

// Button Vars
const int DEBOUNCE_DELAY = 50;
int lastSteadyState = LOW;      // the previous steady state from the input pin
int lastFlickerableState = LOW; // the previous flickerable state from the input pin
int currentState = -1;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

// AHT Vars
AHTValues ahtVals;

// Bluetooth vars
const int BT_HEADER = 0xFC;
enum BT_COMMAND {
  SET_POWER = 0x0,
  SET_BRIGHTNESS = 0x1,
  GET_POWER = 0x2,
  GET_BRIGHTNESS = 0x3,
  GET_AHT_VALUES = 0x4,
};

#endif
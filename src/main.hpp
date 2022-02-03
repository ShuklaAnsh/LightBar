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

// Button Vars
const int DEBOUNCE_DELAY = 50;
int lastSteadyState = LOW;      // the previous steady state from the input pin
int lastFlickerableState = LOW; // the previous flickerable state from the input pin
int currentState = -1;
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled

// AHT Vars
AHTValues ahtVals;

#endif
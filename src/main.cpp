#include "main.hpp"
#include <SoftwareSerial.h>
#include <Adafruit_AHTX0.h>

// Sensors + Comms
SoftwareSerial BTSerial(RX_PIN, TX_PIN);
Adafruit_AHTX0 aht;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LIGHT_CTRL_PIN, OUTPUT);
  currentBrightness = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  analogWrite(LIGHT_CTRL_PIN, currentBrightness);
  if (!aht.begin())
  {
    Serial.println("AHT not found");
  }
}

void loop()
{
  handleButton();
  handlePot();
  handleBT();
}

void fade()
{
  if (lightOn)
  {
    for (int fadeValue = 0; fadeValue <= currentBrightness - fadeJump; fadeValue += fadeJump)
    {
      analogWrite(LIGHT_CTRL_PIN, fadeValue);
      delay(5);
    }
    analogWrite(LIGHT_CTRL_PIN, currentBrightness);
  }
  else
  {
    for (int fadeValue = currentBrightness; fadeValue >= fadeJump; fadeValue -= fadeJump)
    {
      analogWrite(LIGHT_CTRL_PIN, fadeValue);
      delay(5);
    }
    analogWrite(LIGHT_CTRL_PIN, 0);
  }
}

void handleButton()
{
  // Debounce code: https://arduinogetstarted.com/tutorials/arduino-button-debounce
  // Debounce button read
  currentState = digitalRead(BTN_PIN);
  if (currentState != lastFlickerableState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
  {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (lastSteadyState == HIGH && currentState == LOW)
    {
      // Button pressed in
    }
    else if (lastSteadyState == LOW && currentState == HIGH)
    {
      // Button released
      if (lightOn)
      {
        lightOn = false;
      }
      else
      {
        lightOn = true;
      }
      fade();
    }

    // save the the last steady state
    lastSteadyState = currentState;
  }
}

void handlePot()
{
  int newPotValue = analogRead(POT_PIN);
  if (abs(potValue - newPotValue) > 5)
  {
    potValue = newPotValue;
    currentBrightness = map(potValue, 0, 1023, 0, 255);
    if (lightOn)
    {
      analogWrite(LIGHT_CTRL_PIN, currentBrightness);
    }
  }
}

void handleBT()
{
  if (BTSerial.available() == 3)
  {
    int header = BTSerial.read();
    if (header != 0xFC) flushIncoming();
    int command = BTSerial.read();
    int value = BTSerial.read();

    switch (command)
    {
    case 0x0:
      Serial.println("\nSET POWER");
      Serial.println(value);
      if (value == 0)
      {
        lightOn = false;
      }
      else
      {
        lightOn = true;
      }
      fade();
      BTSerial.print("P:");
      BTSerial.println(lightOn);
      break;

    case 0x1:
      Serial.println("\nSET BRIGHTNESS");
      Serial.println(value);
      currentBrightness = value;
      analogWrite(LIGHT_CTRL_PIN, currentBrightness);
      BTSerial.print("B:");
      BTSerial.println(currentBrightness);
      break;

    case 0x2:
      Serial.println("\nGET POWER");
      BTSerial.print("P:");
      BTSerial.println(lightOn);
      break;

    case 0x3:
      Serial.println("\nGET BRIGHTNESS");
      BTSerial.print("B:");
      BTSerial.println(currentBrightness);
      break;

    case 0x4:
      getAHTVals(&ahtVals);
      Serial.println("\nGET AHT VALUES");
      Serial.print("Temperature: ");
      Serial.print(ahtVals.temp);
      Serial.println(" degrees C");
      Serial.print("Humidity: ");
      Serial.print(ahtVals.humidity);
      Serial.println("% rH");

      BTSerial.print("T:");
      BTSerial.println(ahtVals.temp);
      BTSerial.print("H:");
      BTSerial.println(ahtVals.humidity);
      break;

    default:
      break;
    }
  }

}

void getAHTVals(AHTValues *ahtVals)
{
  sensors_event_t humidity_sensor, temp_sensor;
  aht.getEvent(&humidity_sensor, &temp_sensor);
  ahtVals->humidity = String(humidity_sensor.relative_humidity);
  ahtVals->temp = String(temp_sensor.temperature);
}

void flushIncoming() 
{
  while (BTSerial.available()) BTSerial.read();
}

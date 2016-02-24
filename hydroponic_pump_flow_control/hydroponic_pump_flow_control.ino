/*
  Hydroponic pumping control the speed of the pump by potential meter.
*/

#include <Wire.h>
#include "rgb_lcd.h"
#include <Time.h>
#include <TimeAlarms.h>

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 121;
const int colorB = 0;

int potPin = A7;
int pumpPin = 3;
int waterLevelPin = 4;
int potValue;
int pumpSpeed;
long unsigned onTime = 0;
long unsigned onInterval = 300000;  //pump on duration 5 minutes (5 * 60 * 1000 ms)
bool pumpOn = false;

void setup()
{
  Serial.begin(9600);

  setTime(13, 00, 0, 17, 02, 16); // set time to Saturday 8:29:00am Jan 1 2011
  Alarm.timerRepeat(900, Repeats);            // timer for every 15 minutes
  Alarm.timerOnce(5, OnceOnly);             // called once after 10 seconds

  pinMode(pumpPin, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  pinMode(waterLevelPin, INPUT);
}

void loop()
{
  lcd.clear();
  digitalClockDisplay();

  if (pumpOn && ((millis() - onTime) < onInterval) && !isExposedToWater())
  {
    lcd.setCursor(0, 1);
    lcd.print("Pump On ");
    // print the number of seconds since reset:
    potValue = analogRead(potPin);
    potValue = map(potValue, 0, 1024, 44, 200);
    if (potValue >= 44 && potValue <= 200)
    {
      pumpSpeed = potValue;
    }
    lcd.print(pumpSpeed);
    analogWrite(pumpPin, pumpSpeed);
  } else if ((pumpOn && (millis() - onTime) > onInterval) || isExposedToWater())
  {
    pumpOn = false;
    lcd.setCursor(0, 1);
    lcd.print("Pump Off");
    analogWrite(pumpPin, 0);
  }

  Alarm.delay(1000);
}

// functions to be called when an alarm triggers:

void Repeats() {
  pumpOn = true;
  onTime = millis();
}

void OnceOnly() {
  Serial.println("This timer only triggers once");
  pumpOn = true;
  onTime = millis();
}

void digitalClockDisplay()
{
  lcd.setCursor(0, 0);

  // digital clock display of the time
  Serial.print(hour());
  lcd.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits)
{
  Serial.print(":");
  lcd.print(":");
  if (digits < 10)
  {
    Serial.print('0');
    lcd.print('0');
  }

  Serial.print(digits);
  lcd.print(digits);
}

/*Function: Determine whether the sensor is exposed to the water    */
/*Parameter:-void                                 */
/*Return: -boolean,if it is exposed to the water,it will return true. */
boolean isExposedToWater()
{
  if(digitalRead(waterLevelPin) == LOW)
    return true;
  else return false;
}


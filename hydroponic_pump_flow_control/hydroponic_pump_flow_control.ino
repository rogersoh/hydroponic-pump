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

int pump1Pin = 3;
int pump2Pin = 4;
long unsigned onTime = 0;
long unsigned onInterval = 300000;  //pump on duration 5 minutes (5 * 60 * 1000 ms)
bool pump1on = false;
bool pump2on = false;

void setup()
{
  Serial.begin(9600);

  setTime(13, 00, 0, 17, 02, 16); // set time to Saturday 8:29:00am Jan 1 2011
  Alarm.timerRepeat(900, Repeats);            // timer for every 15 minutes
  Alarm.timerOnce(5, OnceOnly);             // called once after 5 seconds

  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
}

void loop()
{
  lcd.clear();
  digitalClockDisplay();

  if (pump1on && ((millis() - onTime) < onInterval) )
  {
    lcd.setCursor(0, 1);
    lcd.print("Pump 1 ON ");
    digitalWrite(pump1Pin, HIGH);
  } else if ((pump1on && (millis() - onTime) > onInterval))
  {
    pump1on = false;
    pump2on = true;
    digitalWrite(pump1Pin, LOW);
    onTime = millis();
  }

  if (pump2on && ((millis() - onTime) < onInterval) )
  {
    lcd.setCursor(0, 1);
    lcd.print("Pump 2 ON ");
    digitalWrite(pump2Pin, HIGH);
  } else if ((pump2on && (millis() - onTime) > onInterval))
  {
    pump2on = false;
    digitalWrite(pump2Pin, LOW);
  }

  Alarm.delay(1000);
}

// functions to be called when an alarm triggers:

void Repeats() {
  pump1on = true;
  onTime = millis();
}

void OnceOnly() {
  Serial.println("This timer only triggers once");
  pump1on = true;
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




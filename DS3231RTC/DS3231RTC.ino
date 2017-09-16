/*
  DS3231_test.pde
  Eric Ayars
  4/11

  Test/demo of read routines for a DS3231 RTC.

  Turn on the serial monitor after loading this to check if things are
  working as they should.

*/

#include <DS3231.h>
#include <Wire.h>


DS3231 Clock;
bool h12;
bool PM;
bool Century=false;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

void setup() {
  Wire.begin();
  Serial.begin(9600);
//  setClock();
}

void setClock() {
  Clock.setClockMode(false);
  Clock.setSecond(0);
  Clock.setMinute(37);
  Clock.setHour(21);
  Clock.setDoW(5);
  Clock.setDate(17);
  Clock.setMonth(7);
  Clock.setYear(17);
  
}

void loop() {
  Serial.print("20");
  Serial.print(Clock.getYear());
  Serial.print('-');
  Serial.print(Clock.getMonth(Century));
  Serial.print('-');
  Serial.print(Clock.getDate());
  Serial.print(',');
  // and the day of the week
//  Serial.print(Clock.getDoW(), DEC);
//  Serial.print('\n');
  // Finally the hour, minute, and second
  Serial.print(Clock.getHour(h12, PM));
  Serial.print(':');
  Serial.print(Clock.getMinute());
  Serial.print(':');
  Serial.print(Clock.getSecond());

  // Display the temperature
  Serial.print(",");
  Serial.print(Clock.getTemperature(), 2);
  Serial.print(',');
  // Tell whether the time is (likely to be) valid
  if (Clock.oscillatorCheck()) {
    Serial.print("osc:OK");
  } else {
    Serial.print("osc:FUCK");
  }
  
//  Serial.print("----------------");
  Serial.print('\n');
  delay(1000);
}

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
#include <SPI.h>
#include <SD.h>



const int chipSelect = 4;

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
Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  analogReference(EXTERNAL);
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
  String dataString = "";
  
  dataString += "20";
  dataString += Clock.getYear();
  dataString += '-';
  dataString += Clock.getMonth(Century);
  dataString += '-';
  dataString += Clock.getDate();
  dataString += ',';
  // Finally the hour, minute, and second
  dataString += Clock.getHour(h12, PM);
  dataString += ':';
  dataString += Clock.getMinute();
  dataString += ':';
  dataString += Clock.getSecond();

  // Display the temperature
  dataString += ",";
  dataString += Clock.getTemperature();
  dataString += ',';
  // Tell whether the time is (likely to be) valid
  if (Clock.oscillatorCheck()) {
    dataString += "osc:OK";
  } else {
    dataString += "osc:FUCK";
  }

  dataString += ',';
  dataString += analogRead(A0);


 String filename = "20";
 filename += Clock.getYear();
 filename += Clock.getMonth(Century);
 filename += Clock.getDate();
 filename += '.';
 filename += "csv";
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(filename, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();        
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening: ");
    Serial.println(filename);
  }
  Serial.println(dataString);
  delay(1000);
}

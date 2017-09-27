
#include <DS3231.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "accelerometer.h"


const int chipSelect = 4;

DS3231 Clock;
bool h12;
bool PM;
bool Century = false;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
boolean gotTrouble = false;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  //  setClock();
  analogReference(EXTERNAL);
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    gotTrouble = true;
    return;
  }

  Serial.println("card initialized.");
  accelerometer();
}


void loop() {
  if(gotTrouble) return;
  
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

  
  // ----------------------------------------
  accRead();

  dataString += ",";
  dataString += String(value_x, 7);
  dataString += ",";
  dataString += String(value_y, 7);
  dataString += ",";
  dataString += String(value_z, 7);
  
  // ----------------------------------------


  // uso un archivo por dia
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
    gotTrouble = true;
  }
  Serial.println(dataString);
  
  delay(100);
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


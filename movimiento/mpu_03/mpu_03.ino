// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added seamless Fastwire support
//                 - added note about gyro calibration
//      2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//      2012-06-20 - improved FIFO overflow handling and simplified read process
//      2012-06-19 - completely rearranged DMP initialization code and simplification
//      2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//      2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//      2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                 - add 3D math helper file to DMP6 example sketch
//                 - add Euler output and Yaw/Pitch/Roll output formats
//      2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//      2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//      2012-05-30 - basic DMP initialization working

/* ============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2012 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

/*
 * ===============================================
   sensor de copa para bruno
   usa MPU6050 + nodemcu
   version 2: all data sobre tcp (wifi)
   ===============================================
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>                // para crear los mensajes osc. https://github.com/CNMAT/OSC

ESP8266WiFiMulti WiFiMulti;

WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
const IPAddress destIp(192, 168, 0, 3); // remote IP of the target device
const unsigned int destPort = 12000;    // remote port of the target device where the NodeMCU sends OSC to
const unsigned int localPort = 8000;   // local port to listen for UDP packets at the NodeMCU (another device must send OSC messages to this port)



#include "I2Cdev.h" //https://github.com/jrowberg/i2cdevlib
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif



// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
   ========================================================================= */
#define INTERRUPT_PIN 12  //D6
// PINOUT https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h#L37-L59
//
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion orientation;           // [w, x, y, z]         quaternion container
VectorInt16 raccel;         // [x, y, z]            accel sensor measurements raw
VectorInt16 laccel;     // [x, y, z]            gravity-free accel sensor measurements local
VectorInt16 waccel;    // [x, y, z]            world-frame accel sensor measurements global
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int32_t gyro[3]; 

// debuggin
#include "configs.h"




// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  setupWifi();

  // Como estoy usando un nodemcu tengo que configurar los pines correctamente porque no corresponden con el pinout de arduino
  // i2cdevlib hace:
  //#define PIN_WIRE_SDA (4)
  //#define PIN_WIRE_SCL (5)
  //static const uint8_t SDA = PIN_WIRE_SDA;
  //static const uint8_t SCL = PIN_WIRE_SCL;

  // tengo que inicializar wire asi:
  // Si tengo el error de que DA no existe, recordar de cambiar board a nodemcu.
  Wire.begin(D4, D5);   // 'D4' no es '4' cuando cambio de arduino a nodemcu. igual tengo que investigarlo un poco mas. así me funciona.
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize serial communication
  Serial.begin(115200);

  initDevice();

    

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================


void loop() {
  // if programming failed, don't try to do anything
  if (!dmpReady) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
    sendOrientationMessage();
    sendLocalAcceleration();
    sendGyro();
    sendYawPitchRoll();
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    debug();

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
  }
}


void sendGyro(){
  // leo el quat y lo mando por osc
   gyro[0] = mpu.getRotationX();
   gyro[1] = mpu.getRotationY();
   gyro[2] = mpu.getRotationZ();
//    mpu.dmpGetGyro(&gyro, fifoBuffer);
    // read btnInput and send OSC
    OSCMessage msgOut("/1/rot");
    msgOut.add(gyro[0]);
    msgOut.add(gyro[1]);
    msgOut.add(gyro[2]);

    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();

}
void sendOrientationMessage(){
  // leo el quat y lo mando por osc
    mpu.dmpGetQuaternion(&orientation, fifoBuffer);
    // read btnInput and send OSC
    OSCMessage msgOut("/1/quat");
    msgOut.add(orientation.w);
    msgOut.add(orientation.x);
    msgOut.add(orientation.y);
    msgOut.add(orientation.z);

    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();

}

void sendLocalAcceleration()
{
      // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&orientation, fifoBuffer);
            mpu.dmpGetAccel(&raccel, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &orientation);
            mpu.dmpGetLinearAccel(&laccel, &raccel, &gravity);

            // read btnInput and send OSC
    OSCMessage msgOut("/1/lacc");
    msgOut.add(laccel.x);
    msgOut.add(laccel.y);
    msgOut.add(laccel.z);

    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
}


void sendYawPitchRoll()
{
    mpu.dmpGetQuaternion(&orientation, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &orientation);
    mpu.dmpGetYawPitchRoll(ypr, &orientation, &gravity);
           
    OSCMessage msgOut("/1/ypr");
    // en relación al diseño del sensor la librería devuelve:
    // yaw: (about Z axis)
    msgOut.add(ypr[0]);
    // pitch: (nose up/down, about Y axis)
    msgOut.add(ypr[1]);
    // roll: (tilt left/right, about X axis)
    msgOut.add(ypr[2]);

    Udp.beginPacket(destIp, destPort);
    msgOut.send(Udp);
    Udp.endPacket();
    msgOut.empty();
}




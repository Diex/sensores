// Simple angle meter using ADXL335 accelerometer
//from electronicsblog.net/

#define ADC_ref 3.3

#define zero_x 1.6
#define zero_y 1.65
#define zero_z 2.14

#define max_x 0.8 //v / 1G
#define min_x 2.36   //v / 1G

#define max_y 0.89 //v / 1G
#define min_y 2.46   //v / 1G

#define min_z 0.8 //v / 1G
#define max_z 2.4   //v / 1G


float value_x;
float value_y;
float value_z;

float xv;
float yv;
float zv;

float angle_x;
float angle_y;
float angle_z;

# define SLEEP A1
# define ZERO_G A0
void accelerometer()   {

pinMode(SLEEP, OUTPUT);
digitalWrite(SLEEP, HIGH);

}
float floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void accRead() {

value_x = analogRead(A6) / 1024.0 * ADC_ref;
value_y = analogRead(A3) / 1024.0 * ADC_ref;
value_z = analogRead(A2) / 1024.0 * ADC_ref;


xv = floatMap(value_x, min_x, max_x, -1, 1);
yv = floatMap(value_y, min_y, max_y, -1, 1);
zv = floatMap(value_z, min_z, max_z, -1, 1);


angle_x = atan2(-yv,-zv)*57.2957795+180;
angle_y =atan2(-xv,-zv)*57.2957795+180;
angle_z =atan2(-yv,-xv)*57.2957795+180;

Serial.print(xv, 5);
Serial.print("\t");
Serial.print(yv, 5);
Serial.print("\t");
Serial.print(zv, 5);
Serial.print("\n");




}




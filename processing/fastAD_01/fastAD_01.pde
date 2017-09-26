import processing.serial.*;

int step = 0;
Serial myPort;

void setup(){
  size(1000, 1024);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 115200);
}

void draw(){

}

void serialEvent(Serial p) { 
  String val = p.readStringUntil('\n');
  if(val == null) return;
  println(Integer.parseInt(val));
  //point(step, int(val));
  step ++;
  if(step > width){
    step = 0;
    background(127);
  }
} 
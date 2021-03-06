import toxi.geom.*;
import java.util.Queue;
import java.util.LinkedList;
import java.util.ListIterator;
/**
 * oscP5parsing by andreas schlegel
 * example shows how to parse incoming osc messages "by hand".
 * it is recommended to take a look at oscP5plug for an
 * alternative and more convenient way to parse messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;
Quaternion q;
static final int MAX_QUEUE_SIZE = 256;
int plotH = 30;
int plotW = MAX_QUEUE_SIZE;

// ------------------------
PVector lacc;
Plot xa;
Plot ya;
Plot za;

LinkedList<Float> xaq = new LinkedList();
LinkedList<Float> yaq = new LinkedList();
LinkedList<Float> zaq = new LinkedList();


// ------------------------
PVector rot;

Plot xr;
Plot yr;
Plot zr;

LinkedList<Float> xrq = new LinkedList();
LinkedList<Float> yrq = new LinkedList();
LinkedList<Float> zrq = new LinkedList();

// ------------------------
PVector ypr;
Plot yaw;
Plot pitch;
Plot roll;

LinkedList<Float> yawq = new LinkedList();
LinkedList<Float> pitchq = new LinkedList();
LinkedList<Float> rollq = new LinkedList();

Glass glass1; 
Tilt tilt1;

void setup() {
  size(1024, 768, P3D);
  frameRate(60);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 12000);

  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1", 8000);
  q = new Quaternion();
  lacc = new PVector();
  xa = new Plot(0, 0, plotW, plotH);
  ya = new Plot(0, plotH, plotW, plotH);
  za = new Plot(0, 2 * plotH, plotW, plotH);

  rot = new PVector();

  xr = new Plot(0, 0, plotW, plotH);  
  yr = new Plot(0, plotH, plotW, plotH);  
  zr = new Plot(0, 2 * plotH, plotW, plotH);


  ypr = new PVector();

  yaw = new Plot(0, 0, plotW, plotH);  
  pitch = new Plot(0, plotH, plotW, plotH);  
  roll = new Plot(0, 2* plotH, plotW, plotH);

  glass1 = new Glass();
  tilt1 = new Tilt();
  tilt1.origin.x = plotW/2;
  tilt1.origin.y = height/2;
  
}

// los ejes del sensor están cruzados
// +X -X
// +y -Z
// +z -Y

color red = color(255, 0, 0);
color green = color(0, 255, 0);
color blue = color(0, 0, 255);

void draw() {
  background(0);
  fill(127, 127, 127);
  noStroke();
  
  // ------------------------------------------------
  // ploteos
  // ------------------------------------------------
  pushMatrix();
  rect(0, 0, plotW, 3 * plotH);  
  xa.render(red, xaq);
  ya.render(green, yaq);
  za.render(blue, zaq);

  translate(0, 10 + 3 * plotH, 0);
  rect(0, 0, plotW, 3 * plotH);
  xr.render(red, xrq);
  yr.render(green, yrq);
  zr.render(blue, zrq);

  translate(0, 10 + 3 * plotH, 0);
  rect(0, 0, plotW, 3 * plotH);
  
  yaw.render(red, yawq);
  yaw.scale = 1;
  pitch.render(green, pitchq);
  pitch.scale = 1;
  roll.render(blue, rollq);
  roll.scale = 1;
  popMatrix();
  
  // ------------------------------------------------
  // copa
  // ------------------------------------------------
  glass1.render(q, new PVector(plotW/2,height-100));
  tilt1.render(ypr.z, ypr.y);
  
  addSample(lacc.x, xaq, -16384, 16384); 
  addSample(lacc.y, yaq, -16384, 16384);
  addSample(lacc.z, zaq, -16384, 16384);

  addSample(rot.x, xrq, -16384, 16384); 
  addSample(rot.y, yrq, -16384, 16384);
  addSample(rot.z, zrq, -16384, 16384);

  addSample(ypr.z, yawq, -PI, PI);        
  addSample(ypr.y, pitchq, -PI, PI);
  addSample(ypr.x, rollq, -PI, PI);
  //println(ypr);
}




void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */

  if (theOscMessage.checkAddrPattern("/1/quat")==true) {
    //println("### received an osc message. with address pattern " + theOscMessage.addrPattern());
    //println("### received an osc message. with address pattern " + theOscMessage.typetag());

    if (theOscMessage.checkTypetag("ffff")) {
      float w = theOscMessage.get(0).floatValue();
      float x = theOscMessage.get(1).floatValue();
      float y = theOscMessage.get(2).floatValue();
      float z = theOscMessage.get(3).floatValue();
      //println("quat: " + " [ "+w+ " , " +x +" , " +y +" , " +z+"]");

      if (q != null) q.set(w, x, y, z);
    }
  } 

  if (theOscMessage.checkAddrPattern("/1/lacc")==true) {
    //println("### received an osc message. with address pattern " + theOscMessage.addrPattern());
    //println("### received an osc message. with address pattern " + theOscMessage.typetag());

    if (theOscMessage.checkTypetag("iii")) {
      float x = theOscMessage.get(0).intValue();
      float y = theOscMessage.get(1).intValue();
      float z = theOscMessage.get(2).intValue();
      //println("lacc: "  + " [ " +x +" , " +y +" , " +z+"]");

      if (lacc != null) {
        lacc.set(x, y, z);
      }
    }
  }
  if (theOscMessage.checkAddrPattern("/1/rot")==true) {
    //println("### received an osc message. with address pattern " + theOscMessage.addrPattern());
    //println("### received an osc message. with address pattern " + theOscMessage.typetag());
    if (theOscMessage.checkTypetag("iii")) {
      float x = theOscMessage.get(0).intValue();
      float y = theOscMessage.get(1).intValue();
      float z = theOscMessage.get(2).intValue();
      //println("rot: "+ " [ " +x +" , " +y +" , " +z+"]");

      if (rot != null) {
        rot.set(x, y, z);
      }
    }
  }

  if (theOscMessage.checkAddrPattern("/1/ypr")==true) {
    //println("### received an osc message. with address pattern " + theOscMessage.addrPattern());
    //println("### received an osc message. with address pattern " + theOscMessage.typetag());
    if (theOscMessage.checkTypetag("fff")) {
      float x = theOscMessage.get(0).floatValue();
      float y = theOscMessage.get(1).floatValue();
      float z = theOscMessage.get(2).floatValue();
      //println("ypr: "+ " [ " +x +" , " +y +" , " +z+"]");

      if (ypr != null) {
        ypr.set(x, y, z);
      }
    }
  }
}

void addSample(float val, LinkedList fifo, float min, float max) {
  float v = map(val, min, max, -1.0, 1.0);
  fifo.add(v);
  if (fifo.size() > MAX_QUEUE_SIZE) fifo.remove();
}

void addSample(float val, LinkedList fifo, int min, int max) {
  float v = map(val, min, max, -1.0, 1.0);
  fifo.add(v);
  if (fifo.size() > MAX_QUEUE_SIZE) fifo.remove();
}
import toxi.audio.*;
import toxi.color.*;
import toxi.color.theory.*;
import toxi.data.csv.*;
import toxi.data.feeds.*;
import toxi.data.feeds.util.*;
import toxi.doap.*;
import toxi.geom.*;
import toxi.geom.mesh.*;
import toxi.geom.mesh.subdiv.*;
import toxi.geom.mesh2d.*;
import toxi.geom.nurbs.*;
import toxi.image.util.*;
import toxi.math.*;
import toxi.math.conversion.*;
import toxi.math.noise.*;
import toxi.math.waves.*;
import toxi.music.*;
import toxi.music.scale.*;
import toxi.net.*;
import toxi.newmesh.*;
import toxi.nio.*;
import toxi.physics2d.*;
import toxi.physics2d.behaviors.*;
import toxi.physics2d.constraints.*;
import toxi.physics3d.*;
import toxi.physics3d.behaviors.*;
import toxi.physics3d.constraints.*;
import toxi.processing.*;
import toxi.sim.automata.*;
import toxi.sim.dla.*;
import toxi.sim.erosion.*;
import toxi.sim.fluids.*;
import toxi.sim.grayscott.*;
import toxi.util.*;
import toxi.util.datatypes.*;
import toxi.util.events.*;
import toxi.volume.*;


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

// ------------------------
PVector lacc;
Plot xa;
Plot ya;
Plot za;
int plotH = 40;
// ------------------------
PVector rot;
Plot xr;
Plot yr;
Plot zr;

PShape glass;
PImage pepsi;

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
  xa = new Plot(width, plotH, -16384, 16384);
  ya = new Plot(width, plotH, -16384, 16384);
  za = new Plot(width, plotH, -16384, 16384);
  rot = new PVector();

  xr = new Plot(width, plotH, -16384, 16384);
  xr.origin.set(0, plotH);
  yr = new Plot(width, plotH, -16384, 16384);
  yr.origin.set(0, plotH);
  zr = new Plot(width, plotH, -16384, 16384);
  zr.origin.set(0, plotH);



  pepsi = loadImage("Glass.jpg");
  glass = loadShape("Glass.obj");
  glass.setTexture(pepsi);
  glass.scale(20);
  glass.rotateX(PI);
}

// +X -X
// +y -Z
// +z -Y


void draw() {
  background(0);
  fill(127, 127, 127);
  noStroke();
  rect(0, 0, width, plotH);
  xa.render(color(255, 0, 0));
  ya.render(color(0, 255, 0));
  za.render(color(0, 0, 255));

  rect(0, plotH, width, plotH);
  xr.render(color(255, 0, 0));
  yr.render(color(0, 255, 0));
  zr.render(color(0, 0, 255));

  lights();
  directionalLight(51, 102, 126, -1, 0, 0);
  directionalLight(255, 0, 100, 1, 0, 0);


  fill(255);
  translate(width/2, height/2);
  // https://gist.github.com/jessherzog/a29829e852579b5f6c7d
  // toxiclibs direct angle/axis rotation from quaternion (NO gimbal lock!)
  // (axis order [1, 3, 2] and inversion [-1, +1, +1] is a consequence of
  // different coordinate system orientation assumptions between Processing
  // and InvenSense DMP)
  float[] axis = q.toAxisAngle();
  // angle + x y z 
  rotate(axis[0], -axis[1], axis[3], axis[2]);
  shape(glass);
  strokeWeight(2);
  stroke(255, 0, 0);  // red
  line(0, 0, 0, 100, 0, 0);
  stroke(0, 0, 255);  // blue (esta cruzado)  
  line(0, 0, 0, 0, 100, 0);
  stroke(0, 255, 0); // green
  line(0, 0, 0, 0, 0, 100);
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
        xa.addSample(x);
        ya.addSample(y);
        za.addSample(z);
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
        xr.addSample(x);
        yr.addSample(y);
        zr.addSample(z);
      }
    }
  }
}
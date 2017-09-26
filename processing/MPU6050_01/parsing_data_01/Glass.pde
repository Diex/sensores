class Glass{
PShape glass;

  Glass(){
    glass = loadShape("Glass.obj");
    glass.scale(20);
    glass.rotateX(PI);
  
  }
  void render(Quaternion q, PVector pos){
    pushMatrix();
    lights();
  directionalLight(51, 102, 126, -1, 0, 0);
  directionalLight(255, 0, 100, 1, 0, 0);


  fill(255);
  translate(pos.x, pos.y);
  // https://gist.github.com/jessherzog/a29829e852579b5f6c7d
  // toxiclibs direct angle/axis rotation from quaternion (NO gimbal lock!)
  // (axis order [1, 3, 2] and inversion [-1, +1, +1] is a consequence of
  // different coordinate system orientation assumptions between Processing
  // and InvenSense DMP)
  float[] axis = q.toAxisAngle();
  // angle + x y z 
  rotate(axis[0], -axis[1], axis[3], axis[2]);
  translate(0, -100, 0);
  shape(glass);
  translate(0, 100, 0);

  strokeWeight(2);
  stroke(255, 0, 0);  // red
  line(0, 0, 0, 100, 0, 0);
  stroke(0, 0, 255);  // blue (esta cruzado)  
  line(0, 0, 0, 0, 100, 0);
  stroke(0, 255, 0); // green
  line(0, 0, 0, 0, 0, 100);
  popMatrix();

  }
}
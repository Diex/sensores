class Tilt{
  float rad = 100;
  float th = 10;
  
  PVector origin = new PVector();
  Tilt(){
    
  };
  
  void render(float alfa, float beta){
    float x = map(beta, -PI, PI, rad, -rad);
    float y = map(alfa, -PI, PI, -rad, rad);
    
    
    pushMatrix();
    pushStyle();
    ellipseMode(CENTER);
    translate(origin.x, origin.y);    
    stroke(255);
    if( dist(0,0, x, y) > th){
      fill (255,0,0);
    }else{      
      fill(127);
    };    
    ellipse(0,0,rad, rad);
    fill(255);
    ellipse(x,y, th, th);
    line(0,0, x, y);
    ellipse(x,y, 5,5);
    popStyle();
    popMatrix();
  }

}
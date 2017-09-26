class Inertial{

  PVector origin = new PVector(width/2, 0);
  int h = 10;
  int w; 
  Inertial(int w){
    this.w = w;
  }
  
  
  void render(color col){
    fill(col);
    rect(origin.x, origin.y, h, 0);
    
  }
  

}
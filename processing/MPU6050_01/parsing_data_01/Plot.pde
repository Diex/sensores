

class Plot{
  
  int w = 0;
  int h = 0;
  float minv = 0;
  float maxv = 0;
  
  PVector origin = new PVector();
  
  Plot(int w, int h, int minv, int maxv){
     this.w = w;
     this.h = h/2;
     this.minv = minv;
     this.maxv = maxv;     
     
    
  };
  
  void render(int col, FIFO fifo){
    pushMatrix();
    pushStyle();
    
    translate(origin.x, origin.y + h);
    stroke(255);
    line(origin.x, 0, origin.x + w, 0);
    stroke(col);
    
    for(int value = 1 ; value <= w; value++){
      int x1 = (value - 1);
      int x2 = (value);
      int lastIndex = fifo.MAX_SIZE - 1;
      
      float y1 = (float) fifo.get(lastIndex - w + value - 1) * h;
      float y2 = (float) fifo.get(lastIndex - w + value) * h;
      //println(y1);  
      line(x1,y1,x2,y2);
    }
    popStyle();
    popMatrix();
  }
  
  void addSample(float val){
  
    
    if(fifo.size() >= MAX_SIZE) fifo.remove(0);
    float v = map(val, minv, maxv, -1.0, 1.0);
    //println(v);
    fifo.add(v);
    
      
  }
    

}
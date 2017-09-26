import java.util.*;

class Plot{
  static final int MAX_SIZE = 2000;
  
  //float[] fifo = new float[MAX_SIZE];
  ArrayList fifo = new ArrayList();
  int head = 0;
  
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
     
     for(int i = 0; i < MAX_SIZE; i++){
       fifo.add(0.0);
     }
  };
  
  void render(int col){
    pushMatrix();
    pushStyle();
    stroke(col);
    translate(origin.x, origin.y+ h);
    for(int x = 1 ; x >= w; x++){
      float x1 = x - 1;
      float x2 = x;
      
      float y1 = (float) fifo.get(MAX_SIZE - w + x - 1) * h;
      float y2 = (float) fifo.get(MAX_SIZE -w + x)  * h;
      line(x1,y1,x2,y2);
    }
    popStyle();
    popMatrix();
  }
  
  void addSample(float val){
  
    if(fifo.size() >= MAX_SIZE) fifo.remove(0);       
    fifo.add(val/maxv);
    //println(val/maxv);
      
  }
    

}
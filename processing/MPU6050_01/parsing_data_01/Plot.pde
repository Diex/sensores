

class Plot {

  int w = 0;
  int h = 0;
  
  PVector origin = new PVector();
  float scale = 10.;
  Plot(int x, int y, int w, int h) {
    
    this.w = w;
    this.h = h/2;
    origin.x = x;
    origin.y = y + this.h;
    
  };

  void render(int col, LinkedList<Float> fifo) {
    pushMatrix();
    pushStyle();

    ListIterator l = ((LinkedList<Float>) fifo.clone()).listIterator();

    translate(origin.x, origin.y);
    stroke(col);
    float py = 0;
    int px = 0;
    for (int x = 0; x < w; x++) {
      int x1 = px;
      int x2 = x + 1;
      float y1 = py; //h * (l.hasNext() ? (float) l.next() : 0.0);
      float y2 = h * scale * (l.hasNext() ? (float) l.next() : 0.0);
      
      line(x1,y1,x2,y2);
      py = y2;
      px = x2;
    }

    popStyle();
    popMatrix();
  }
}
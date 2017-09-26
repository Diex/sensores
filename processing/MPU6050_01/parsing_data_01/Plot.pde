

class Plot {

  int w = 0;
  int h = 0;
  
  PVector origin = new PVector();

  Plot(int w, int h) {
    this.w = w;
    this.h = h/2;
  };

  void render(int col, LinkedList<Float> fifo) {
    pushMatrix();
    pushStyle();

    ListIterator l = ((LinkedList<Float>) fifo.clone()).listIterator();

    translate(origin.x, origin.y + h);
    stroke(col);
    for (int x = 0; x < w; x++) {
      int x1 = x;
      int x2 = x + 1;
      float y1 = h * (l.hasNext() ? (float) l.next() : 0.0);
      float y2 = h * (l.hasNext() ? (float) l.next() : 0.0);
      
      line(x1,y1,x2,y2);
    }

    popStyle();
    popMatrix();
  }
}
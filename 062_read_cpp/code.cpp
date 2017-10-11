#include <stdio.h>
#include <stdlib.h>

class MyClass {
private:
  int x;
  int y;
public:
  void setPoints(int newx, int newy) { 
    x = newx;
    y = newy;
  }
  void doStuff(const MyClass & p);
  int computeSomething() const {
    return x *3 + y -2;
  }
};

void printMessage(const MyClass * m1) {
  printf("%p has %d\n", m1, m1->computeSomething());
}
void MyClass::doStuff(const MyClass & p) {
  x = p.y + 3;
  y = p.x - 4;
  printMessage(this);
  printMessage(&p);
}

int main(void) {
  MyClass a;
  MyClass b;
  MyClass c;
  a.setPoints(1,1);
  b.setPoints(22,40);
  c.setPoints(9,3);
  a.doStuff(b);
  b.doStuff(c);
  c.doStuff(a);
  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

class MyClass {
private:
  int a;
  int b;
public:
  MyClass() : a(3), b(4) {
    printf("Creating MyClass\n");
  }
  ~MyClass() {
    printf("Destroying with a=%d, b=%d\n",a, b);
  }
  void mathify(int x) {
    a = a * x + b;
    b = b - x;
    printf("Math resulted in a=%d b=%d\n",a,b);
  }
};

int main(void) {
  MyClass ** array = new MyClass*[4];
  printf("First loop\n");
  for (int i = 0; i < 4; i++) {
    array[i] = new MyClass();
  }
  printf("Second loop\n");
  for (int i = 0; i < 4; i++) {
    array[i]->mathify(i);
  }
  printf("Third loop\n");
  for (int i = 0; i < 4; i++) {
    delete array[i];
  }
  printf("Done looping\n");
  delete[] array;
  return EXIT_SUCCESS;
}

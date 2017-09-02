 1. Create a file "answer.txt".
 2. Execute the following code by hand 
   (as you learned in Chapter 2), and write
   the output in your "answer.txt" file

struct _myStruct {
  int a;
  double b;
};
typedef struct _myStruct myStruct;

struct _otherStruct {
  myStruct s;
  char c;
};
typedef struct _otherStruct otherStruct;

otherStruct f(otherStruct x, otherStruct y) {
  x.s.a += y.s.a;
  x.s.b +=  2 * y.s.b;
  x.c += y.s.a;
  return x;
}

int main(void) {
  otherStruct s1;
  otherStruct s2;
  s1.s.a = 4;
  s1.s.b = 3.14;
  s1.c = 'a';
  s2.s.a = 2;
  s2.s.b = 103.4;
  s2.c = 'M';
  otherStruct s3 = f(s1,s2);
  otherStruct s4 = f(s2,s3);
  printf("s1: %d %f %c\n", s1.s.a, s1.s.b, s1.c);
  printf("s2: %d %f %c\n", s2.s.a, s2.s.b, s2.c);
  printf("s3: %d %f %c\n", s3.s.a, s3.s.b, s3.c);
  printf("s4: %d %f %c\n", s4.s.a, s4.s.b, s4.c);
  return 0;
}
//end of code
3. Submit your answer.txt file for grading



 



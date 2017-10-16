#include "Matrix.h"
typedef Matrix<int> IntMatrix;

int main(void) {
  IntMatrix a(4,5);
  for (int i = 0; i < a.getRows(); i++) {
    for (int j = 0; j < a.getColumns(); j++) {
      a[i][j] = i+j;
    }
  }
  std::cout << "a is:\n" << a << std::endl;
  IntMatrix b(a);
  std::cout << "b is:\n" << a << std::endl;
  if (a == b) {
    std::cout << "a and b have the same contents" << std::endl;
  }
  else {
    std::cout << "a and b have different contents" << std::endl;
  }
  for (int i = 0; i < b.getRows(); i++) {
    for (int j = 0; j < b.getColumns(); j++) {
      b[i][j] += i*j;
    }
  }
  std::cout << "Now, b is:\n" << b << std::endl;
  if (a == b) {
    std::cout << "a and b have the same contents" << std::endl;
  }
  else {
    std::cout << "a and b have different contents" << std::endl;
  }

  IntMatrix c;
  std::cout << "c is:\n" << c << std::endl;
  c = a + b;
  std::cout << "Now, c is:\n" << c << std::endl;

  Matrix<std::string> s1(3,3);
  s1[0][0] = "abc";
  s1[0][1] = "def";
  s1[0][2] = "ghi";
  s1[1][0] = "jkl";
  s1[1][1] = "mno";
  s1[1][2] = "pqr";
  s1[2][0] = "stu";
  s1[2][1] = "vwx";
  s1[2][2] = "yz_";
  std::cout << "s1 is:\n" << s1 << std::endl;
  Matrix<std::string> s2(3,3);
  s2[0][0] = "...";
  s2[0][1] = "+++";
  s2[0][2] = "---";
  s2[1][0] = "***";
  s2[1][1] = "&&&";
  s2[1][2] = "[ ]";
  s2[2][0] = "( )";
  s2[2][1] = "@@@";
  s2[2][2] = "!!!";
  std::cout << "s2 is:\n" << s2 << std::endl;
  Matrix<std::string> s3(s1+s2);
  std::cout << "s3 is:\n" << s3 << std::endl;
  return EXIT_SUCCESS;
}

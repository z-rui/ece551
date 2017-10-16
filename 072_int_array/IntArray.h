#ifndef __INT_ARRAY__H__
#define __INT_ARRAY__H__
#include <cstdlib>
#include <ostream>

class IntArray {
private:
  int * data;
  int numElements;
public:
  IntArray();
  IntArray(int n);
  IntArray(const IntArray & rhs);
  IntArray & operator=(const IntArray & rhs);
  int & operator[](int index);
  const int & operator[](int index) const;
  int size() const;
  bool operator==(const IntArray& rhs) const;
  bool operator!=(const IntArray& rhs) const;
  ~IntArray();
};

std::ostream & operator<<(std::ostream & s, const IntArray & rhs);

#endif

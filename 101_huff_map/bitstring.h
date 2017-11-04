#ifndef __BIT_STRING_H__
#define __BIT_STRING_H__
#include <deque>
#include <iostream>
#include <stdio.h>

class BitString {
  std::deque<bool> bits;
  BitString(std::deque<bool> b);
 public:
  BitString();
  BitString plusZero() const ;
  BitString plusOne() const;
  void append(const BitString & rhs) ;
  void insertByte(unsigned char b);
  unsigned char removeByte() ;
  bool removeBit();
  int size() const ;
  friend std::ostream & operator<<(std::ostream & s, const BitString & bs);
};

class BitFileWriter {
private:
  //This should NOT be copied or assigned to.
  BitFileWriter(const BitFileWriter & rhs);
  BitFileWriter & operator=(const BitFileWriter & rhs);
  FILE * f;
  BitString pending;
  void pushData();
public:
  BitFileWriter(const char * fname);
  void writeByte(unsigned char b);
  void writeBitString(const BitString & bs);
  ~BitFileWriter();
};

class BitReader {
private:
  FILE * f;
  BitString bs;
  void ensureData(int);
public:
 BitReader(FILE * input): f(input), bs() {}
  virtual ~BitReader() {
    fclose(f);
    f = NULL;
  }
  bool readBit();
  unsigned char readByte() ;

};

#endif

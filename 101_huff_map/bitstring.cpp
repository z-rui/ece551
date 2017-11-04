#include "bitstring.h"
#include <stdio.h>
#include <exception>
#include <assert.h>
#include <stdlib.h>

class IllegalCopy : public std::exception {
public:
  const char * what()const throw() {
    return "You cannot make a copy of a BitFileWriter\n";
  }
};
BitString::BitString():bits() {}
BitString::BitString(std::deque<bool> b): bits(b) {
}
BitString BitString::plusZero() const {
  std::deque<bool> temp(bits);
  temp.push_back(false);
  return BitString(temp);
}
BitString BitString::plusOne()const {
  std::deque<bool> temp(bits);
  temp.push_back(true);
  return BitString(temp);
}
void BitString::append(const BitString & rhs) {
  for (unsigned i =0; i < rhs.bits.size(); i++) {
    bits.push_back(rhs.bits[i]);
  }
}
void BitString::insertByte(unsigned char b) {
  unsigned char mask = 0x80;
  for (int i = 0; i < 8; i++) {
    bits.push_back((b&mask) != 0);
    mask = mask >> 1;
  }
}
bool BitString::removeBit() {
  assert(bits.size() > 0);
  bool b = bits[0];
  bits.pop_front();
  return b;
}
unsigned char BitString::removeByte() {
  assert(bits.size() >= 8);
  unsigned char ans = 0;
  for (int i = 0; i < 8; i++) {
    bool b = bits[0];
    bits.pop_front();
    ans = (ans << 1) | (b ? 1 : 0);
  }
  return ans;
}
int BitString::size() const { 
  return bits.size(); 
}

std::ostream & operator<<(std::ostream & s, const BitString  & bs) {
  for(unsigned i = 0; i < bs.bits.size(); i++) {
    char c = bs.bits[i] ? '1' : '0';
    s << c;
  }
  return s;
}

BitFileWriter::BitFileWriter(const char * fname): f(fopen(fname,"w")), 
						  pending() {
  if (f == NULL) {
    fprintf(stderr,"Could not open %s\n", fname);
    exit(EXIT_FAILURE);
  }
}

BitFileWriter::BitFileWriter(const BitFileWriter & rhs) {
  throw IllegalCopy();
}
BitFileWriter & BitFileWriter::operator=(const BitFileWriter & rhs) {
  throw IllegalCopy();
}
void BitFileWriter::pushData() {
  while(pending.size() >= 8) {
    unsigned char b = pending.removeByte();
    fwrite(&b,sizeof(b),1, f);
  }
}
void BitFileWriter::writeByte(unsigned char b) {
  pending.insertByte(b);

  pushData();
}
void BitFileWriter::writeBitString(const BitString & bs) {
  pending.append(bs);
  pushData();
}
BitFileWriter::~BitFileWriter() {
  while (pending.size() % 8 != 0) {
    pending = pending.plusZero();
  }
  pushData();
  int result = fclose(f);
  if (result != 0) {
    perror("fclose failed!");
  }
}

void BitReader::ensureData(int n) {
  if (bs.size() < n) {
    int x = fgetc(f);
    unsigned char c = x == EOF? 0 : x;
    bs.insertByte(c);
  }
}

bool BitReader::readBit() {
  ensureData(1);
  return bs.removeBit();
}
unsigned char BitReader::readByte() {
  ensureData(8);
  return bs.removeByte();
}

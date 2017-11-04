#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include "readFreq.h"

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256 ) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
  std::ifstream f(fname);
  char ch;
  uint64_t *counts;
  counts = new uint64_t[257]();
  while (f.get(ch)) {
    counts[(unsigned char) ch]++;
  }
  counts[256] = 1;
  return counts;
}

// vim: ts=2:sw=2:et

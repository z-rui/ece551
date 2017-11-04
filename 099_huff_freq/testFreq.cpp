#include "readFreq.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " (input file)" << std::endl;
    return EXIT_FAILURE;
  }
  uint64_t * counts = readFrequencies(argv[1]);
  assert(counts != NULL);
  for (unsigned i = 0; i < 257; i++) {
    if (counts[i] == 0) {
      continue;
    }
    printSym(std::cout, i);
    std::cout << ": " << counts[i] << std::endl;
  }
  delete[] counts;
  return EXIT_SUCCESS;
}

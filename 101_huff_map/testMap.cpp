#include <iostream>
#include "readFreq.h"
#include "node.h"

void printTable(const std::map<unsigned,BitString> & theMap) {
  for (int i =0 ; i < 257; i++) {
    std::map<unsigned,BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      printSym(std::cout, i);
      std::cout << " = " <<  (it->second) << std::endl ;
    }
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " (input file)" << std::endl;
    return EXIT_FAILURE;
  }
  uint64_t * counts = readFrequencies(argv[1]);
  assert(counts != NULL);
  Node * tree = buildTree (counts);
  delete[] counts;
  std::map<unsigned,BitString> theMap;
  BitString empty;
  tree->buildMap(empty, theMap);
  printTable(theMap);
  delete tree;
  return EXIT_SUCCESS;
}

#include "node.h"
#include "readFreq.h"
#include <iostream>

void indentBy(int n) {
  for (int i = 0; i < n; i++) {
    std::cout << " ";
  }
}
int printTree(int indent, Node * t) {
  assert (t != NULL);
  if (t->sym == NO_SYM) {
    std::cout << "(";
    int rpos = printTree(indent + 1, t->left);
    if (rpos == 0) {
      std::cout << std::endl;
      rpos = indent +1;
      indentBy(rpos);
    }
    printTree(rpos, t->right);
    std::cout << ")";
    return 0;
  }
  else {
    std::cout << "<";
    printSym(std::cout, t->sym);
    std::cout << ": ";
    std::streamsize w = std::cout.width(6);
    std::cout << t->freq ;
    std::cout.width(w);
    std::cout << ">";
    return indent + 1 + 3+ 2 + 6 + 1;
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
  printTree(0, tree);
  std::cout << std::endl;
  delete tree;
  return EXIT_SUCCESS;
}

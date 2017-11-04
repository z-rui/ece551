#include "node.h"
void Node::buildMap(BitString b, std::map<unsigned,BitString> & theMap) {
  if (sym != NO_SYM) {
    theMap[sym] = b;
  } else {
    left->buildMap(b.plusZero(), theMap);
    right->buildMap(b.plusOne(), theMap);
  }
}

// vim: sw=2:ts=2:et

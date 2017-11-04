#include "node.h"

Node *  buildTree(uint64_t * counts) {
  priority_queue_t pq;
  for (int i = 0; i <= 256; i++) {
    if (counts[i] > 0) {
      pq.push(new Node(i, counts[i]));
    }
  }
  while (pq.size() > 1) {
    Node *l = pq.top();
    pq.pop();
    Node *r = pq.top();
    pq.pop();
    pq.push(new Node(l, r));
  }
  return pq.top();
}

// vim: ts=2:sw=2:et

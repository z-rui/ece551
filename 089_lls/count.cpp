#include "ll.h"
#include <iostream>
#include <stdio.h>

template<typename T>
class Counted_OrdItem {
public:
  T item;
  unsigned int count;
  Counted_OrdItem(): item(), count(0) {}
  Counted_OrdItem(const T& _item): item(_item), count(1) {}
  bool operator==(const Counted_OrdItem & rhs) const {
    return item == rhs.item;
  }
  bool operator!=(const Counted_OrdItem & rhs) const {
    return item != rhs.item;
  }
  Counted_OrdItem & operator++() {
    count++;
    return *this;
  }
};

template<typename T>
class Counted_OrdCount {
public:
  T item;
  unsigned int count;
  Counted_OrdCount(): item(), count(0){}
  Counted_OrdCount(const Counted_OrdItem<T> & rhs): item(rhs.item), count(rhs.count) {}
  bool operator < (const Counted_OrdCount & rhs) const {
    return count < rhs.count;
  }
  
};


template<typename T>
void sort(LinkedList<T> & ll) {
  bool sorted = false;
  int sz = ll.getSize();
  while (!sorted) {
    sorted = true;
    for (int i =0; i < sz - 1; i++) {
      if (ll[i+1] < ll[i]) {
	T temp = ll[i];
	ll[i] = ll[i+1];
	ll[i+1] = temp;
	sorted = false;
      }
    }
  }
}

template<typename T>
LinkedList<Counted_OrdItem<T> > count(const LinkedList<T> & ll) {
  LinkedList<Counted_OrdItem<T> > ans;
  for (int i = 0; i < ll.getSize(); i++) {
    const T & curr = ll[i];
    int index = ans.find(Counted_OrdItem<T>(curr));
    if (index == -1) {
      ans.addBack(Counted_OrdItem<T>(curr));
    }
    else {
      ++ans[index];
    }
  }
  return ans;
}

template<typename T>
LinkedList<Counted_OrdCount<T> > convertTypes(LinkedList<Counted_OrdItem<T> > ll) {
  LinkedList<Counted_OrdCount<T> > ans;
  while (ll.getSize() != 0) {
    Counted_OrdItem<T> & curr = ll[0];
    ans.addFront(Counted_OrdCount<T>(curr));
    ll.remove(curr);
  }
  return ans;

}
int isidchar(char c) {
  return isalnum(c) || c == '_';
}
void addInput (LinkedList<std::string> * list, char * line) {
  if (*line == '\0') {
    return;
  }
  char * start = line;
  while (*start != '\0') {
    while(isspace(*line) == isspace(*start) &&
	  isidchar(*line) == isidchar(*start)) {
      line++;
    }
    if (!isspace(*start)) {
      list->addFront(std::string(start,line));
    }
    start = line;
  }
}
LinkedList<std::string> * readInput(const char * fname) {
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    std::cerr << "Cannot open " << fname << "\n";
    return NULL;
  }
  LinkedList<std::string> * ans = new LinkedList<std::string>();
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, f) != -1) {
    addInput (ans, line);
  }
  free(line);
  if(fclose(f) != 0) {
    std::cerr << "Warning: Cannot close" << fname << "\n";
  }
  return ans;
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: ll infile removalfile\n";
    return EXIT_FAILURE;
  }
  LinkedList<std::string> * items = readInput(argv[1]);
  if (items == NULL) {
    return EXIT_FAILURE;
  }
  LinkedList<std::string> * toRemove = readInput(argv[2]);
  if (toRemove == NULL) {
    delete items;
    return EXIT_FAILURE;
  }
  LinkedList<Counted_OrdItem<std::string> > counts = count(*items);
  int sz = toRemove->getSize();
  while (sz > 0){
    std::string s = (*toRemove)[sz-1];
    if (counts.remove(Counted_OrdItem<std::string>(s))) {
      std::cout << "Removed " << s << "\n";
    }
    else {
      std::cout << "Did not find " << s << "\n";
    }
    while(toRemove->remove(s)) { sz--; }
  }
  LinkedList<Counted_OrdCount<std::string> > converted = convertTypes(counts);
  sort(converted);
  for (int i = 0; i < converted.getSize(); i++) {
    std::cout << converted[i].count << ": " << converted[i].item << "\n";
  }
  delete items;
  delete toRemove;
  return EXIT_SUCCESS;
}

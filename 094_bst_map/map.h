#ifndef __MAP_H__
#define __MAP_H__
#include <stdexcept>

template<typename K, typename V>
class Map {
public:
  virtual void add(const K & key, const V & value) = 0;
  virtual const V & lookup(const K& key) const throw (std::invalid_argument) = 0;
  virtual void remove(const K& key) = 0;
  virtual ~Map<K,V>() {}
};
#endif

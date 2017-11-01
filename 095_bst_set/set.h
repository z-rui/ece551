#ifndef __SET_H__
#define __SET_H__
#include <stdexcept>

template<typename T>
class Set {
public:
  virtual void add(const T & key) = 0;
  virtual bool contains(const T& key) const = 0;
  virtual void remove(const T& key) = 0;
  virtual ~Set<T>() {}
};
#endif

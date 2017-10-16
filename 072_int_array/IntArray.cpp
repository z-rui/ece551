#include "IntArray.h"
#include <assert.h>
#include <ostream>

IntArray::IntArray() {

}
IntArray::IntArray(int n) {

}

IntArray::IntArray(const IntArray & rhs)  {

}
IntArray::~IntArray() {

}

IntArray & IntArray::operator=(const IntArray & rhs) {
}
const int & IntArray::operator[](int index) const {
}
int & IntArray::operator[](int index) {
}

int IntArray::size() const {
}

bool IntArray::operator==(const IntArray & rhs) const {
}

bool IntArray::operator!=(const IntArray & rhs) const {
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {

}

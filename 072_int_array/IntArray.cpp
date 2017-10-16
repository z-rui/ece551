#include "IntArray.h"
#include <assert.h>
#include <ostream>
#include <algorithm>

IntArray::IntArray() : data(NULL), numElements(0) {

}
IntArray::IntArray(int n) : data(new int[n]), numElements(n) {

}

IntArray::IntArray(const IntArray & rhs)  {
	numElements = rhs.numElements;
	data = new int[numElements]();
}
IntArray::~IntArray() {
	delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
	if (this == &rhs) {
		return *this;
	}
	delete[] data;
	numElements = rhs.numElements;
	data = new int[numElements];
	std::copy(rhs.data, rhs.data + numElements, data);
	return *this;
}
const int & IntArray::operator[](int index) const {
	assert(0 <= index && index < numElements);
	return data[index];
}
int & IntArray::operator[](int index) {
	assert(0 <= index && index < numElements);
	return data[index];
}

int IntArray::size() const {
	return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
	if (numElements != rhs.numElements) {
		return false;
	}
	for (int i = 0; i < numElements; i++) {
		if (data[i] != rhs.data[i]) {
			return false;
		}
	}
	return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
	return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
	bool first = true;

	s << '{';
	for (int i = 0; i < rhs.size(); i++) {
		if (first) {
			first = false;
		} else {
			s << ", ";
		}
		s << rhs[i];
	}
	s << '}';
	return s;
}

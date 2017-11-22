#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

class DebugStream {
public:
	std::ostream *os;
	DebugStream() : os(NULL) {}
	template <typename T>
	const DebugStream &operator<< (T x) const {
		if (os) {
			*os << x;
		}
		return *this;
	}
};

#endif

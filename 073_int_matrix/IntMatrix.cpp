#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(NULL) {
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c) {
	rows = new IntArray*[numRows];
	for (int i = 0; i < numRows; i++) {
		rows[i] = new IntArray(numColumns);
	}
}
IntMatrix::IntMatrix(const IntMatrix & rhs)  {
	numRows = rhs.numRows;
	numColumns = rhs.numColumns;
	rows = new IntArray*[numRows];
	for (int i = 0; i < numRows; i++) {
		rows[i] = new IntArray(numColumns);
		*rows[i] = *rhs.rows[i];
	}
}
IntMatrix::~IntMatrix() {
	for (int i = 0; i < numRows; i++) {
		delete rows[i];
	}
	delete[] rows;
}
IntMatrix &IntMatrix::operator=(const IntMatrix & rhs) {
	if (this == &rhs) {
		return *this;
	}
	for (int i = 0; i < numRows; i++) {
		delete rows[i];
	}
	delete[] rows;

	numRows = rhs.numRows;
	numColumns = rhs.numColumns;
	rows = new IntArray*[numRows];
	for (int i = 0; i < numRows; i++) {
		rows[i] = new IntArray(numColumns);
		*rows[i] = *rhs.rows[i];
	}

	return *this;
}
int IntMatrix::getRows() const {
	return numRows;
}
int IntMatrix::getColumns() const {
	return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
	assert(0 <= index && index < numRows);
	return *rows[index];
}
IntArray & IntMatrix::operator[](int index){
	assert(0 <= index && index < numRows);
	return *rows[index];
}



bool IntMatrix::operator==(const IntMatrix & rhs) const {
	if (numRows != rhs.numRows || numColumns != rhs.numColumns) {
		return false;
	}
	for (int i = 0; i < numRows; i++) {
		if (*rows[i] != *rhs.rows[i]) {
			return false;
		}
	}
	return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
	assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
	IntMatrix ans(numRows, numColumns);
	for (int i = 0; i < numRows; i++) {
		IntArray &z = *ans.rows[i];
		IntArray &x = *rows[i];
		IntArray &y = *rhs.rows[i];
		for (int j = 0; j < numColumns; j++) {
			z[j] = x[j] + y[j];
		}
	}
	return ans;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
	bool first = true;
	s << "[ ";
	for (int i = 0; i < rhs.getRows(); i++) {
		if (first) {
			first = false;
		} else {
			s << ",\n";
		}
		s << rhs[i];
	}
	s << " ]";
	return s;
}

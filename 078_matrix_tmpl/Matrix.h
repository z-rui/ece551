#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>
#include <iostream>
#include <vector>
#include <cstdlib>


//YOUR CODE GOES HERE!
template <typename T>
class Matrix {
private:
	int nRows, nCols;
	std::vector<std::vector<T> > rows;
public:
	Matrix() : nRows(0), nCols(0), rows(0) {}
	Matrix(int r, int c);
	Matrix(const Matrix<T> &m) : nRows(m.nRows), nCols(m.nCols), rows(m.rows) {}
	//~Matrix() {}

	int getRows() const {return nRows;}
	int getColumns() const {return nCols;}

	std::vector<T>& operator [](int index);
	const std::vector<T>& operator [](int index) const;
	bool operator ==(const Matrix<T>& m);
	Matrix<T> operator +(const Matrix<T>& m);
	//Matrix& operator =(const Matrix<T>& m);
	template <typename U>
	friend std::ostream& operator <<(std::ostream& s, const Matrix<U>& m);
};

template <typename T>
Matrix<T>::Matrix(int r, int c) : nRows(r), nCols(c), rows(r) {
	for (typename std::vector<std::vector<T> >::iterator it = rows.begin(); it != rows.end(); ++it) {
		it->resize(c, T());
	}
}

template <typename T>
std::vector<T>& Matrix<T>::operator [](int index) {
	assert(0 <= index && (size_t) index < rows.size());
	return rows[index];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator [](int index) const {
	assert(0 <= index && (size_t) index < rows.size());
	return rows[index];
}

template <typename T>
bool Matrix<T>::operator ==(const Matrix<T>& m) {
	return nRows == m.nRows && nCols == m.nCols && rows == m.rows;
}

template <typename T>
Matrix<T> Matrix<T>::operator +(const Matrix<T>& m) {
	assert(nRows == m.nRows && nCols == m.nCols);
	Matrix<T> ans(nRows, nCols);
	typename std::vector<std::vector<T> >::iterator z;
	typename std::vector<std::vector<T> >::const_iterator x = rows.begin(), y = m.rows.begin();
	for (z = ans.rows.begin(); z != ans.rows.end(); ++z, ++x, ++y) {
		typename std::vector<T>::iterator zz = z->begin();
		typename std::vector<T>::const_iterator xx = x->begin(), yy = y->begin();
		while (zz != z->end()) {
			*zz++ = *xx++ + *yy++;
		}
	}
	return ans;
}

#if 0
template <typename T>
Matrix<T>& Matrix<T>::operator =(const Matrix<T>& m) {
	rows = m.rows;
	nRows = m.nRows;
	nCols = m.nCols;
	return *this;
}
#endif

template <typename T>
std::ostream& operator <<(std::ostream& s, const Matrix<T>& m)
{
	s << "[ ";
	for (typename std::vector<std::vector<T> >::const_iterator it = m.rows.begin(); it != m.rows.end(); ++it) {
		if (it != m.rows.begin()) {
			s << ",\n{";
		} else {
			s << "{";
		}
		for (typename std::vector<T>::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
			if (jt != it->begin()) {
				s << ", ";
			}
			s << *jt;
		}
		s << "}";
	}
	s << " ]";
	return s;
}

#endif

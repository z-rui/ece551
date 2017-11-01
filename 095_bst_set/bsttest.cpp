//CITE: this file is adopted from the previous assignment
#include "bstset.h"
#include <string>
#include <sstream>
#include <iostream>

class TestBstSet : public BstSet<std::string> {
	void dumpNode(const Node *n, std::ostream &os) const
	{
		os << '(';
		if (n != NULL) {
			os << n->key;
			dumpNode(n->left, os);
			dumpNode(n->right, os);
		}
		os << ')';
	}
public:
	friend std::ostream& operator<< (std::ostream &, const TestBstSet&);
};

std::ostream& operator<< (std::ostream &os, const TestBstSet &m)
{
	m.dumpNode(m.root, os);
	return os;
}

int main()
{
	TestBstSet m;
	static int seq[] = {
		3, 1, 4, 1, 5, 9, 2, 6, 5, 3,
		5, 8, 9, 7, 9, 3, 2, 3, 8, 4,
		6, 2, 6, 4, 3, 3, 8, 3, 2, 7,
		9, 5, 0, 2, 8, 8, 4, 1, 9, 7,
		1, 6, 9,
	-1};
	int n;
	for (int i = 0; (n = seq[i]) != -1; i++) {
		std::stringstream ss;
		ss << n;
		const std::string& s = ss.str();
		if (m.contains(s)) {
			m.remove(s);
		} else {
			m.add(s);
		}
		std::cout << m << std::endl;
	}
	{
		TestBstSet *evilCopy = new TestBstSet(m);
		Set<std::string> *p = evilCopy;

		std::cout << (*evilCopy = *evilCopy) << std::endl;
		delete p;
	}
	return 0;
}

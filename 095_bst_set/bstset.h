#include "set.h"
#include <exception>

//CITE: this code is adopted from the previous assignment
template <typename T>
class BstSet : public Set<T> {
protected:
	struct Node {
		T key;
		Node *left, *right;
		Node(const T &key)
			: key(key), left(NULL), right(NULL) {}
	};
	Node *root;
	struct Node **lookupNode(const T&) const;
	void copyTree(Node **, const Node *);
	void freeTree(Node *);
public:
	virtual void add(const T & key)
	{
		Node **n = lookupNode(key);
		if (*n == NULL) {
			*n = new Node(key);
		}
	}
	virtual bool contains(const T& key) const
	{
		Node **n = lookupNode(key);
		return *n != NULL;
	}
	virtual void remove(const T& key);
	BstSet() : root(NULL) {}
	BstSet(const BstSet& m) : root(NULL)
	{
		copyTree(&root, m.root);
	}
	const BstSet &operator=(const BstSet& m)
	{
		if (&m != this) {
			BstSet temp = m;
			Node *t = root;
			root = temp.root;
			temp.root = t;
		}
		return *this;
	}
	virtual ~BstSet() { freeTree(root); }
};

template <typename T>
typename BstSet<T>::Node **
BstSet<T>::lookupNode(const T &key) const
{
	Node **n = const_cast<Node **>(&root);
	while (*n != NULL) {
		if (key < (*n)->key) {
			n = &(*n)->left;
		} else if ((*n)->key < key) {
			n = &(*n)->right;
		} else {
			break;
		}
	}
	return n;
}

template <typename T>
void BstSet<T>::remove(const T& key)
{
	Node **n = lookupNode(key);
	Node *p = *n;

	if (p == NULL) {
		return;	// this time not throw ???
	}
	if (p->left == NULL) {
		*n = p->right;
	} else {
		Node **victim = &p->left;
		while ((*victim)->right != NULL) {
			victim = &(*victim)->right;
		}
		*n = *victim;
		*victim = (*victim)->left;
		(*n)->left = p->left;
		(*n)->right = p->right;
	}

	delete p;
}

template <typename T>
void BstSet<T>::copyTree(Node **n, const Node *m)
{
	if (m != NULL) {
		*n = new Node(m->key);
		copyTree(&(*n)->left, m->left);
		copyTree(&(*n)->right, m->right);
	}
}

template <typename T>
void BstSet<T>::freeTree(Node *n)
{
	if (n == NULL) {
		return;
	}
	freeTree(n->left);
	freeTree(n->right);
	delete n;
}

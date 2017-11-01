#include "map.h"
#include <exception>

template <typename K, typename V>
class BstMap : public Map<K,V> {
protected:
	struct Node {
		K key;
		V value;
		Node *left, *right;
		Node(const K &key, const V &value)
			: key(key), value(value), left(NULL), right(NULL) {}
	};
	Node *root;
	struct Node **lookupNode(const K&) const;
	void copyTree(Node **, const Node *);
	void freeTree(Node *);
public:
	virtual void add(const K & key, const V & value)
	{
		Node **n = lookupNode(key);
		if (*n == NULL) {
			*n = new Node(key, value);
		} else {
			(*n)->value = value;
		}
	}
	virtual const V & lookup(const K& key) const
		throw (std::invalid_argument)
	{
		Node **n = lookupNode(key);
		if (*n == NULL) {
			throw std::invalid_argument("key does not exist");
		}
		return (*n)->value;
	}
	virtual void remove(const K& key);
	BstMap() : root(NULL) {}
	BstMap(const BstMap& m) : root(NULL)
	{
		copyTree(&root, m.root);
	}
	const BstMap &operator=(const BstMap& m)
	{
		if (&m != this) {
			BstMap temp = m;
			Node *t = root;
			root = temp.root;
			temp.root = t;
		}
		return *this;
	}
	virtual ~BstMap() { freeTree(root); }
};

template <typename K, typename V>
typename BstMap<K,V>::Node **
BstMap<K,V>::lookupNode(const K &key) const
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

template <typename K, typename V>
void BstMap<K,V>::remove(const K& key)
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

template <typename K, typename V>
void BstMap<K,V>::copyTree(Node **n, const Node *m)
{
	if (m != NULL) {
		*n = new Node(m->key, m->value);
		copyTree(&(*n)->left, m->left);
		copyTree(&(*n)->right, m->right);
	}
}

template <typename K, typename V>
void BstMap<K,V>::freeTree(Node *n)
{
	if (n == NULL) {
		return;
	}
	freeTree(n->left);
	freeTree(n->right);
	delete n;
}

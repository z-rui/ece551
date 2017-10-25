#ifndef _LL_H_
#define _LL_H_
#include <cstdlib>
#include <exception>
#include <assert.h>

//YOUR CODE GOES HERE
template <typename T>
class LinkedList {
private:
	class Node {
	public:
		T data;
		Node *next, *prev;
		Node(T data) : data(data), next(NULL), prev(NULL) {}
	};
	Node *head, *tail;
	class IndexError : public std::exception {
		const char *what() { return "Index out of boundd"; }
	};
protected:
	void destroy();
	void append(const LinkedList&);
	void link(Node *prev, Node *next);
	T& index(int) const;
public:
	void addFront(const T&);
	void addBack(const T&);
	bool remove(const T&);
	T& operator[] (int i) { return index(i); }
	const T& operator[] (int i) const { return index(i); }
	int find(const T&) const;
	int getSize() const;
	LinkedList() : head(NULL), tail(NULL) {}
	LinkedList(const LinkedList &x) : head(NULL), tail(NULL) { append(x); }
	LinkedList& operator= (const LinkedList &x) {
		destroy();
		head = tail = NULL;
		append(x);
		return *this;
	}
	~LinkedList() { destroy(); }
};

template <typename T>
void LinkedList<T>::append(const LinkedList &x)
{
	for (Node *p = x.head; p != NULL; p = p->next) {
		addBack(p->data);
	}
}

template <typename T>
void LinkedList<T>::destroy()
{
	Node *next;
	for (Node *p = head; p != NULL; p = next) {
		next = p->next;
		delete p;
	}
}

template <typename T>
int LinkedList<T>::getSize() const
{
	int size = 0;
	for (Node *p = head; p != NULL; p = p->next) {
		++size;
	}
	return size;
}

template <typename T>
int LinkedList<T>::find(const T &v) const
{
	int index = 0;
	Node *p;
	for (p = head; p != NULL && !(p->data == v); p = p->next) {
		++index;
	}
	return (p == NULL) ? -1 : index;
}

template <typename T>
void LinkedList<T>::addFront(const T &v)
{
	Node *node = new Node(v);
	link(node, head);
	link(NULL, node);
}

template <typename T>
void LinkedList<T>::addBack(const T &v)
{
	Node *node = new Node(v);
	link(tail, node);
	link(node, NULL);
}

template <typename T>
bool LinkedList<T>::remove(const T &v)
{
	Node *p;
	for (p = head; p != NULL && !(p->data == v); p = p->next) {
		/* slack off */;
	}
	if (p == NULL) {
		return false;
	}
	link(p->prev, p->next);
	delete p;
	return true;
}

template <typename T>
void LinkedList<T>::link(Node *prev, Node *next)
{
	Node **llink, **rlink;
	llink = (prev == NULL) ? &head : &prev->next;
	rlink = (next == NULL) ? &tail : &next->prev;
	*llink = next;
	*rlink = prev;
}

template <typename T>
T& LinkedList<T>::index(int i) const
{
	Node *p;

	for (p = head; p != NULL && i > 0; p = p->next) {
		--i;
	}
	if (p == NULL)
		throw IndexError();
	return p->data;
}


#endif

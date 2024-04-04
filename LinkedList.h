#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
#include"Node.h"

template <class T> class LinkedList
{
	Node<T>* head, * tail;
	char arch[260] = "";

	Node<T>* partition(Node<T>* l, Node<T>* h);
public:

	LinkedList(char* nomArch);

	~LinkedList();

	Node<T>* getHead();

	Node<T>* getTail();

	void swap(Node<T>* a, Node<T>* b);

	Node<T>* findNodeId(short id);

	void _quickSort(Node<T>* l, Node<T>* h);

	void add(T info, short id);

	void deleteNode(Node<T>* aux);

};


#endif // !_LINKEDLIST_H


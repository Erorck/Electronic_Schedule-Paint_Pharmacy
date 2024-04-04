#ifndef _NODE_H
#define _NODE_H

#include"NodeEsp.h"
#include"NodePac.h"
#include"NodeCita.h"

template <typename T>
class LinkedList;

template <class T> class Node
{
	friend class LinkedList<T>;

	T data;
	short id;
	Node* prev;
	Node* next;

public:
	void setData(T data);

	T getData();

	void setId(short id);

	short getId();

	Node<T>* getNext();

	Node<T>* getPrev();
};

#endif 


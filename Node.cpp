#include"Node.h"
#include<iostream>

using namespace std;

template <class T>
void Node<T>::setData(T data) { this->data = data; };

template <class T>
T Node<T>::getData() { return this->data; };

template <class T>
void Node<T>::setId(short id) { this->id = id; };

template <class T>
short Node<T>::getId() { return this->id; };

template <class T>
Node<T>* Node<T>::getNext() { return this->next; };

template <class T>
Node<T>* Node<T>::getPrev() { return this->prev; };


template Node<NodeEsp>;
template Node<NodePac>;
template Node<NodeCita>;
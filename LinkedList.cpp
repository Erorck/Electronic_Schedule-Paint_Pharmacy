#include"LinkedList.h"
#include<iostream>
#include<string.h>
#include<windows.h>
#include<fstream>

using namespace std;

template <class T>
LinkedList<T>::LinkedList(char* nomArch)
{
	ifstream archivoLL;
	head = NULL;
	tail = NULL;
	strcpy(arch, nomArch);
	
	archivoLL.open(nomArch, ios::in | ios::binary | ios::ate);
	
	if (!archivoLL.is_open()) {
		MessageBox(0, "NO SE HA PODIDO ABRIR EL ARCHIVO", nomArch, MB_ICONEXCLAMATION);
		return;
	}
	int size = archivoLL.tellg();
	if (size <= 0) { 
		MessageBox(0, "EL ARCHIVO ESTA VACIO", nomArch, MB_ICONEXCLAMATION);
		return;
	}
	for (int i = 0; i < (size / sizeof(Node<T>)); i++) {

		Node<T>* Temp = new Node<T>;

		archivoLL.seekg(i * sizeof(Node<T>));

		archivoLL.read(reinterpret_cast<char*>(Temp), sizeof(Node<T>));

		add(Temp->getData(), Temp->getId());

		delete reinterpret_cast<char*>(Temp);


	}
	archivoLL.close();
	MessageBox(0, "CARGADO DE DATOS EXITOSO", nomArch, MB_OK);
}

template <class T>
LinkedList<T>::~LinkedList()
{
	ofstream archivoLL;
	archivoLL.open(arch, ios::out | ios::trunc | ios::binary);

	if (!archivoLL.is_open()) { 
		MessageBox(0, "NO SE HA PODIDO ABRIR EL ARCHIVO PARA GUARDAR", arch, MB_ICONEXCLAMATION);
		return;
	}
	Node<T>* temp = head;
	while (temp != NULL) {
		archivoLL.write(reinterpret_cast<char*>(temp), sizeof(Node<T>));
		temp = temp->next;
	}
	archivoLL.close(); 

	temp = head;
	while (temp != NULL) {
		Node<T>* next = temp->next;
		delete temp;
		temp = next;
	}

	MessageBox(0, "SE HA GUARDADO CORRECTAMENTE EN EL ARCHIVO", arch, MB_OK);
	return;
}

template <class T>
Node<T>* LinkedList<T>::getHead() { return this->head; }

template <class T>
Node<T>* LinkedList<T>::getTail() { return this->tail; }

template <class T>
void LinkedList<T>::swap(Node<T>* a, Node<T>* b) {
	Node<T> t;
	t.data = a->data;
	t.id = a->id;
	a->data = b->data;
	a->id = b->id;
	b->data = t.data;
	b->id = t.id;
}

template <class T>
Node<T>* LinkedList<T>::findNodeId(short id) {
	Node<T>* aux = head;
	while (aux != NULL) {
		if (aux->getId() == id)
			return aux;
		aux = aux->next;
	}
	return 0;
};

template <class T>
Node<T>* LinkedList<T>::partition(Node<T>* l, Node<T>* h)
{
	T x = h->data; // Definimos nuestro pivote x con la información de h(head)

	Node<T>* i = l->prev; //Definimos i como el penultimo miembro del arreglo

	for (Node<T>* j = l; j != h; j = j->next) //Iniciamos desde el primer último elemento e iteramos mientras j no llegue a h(head)
	{
		if (j->data <= x) //Si el elemento actual es menor al pivote entonces
		{
			 //si el elemento anterior a l(last) es nulo, le asignamos el último elemento, caso contrario le asignamos el siguiente a si mismo
			i = (i == NULL) ? l : i->next;
			swap(i, j); //Intercambiamos el elemento i con el j
		}
	}
	//si el elemento anterior a l(last) es nulo, le asignamos el último elemento, caso contrario le asignamos el siguiente a si mismo
	i = (i == NULL) ? l : i->next;
	//Intercambiamos el elemento i con el h(head)
	swap(i, h);
	//retornamos el elemento i, ahora con el valor de h(head)
	return i;
}

template <class T>
void LinkedList<T>::_quickSort(Node<T>* l, Node<T>* h) //Método de ordenamiento quicksort
{
	if (h != NULL && l != h && l != h->next) //Validamos que h(head) no sea nulo, que l(last) no sea igual a h(head), y que l(last) no sea el siguiente elemento a h(head)
	{
		Node<T>* p = partition(l, h);
		//Dividimos la lista en dos sublistas tomando como referencia el pivote, posicionando todos los elementos menores a la izquierda y los mayores 
		//a la derecha, retornando el elemento que divide ambas listas
		_quickSort(l, p->prev);//Repetimos el proceso, con la primera sublista de la división anterior, donde l(last) es el último y p-prev es la cabeza 
		_quickSort(p->next, h);//Repetimos el proceso, con la segunda sublista de la división anterior, donde p-next es el último y h es la cabeza
	}
}

template <class T>
void LinkedList<T>::add(T info, short id)
{
	if (head == NULL) 
	{
		head = new Node<T>; 
		head->data = info;
		head->next = NULL;
		head->prev = NULL;
		head->id = id;
		tail = head;
	}
	else 
	{
		Node<T>* temp = new Node<T>;
		temp->data = info;
		temp->next = NULL;
		temp->prev = tail;
		tail->next = temp;
		tail = tail->next;
		tail->id = id;
	}
}

template <class T>
void LinkedList<T>::deleteNode(Node<T>* aux) {
	if (aux->prev == NULL && aux->next == NULL) {
		delete aux;
		head = NULL;
		aux = NULL;
	}
	else if (aux == tail) {
		aux->prev->next = NULL;
		tail = tail->prev;
		delete aux;
	}
	else if (aux == head) {
		head = head->next;
		head->prev = NULL;
		delete aux;
	}
	else { 
		aux->next->prev = aux->prev;
		aux->prev->next = aux->next;
		delete aux;
	}
}

template LinkedList<NodeEsp>;
template LinkedList<NodePac>;
template LinkedList<NodeCita>;
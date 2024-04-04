#include "TreeMed.h"
#include<iostream>
#include<string.h>
#include<windows.h>

using namespace std;


TreeMed::TreeMed() {
	ifstream archivoTree;
	raiz = 0;
	archivoTree.open("ArbolMed.dat", ios::in | ios::binary | ios::ate);
	
	if (!archivoTree.is_open()) {
		MessageBox(0, "NO SE HA PODIDO ABRIR EL ARCHIVO PARA GUARDAR", "ArbolMed.dat", MB_ICONEXCLAMATION);
		return;
	}
	int size = archivoTree.tellg();
	if (size <= 0) { 
		MessageBox(0, "ARCHIVO DE MÉDICOS VACIO", "ArbolMed.dat", MB_ICONEXCLAMATION);
		return;
	}
	else for (int i = 0; i < (size / sizeof(NodeMed)); i++) {

		NodeMed* Temp = new NodeMed;
		NodeMed data;
		
		archivoTree.seekg(i * sizeof(NodeMed));

		archivoTree.read(reinterpret_cast<char*>(Temp), sizeof(NodeMed));

		copyData(&data, Temp);
		addNode(getRaizDir(), data);
		
		delete reinterpret_cast<char*>(Temp);
	}
	archivoTree.close();
	MessageBox(0, "MEDICOS CARGADOS", "ArbolMed.dat", MB_OK);
}

TreeMed::~TreeMed() {
	ofstream archivoTree;
	archivoTree.open("ArbolMed.dat", ios::out | ios::trunc | ios::binary);

	if (!archivoTree.is_open()) { 
		MessageBox(0, "NO SE HA PODIDO ABRIR EL ARCHIVO PARA GUARDAR LOS MEDICOS", "ArbolMed.dat", MB_ICONEXCLAMATION);
		return;
	}

	guardarMed(getRaiz(), &archivoTree);

	archivoTree.close(); 

	MessageBox(0, "SE HAN GUARDADO LOS MEDICOS EN EL ARCHIVO", "ArbolMed.dat", MB_OK);
	return;
}

short TreeMed::getContMed() { return this->contMed; }

NodeMed* TreeMed::getRaiz() { return this->raiz; }

NodeMed** TreeMed::getRaizDir() { return &(this->raiz); }

NodeMed** TreeMed::getAuxMed() { return this->auxMed; }

void TreeMed::copyNode(NodeMed* node, NodeMed data) {
	node->cedula = data.cedula;
	strcpy(node->nombre, data.nombre);
	node->noCon = data.noCon;
	node->especialidad = data.especialidad;
	node->h1 = data.h1;
	node->h2 = data.h2;
	node->m1 = data.m1;
	node->m2 = data.m2;
	strcpy(node->tel, data.tel);
	for (int i = 0; i < 7; i++)
		node->horario[i] = data.horario[i];
	strcpy(node->dirFoto, data.dirFoto);
	strcpy(node->usuario, data.usuario);
	strcpy(node->mostrar, data.mostrar);
}

void TreeMed::copyData(NodeMed* data, NodeMed* node) {
	data->cedula = node->cedula;
	strcpy(data->nombre, node->nombre);
	data->noCon = node->noCon;
	data->especialidad = node->especialidad;
	data->h1 = node->h1;
	data->h2 = node->h2;
	data->m1 = node->m1;
	data->m2 = node->m2;
	strcpy(data->tel, node->tel);
	for (int i = 0; i < 7; i++)
		data->horario[i] = node->horario[i];
	strcpy(data->dirFoto, node->dirFoto);
	strcpy(data->usuario, node->usuario);
	strcpy(data->mostrar, node->mostrar);
}

void TreeMed::swap(NodeMed* n1, NodeMed* n2) {
	NodeMed* t = new NodeMed;

	copyData(t, n1);
	copyData(n1, n2);
	copyData(n2, t);

	delete t;
}

void TreeMed::addNode(NodeMed** node, NodeMed data) {
	NodeMed* nTemp = new NodeMed;
	copyNode(nTemp, data);

	if (*node == NULL) {
		nTemp->izq = NULL;
		nTemp->der = NULL;
		nTemp->dad = NULL;
		*node = nTemp;
		auxMed[contMed] = *node;
		contMed++;
	}
	else {
		add(node, nTemp);
	}
}

void TreeMed::add(NodeMed** node, NodeMed* newNode) {
	NodeMed* aux = *node;
	if (aux == NULL) {
		newNode->izq = NULL;
		newNode->der = NULL;
		newNode->dad = NULL;
		*node = newNode;

		auxMed[contMed] = *node;
		contMed++;
	}
	else {
		if (newNode->cedula > aux->cedula)
			add(&(aux->der), newNode);
		if (newNode->cedula < aux->cedula)
			add(&(aux->izq), newNode);
	}
}

bool TreeMed::cedRepetido(NodeMed* node, int ced) {
	if (node != NULL) {
		if (node->cedula == ced)
			return true;
		else {
			if (ced > node->cedula)
				return cedRepetido(node->der, ced);
			if (ced < node->cedula)
				return cedRepetido(node->izq, ced);
		}
	}
	else
		return false;
}

NodeMed* TreeMed::findNodeCed(NodeMed* node, int ced) {
	if (node != NULL) {
		if (node->cedula == ced)
			return node;
		else {
			if (ced > node->cedula)
				return findNodeCed(node->der, ced);
			if (ced < node->cedula)
				return findNodeCed(node->izq, ced);
		}
	}
	else
		return 0;
}

NodeMed* TreeMed::findNodeName(NodeMed* node, char* name) {
	if (node != NULL) {
		if (strcmp(node->nombre, name) == 0)
			return node;
		else {
			NodeMed* aM;
			aM = findNodeName(node->izq, name);
			if (aM == 0)
				return findNodeName(node->der, name);
			else
				return aM;
		}
	}
	else
		return 0;
}

int TreeMed::binSearchCed(NodeMed arr[], int l, int r, int x) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		if (arr[mid].cedula == x)
			return mid;
		if (arr[mid].cedula > x) 
			return binSearchCed(arr, l, mid - 1, x);
		return binSearchCed(arr, mid + 1, r, x);
	} 
	return -1;
}

void TreeMed::guardarMed(NodeMed* node, ofstream *archivoTree) {
	if (node != NULL) {
		archivoTree->write(reinterpret_cast<char*>(node), sizeof(NodeMed));
		guardarMed(node->izq, archivoTree);
		guardarMed(node->der, archivoTree);
	}
}

NodeMed* TreeMed::atencionTraspapelada(NodeMed* node, NodeMed data) {
	if (node != NULL) {
		if (node->cedula != data.cedula) {
			if (node->noCon == data.noCon) {
				for (int i = 0; i < 7; i++) {
					if (node->horario[i] && data.horario[i]) {
						if (horasTraspapeladas(node->h1, node->m1, node->h2, node->m2, data.h1, data.m1, data.h2, data.m2))
							return node;
						else
							break;
					}
				}
			}
		}
		NodeMed* aM;
		aM = atencionTraspapelada(node->izq, data);
		if (aM == 0)
			return atencionTraspapelada(node->der, data);
		else
			return aM;
	}
	else
		return 0;
}

bool TreeMed::horasTraspapeladas(short hi1, short mi1, short hf1, short mf1, short hi2, short mi2, short hf2, short mf2) {
	if (hi1 == hi2 && hf1 == hf2)
		return true;

	if (hi1 == hf1) {
		if (hi2 == hf1) {
			if (mi2 < mf1)
				return true;
		}
		if (hf2 == hi1) {
			if (mf2 > mi1)
				return true;
		}
	}
	else if (hi2 == hf2) {
		if (hi1 == hf2) {
			if (mi1 < mf2)
				return true;
		}
		if (hf1 == hi2) {
			if (mf1 > mi2)
				return true;
		}
	}
	
	if (hi1 < hi2) {
		for (int j = hi1; j < hf1 + 1; j++) {
			if (j == hi2) {
				if (mi1 < mf1) {
					for (int i = mi1; i < mf1 + 20; i += 20) {
						if (i > mi2)
							return true;
					}
				}
				else {
					for (int i = mf1; i < mi1 + 20; i += 20) {
						if (i > mi2)
							return true;
					}
				}
			}
			if (j > hi2)
				return true;
		}
	}
	else {
		for (int j = hi2; j < hf2 + 1; j++) {
			if (j == hi1) {
				if (mi2 < mf2) {
					for (int i = mi2; i < mf2 + 20; i += 20) {
						if (i > mi1)
							return true;
					}
				}
				else {
					for (int i = mf2; i < mi2 + 20; i += 20) {
						if (i > mi1)
							return true;
					}
				}
			}
			if (j > hi1)
				return true;
		}
	}
	return false;
}

void TreeMed::parentLinker(NodeMed* node) {
	if (node != NULL) {
		if (node->izq != NULL)
			node->izq->dad = node;
		if (node->der != NULL)
			node->der->dad = node;
		parentLinker(node->izq);
		parentLinker(node->der);
	}
}

NodeMed* TreeMed::lefty(NodeMed* node) {
	if (node == NULL)
		return NULL;
	if (node->izq)
		return lefty(node->izq);
	else
		return node;
}

void TreeMed::replace(NodeMed* aux, NodeMed* newNode) {
	if (aux->dad) {
		if (aux->dad->izq != NULL) {
			if (aux->cedula == aux->dad->izq->cedula)
				aux->dad->izq = newNode;
			else if (aux->cedula == aux->dad->der->cedula)
				aux->dad->der = newNode;
		}
		else if (aux->cedula == aux->dad->der->cedula)
			aux->dad->der = newNode;
	}
	if (newNode) {
		newNode->dad = aux->dad;
	}
}

void TreeMed::deleteNodeTree(NodeMed* aux) {
	if (aux->izq && aux->der) {
		NodeMed* lessy = lefty(aux->der);
		copyNode(aux, *lessy);
		deleteNodeTree(lessy);
	}
	else if (aux->izq) {
		replace(aux, aux->izq);
		if (raiz == aux)
			raiz = aux->izq;
		delete aux;
	}
	else if (aux->der) {
		replace(aux, aux->der);
		if (raiz == aux)
			raiz = aux->der;
		delete aux;
	}
	else {
		replace(aux, NULL);
		if (raiz == aux)
			raiz = NULL;
		delete aux;
	}
}

void TreeMed::contNodes(NodeMed* node, NodeMed* aux[]) {
	if (node != NULL) {
		aux[contMed] = node;
		contMed++;

		contNodes(node->izq, aux);
		contNodes(node->der, aux);
	}
}

void TreeMed::resetAuxMed(NodeMed* auxMed[], short conMed) {
	for (int i = 0; i < conMed; i++) {
		auxMed[i] = NULL;
	}
	this->contMed = 0;
	contNodes(getRaiz(), auxMed);
}

void TreeMed::ordBurb(NodeMed* arr[], NodeMed arrTemp[], int n) {
	int i, j;

	for (i = 0; i < n; i++) {
		copyNode(&arrTemp[i], *arr[i]);
	}

	bool swapped;
	for (i = 0; i < n - 1; i++) {
		swapped = false; 
		for (j = 0; j < n - i - 1; j++) {
			if (arrTemp[j].cedula > arrTemp[j + 1].cedula) { 
				swap(&arrTemp[j], &arrTemp[j + 1]);
				swapped = true; 
			}
		}
		if (swapped == false) 
			break; 
	} 
}

void TreeMed::heapify(NodeMed arr[], int n, int i) {
	int largest = i; //Inicializamos el elemento más grande con el indice proporcionado en el parametro "i"
	int l = 2 * i + 1; //Incializamos los indices de los hijos del nodo "i"
	int r = 2 * i + 2; // l de left(izquierda), r de right(derecha)

	if (l < n && arr[l] > arr[largest]) //Si el hijo izquierdo es mayor, asignamos su indice a largest
		largest = l;

	if (r < n && arr[r] > arr[largest]) //Si el hijo derecho es mayor, asignamos su indice a largest
		largest = r;

	if (largest != i) //Si largest ha cambiado de valor, 
	{
		swap(&arr[i], &arr[largest]); //entonces intercambiamos el nodo<i> con el nodo<largest>
		heapify(arr, n, largest); //seguimos iterando, pero ahora con el indice alamacenado en largest
	}
}

void TreeMed::heapSort(NodeMed arr[], int n) {//Método de ordemiento Heapsort. Parametros: Arreglo de elementos a ordenar y cantidad de elementos en dicho arreglo
	
	for (int i = n / 2 - 1; i >= 0; i--) //Primer fase del método: Creación del montículo
		heapify(arr, n, i); //Iniciamos en el último elemento justo en la mitad del arreglo

	for (int i = n - 1; i >= 0; i--) //Segunda fase: Extracción de la cima del montículo
	{//Iniciamos en el último elemento del arreglo, ahora ordenado en montículos
		swap(&arr[0], &arr[i]); //Intercambiamos el nodo<0> con el nodo<i> (Extraemos la cima)
		heapify(arr, i, 0); //Creamos un monticulo alrededor del último elemento
	}
}
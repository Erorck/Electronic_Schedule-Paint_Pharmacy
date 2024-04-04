#ifndef _TREEMED_H
#define _TREEMED_H

#include"NodeMed.h"
#include<fstream>

class TreeMed {
private:
	NodeMed* raiz;
	short contMed = 0;
	NodeMed* auxMed[' '] = { 0 };

	void swap(NodeMed* n1, NodeMed* n2);

	void add(NodeMed** node, NodeMed* newNode);

	NodeMed* lefty(NodeMed* node);
	
	void replace(NodeMed* aux, NodeMed* newNode);

	void heapify(NodeMed arr[], int n, int i);

	void guardarMed(NodeMed* node,std::ofstream *archivoTree);

public:

	TreeMed();

	~TreeMed();

	short getContMed();

	NodeMed* getRaiz();

	NodeMed** getAuxMed();
	
	NodeMed** getRaizDir();

	void copyNode(NodeMed* node, NodeMed data);

	void copyData(NodeMed* data, NodeMed* node);

	void addNode(NodeMed** node, NodeMed data);

	bool cedRepetido(NodeMed* node, int ced);

	NodeMed* findNodeCed(NodeMed* node, int ced);

	NodeMed* findNodeName(NodeMed* node, char* name);

	int binSearchCed(NodeMed arr[], int l, int r, int x);

	NodeMed* atencionTraspapelada(NodeMed* node, NodeMed data);

	bool horasTraspapeladas(short hi1, short mi1, short hf1, short mf1, short hi2, short mi2, short hf2, short mf2);

	void parentLinker(NodeMed* node);

	void deleteNodeTree(NodeMed* aux);

	void contNodes(NodeMed* node, NodeMed* aux[]);

	void resetAuxMed(NodeMed* auxMed[], short conMed);

	void ordBurb(NodeMed* arr[],NodeMed arrTemp[], int n);

	void heapSort(NodeMed arr[], int n);
};

#endif // !_TREEMED_H


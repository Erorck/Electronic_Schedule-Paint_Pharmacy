#ifndef _NODE_PAC_H
#define _NODE_PAC_H

class NodePac {
public:
	char nombre[45];
	char apellido[45];
	char mostrar[100];
	char genero[10];
	char tel[15];
	char ref[80];
	short dia;
	short mes;
	short anio;
	short edad;
	int primerMed;
	char usuario[15];

	bool operator >(const NodePac& paciente);

	bool operator >=(const NodePac& paciente);

	bool operator <(const NodePac& paciente);

	bool operator <=(const NodePac& paciente);

};

#endif // !_NODE_PAC_H


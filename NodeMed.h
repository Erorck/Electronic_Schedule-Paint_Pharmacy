#ifndef _NODE_MED_H
#define _NODE_MED_H

class NodeMed {
public:
	int cedula;
	char nombre[55];
	short noCon;
	short especialidad;
	short h1;
	short m1;
	short h2;
	short m2;
	char tel[15];
	bool horario[7] = { false };
	char usuario[15];
	char dirFoto[260] = "";
	char mostrar[60];
	NodeMed* der = 0;
	NodeMed* izq = 0;
	NodeMed* dad = 0;

	bool operator >(const NodeMed& med);

	bool operator >=(const NodeMed& med);

	bool operator <(const NodeMed& med);

	bool operator <=(const NodeMed& med);
};

#endif // !_NODE_MED_H


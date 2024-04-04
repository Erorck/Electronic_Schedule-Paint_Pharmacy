#ifndef _NODE_CITA_H
#define _NODE_CITA_H
#include<windows.h>

class NodeCita {
public:
	short paciente;
	int medico;
	SYSTEMTIME fecha;
	char tel[15];
	short hora;
	short min;
	char usuario[15];
	char mostrar[120];
	bool cancelada = false;
	char motCanc[50] = "";

	bool operator >(const NodeCita& cita);

	bool operator >=(const NodeCita& cita);

	bool operator <(const NodeCita& cita);

	bool operator <=(const NodeCita& cita);
};



#endif // !_NODE_CITA_H


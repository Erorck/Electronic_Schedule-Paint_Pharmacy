#include "NodeCita.h"
#include<string.h>

bool NodeCita::operator >(const NodeCita& cita) {
	if (strcmp(this->mostrar, cita.mostrar) > 0)
		return true;
	else
		return false;
}

bool NodeCita::operator >=(const NodeCita& cita) {
	if (strcmp(this->mostrar, cita.mostrar) >= 0)
		return true;
	else
		return false;
}

bool NodeCita::operator <(const NodeCita& cita) {
	if (strcmp(this->mostrar, cita.mostrar) < 0)
		return true;
	else
		return false;
}

bool NodeCita::operator <=(const NodeCita& cita) {
	if (strcmp(this->mostrar, cita.mostrar) <= 0)
		return true;
	else
		return false;
}

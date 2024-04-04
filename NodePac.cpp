#include "NodePac.h"
#include<string.h>

bool NodePac::operator >(const NodePac& paciente) {
	if (strcmp(this->apellido, paciente.apellido) > 0)
		return true;
	else
		return false;
}

bool NodePac::operator >=(const NodePac& paciente) {
	if (strcmp(this->apellido, paciente.apellido) >= 0)
		return true;
	else
		return false;
}

bool NodePac::operator <(const NodePac& paciente) {
	if (strcmp(this->apellido, paciente.apellido) < 0)
		return true;
	else
		return false;
}

bool NodePac::operator <=(const NodePac& paciente) {
	if (strcmp(this->apellido, paciente.apellido) <= 0)
		return true;
	else
		return false;
}
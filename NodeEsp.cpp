#include"NodeEsp.h"
#include<string.h>

bool NodeEsp::operator >(const NodeEsp& esp) {
	if (strcmp(this->nombre, esp.nombre) > 0)
		return true;
	else
		return false;
}

bool NodeEsp::operator >=(const NodeEsp& esp) {
	if (strcmp(this->nombre, esp.nombre) >= 0)
		return true;
	else
		return false;
}

bool NodeEsp::operator <(const NodeEsp& esp) {
	if (strcmp(this->nombre, esp.nombre) < 0)
		return true;
	else
		return false;
}

bool NodeEsp::operator <=(const NodeEsp& esp) {
	if (strcmp(this->nombre, esp.nombre) <= 0)
		return true;
	else
		return false;
}
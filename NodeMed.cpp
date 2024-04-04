#include "NodeMed.h"

bool NodeMed::operator >(const NodeMed& med) {
	if (this->cedula > med.cedula)
		return true;
	else
		return false;
}

bool NodeMed::operator >=(const NodeMed& med) {
	if (this->cedula >= med.cedula)
		return true;
	else
		return false;
}

bool NodeMed::operator <(const NodeMed& med) {
	if (this->cedula < med.cedula)
		return true;
	else
		return false;
}

bool NodeMed::operator <=(const NodeMed& med) {
	if (this->cedula <= med.cedula)
		return true;
	else
		return false;
}
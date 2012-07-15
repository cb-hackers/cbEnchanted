#include "customfunction.h"

bool CustomFunction::operator > (const CustomFunction &o) const{
	if (this->groupId > o.groupId) return true;
	if (this->groupId < o.groupId) return false;
	if (this->funcId > o.funcId) return true;
	return false;
}
bool CustomFunction::operator < (const CustomFunction &o) const {
	if (this->groupId < o.groupId) return true;
	if (this->groupId > o.groupId) return false;
	if (this->funcId < o.funcId) return true;
	return false;
}

bool CustomFunction::operator ==(const CustomFunction &o) {
	return (o.groupId == this->groupId) && (this->funcId == o.funcId);
}

bool CustomFunction::operator !=(const CustomFunction &o) {
	return (o.groupId != this->groupId) || (this->funcId != this->funcId);
}

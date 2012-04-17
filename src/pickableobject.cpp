#include "pickableobject.h"

PickableObject::PickableObject() : object(0) {
	// ...
}

PickableObject::PickableObject(CBObject* obj, int32_t type) {
	object = obj;
	switch(type) {
		// ...
	}
}

void PickableObject::setPickType(int32_t type) {
	// ...
}

void PickableObject::doPick(CBObject *fromObj, float &returnX, float &returnY) {
	// ...
}

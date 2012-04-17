#ifndef PICKABLEOBJECT_H
#define PICKABLEOBJECT_H

#include "cbobject.h"

/** Holds one object that is specified with commandObjectPickable() */
class PickableObject
{
	public:
		PickableObject();
		PickableObject(CBObject* obj, int32_t type);
		void setPickType(int32_t type);
		void doPick(CBObject *fromObj, float &returnX, float &returnY);
	private:
		enum PickType {
			BoxPick,
			CirclePick,
			PixelPick
		};
		CBObject* object;
		PickType pickType;
};

#endif // PICKABLEOBJECT_H

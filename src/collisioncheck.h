#ifndef COLLISIONCHECK_H
#define COLLISIONCHECK_H
#include "precomp.h"
#include "cbobject.h"

class CollisionCheck
{
public:
	enum CollisionType {
		Box,
		Circle,
		Map,
		Pixel
	};

	CollisionCheck();
	bool isNull()const{return mObject1 == 0 || mObject2 == 0;}
	void setObjects(CBObject *a,CBObject *b);
	void setCollisionType1(CollisionType c){mCollisionType1 = c;}
	void setCollisionType2(CollisionType c){mCollisionType2 = c;}
private:
	CBObject *mObject1;
	CBObject *mObject2;
	CollisionType mCollisionType1,mCollisionType2;

};

#endif // COLLISIONCHECK_H

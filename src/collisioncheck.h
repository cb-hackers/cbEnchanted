#ifndef COLLISIONCHECK_H
#define COLLISIONCHECK_H
#include "precomp.h"
#include "cbobject.h"

class CollisionCheck
{
public:
	enum CollisionType {
		Box = 1,
		Circle,
		Pixel,
		Map
	};

	CollisionCheck();
	bool isNull()const{return mObject1 == 0 || mObject2 == 0;}
	void setObjects(CBObject *a,CBObject *b){mObject1 = a;mObject2 = b;}
	void setCollisionType1(CollisionType c){mCollisionType1 = c;}
	void setCollisionType2(CollisionType c){mCollisionType2 = c;}
	bool testCollision();
	void handleCollision();
private:
	CBObject *mObject1;
	CBObject *mObject2;
	CollisionType mCollisionType1,mCollisionType2;
	float mCollisionX, mCollisionY;
	float mCollisionAngle;
	uint16_t collisionCount;
	bool CircleRectTest();
};

#endif // COLLISIONCHECK_H

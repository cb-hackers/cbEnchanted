#ifndef COLLISIONCHECK_H
#define COLLISIONCHECK_H
#include "precomp.h"
#include "cbobject.h"

/** Handles collisions between two objects */
class CollisionCheck
{
	public:
		/** The type of collision set with SetupCollision */
		enum CollisionType {
			Box = 1,
			Circle,
			Pixel,
			Map
		};
		/** The way collision are handled */
		enum CollisionHandling {
			Stop = 1,
			Slide
		};

		/** An empty constructor */
		CollisionCheck();
		/** Sets the objects during construction */
		CollisionCheck(CBObject *object1, CBObject *object2);

		/** Check if either of the collision objects are null */
		bool isNull() const { return mObject1 == 0 || mObject2 == 0; }
		/** Sets the objects that take part in this collision */
		void setObjects(CBObject *a, CBObject *b) { mObject1 = a; mObject2 = b; }
		/** Sets the type of collision for the colliding object */
		void setCollisionType1(CollisionType c){ mCollisionType1 = c; }
		/** Sets the type of collision for the object to collide to */
		void setCollisionType2(CollisionType c){ mCollisionType2 = c; }

		/** Sets the way collision are handled */
		void setCollisionHandling(CollisionHandling h) { mCollisionHandling = h; }

		/** Tests the collision */
		bool testCollision();
		/** Handles the collision */
		void handleCollision();
	private:
		/** Object that has the collision set */
		CBObject *mObject1;
		/** Object to collide to */
		CBObject *mObject2;

		/** The type of collision, @see CollisionType */
		CollisionType mCollisionType1, mCollisionType2;
		/** The way collision is handled, @see CollisionHandling */
		CollisionHandling mCollisionHandling;

		/** Coordinates where the collision happened */
		float mCollisionX, mCollisionY;
		/** Angle of collision, in degrees */
		float mCollisionAngle;

		/** The amount of collisions detected */
		uint16_t collisionCount;

		/** A circle - rectangle collision test */
		bool CircleRectTest();
		/** A rectangle - circle collision test */
		bool RectCircleTest();
		/** A rectangle - rectangle collision test */
		bool RectRectTest();
		/** A circle - circle collision test */
		bool CircleCircleTest();
		/** A rectangle - map collision test */
		bool RectMapTest();
		/** A circle - map collision test */
		bool CircleMapTest();
};

#endif // COLLISIONCHECK_H

#include "precomp.h"
#include "collisioncheck.h"
#include <cmath>
#include "cbmap.h"
#include "debug.h"

/** An empty constructor */
CollisionCheck::CollisionCheck() : mObject1(0), mObject2(0) {

}

/** Sets the objects during construction */
CollisionCheck::CollisionCheck(CBObject *object1, CBObject *object2) {
	setObjects(object1, object2);
}

/** Sets the type of collision for the colliding object.
 *
 * If the type is of invalid value, both mObject1 and mObject2 properties are
 * assigned null values, so that the collision check will not be made.
 */
void CollisionCheck::setCollisionType1(uint16_t c) {
	switch (c) {
		case 1:
			mCollisionType1 = Box;
		break;
		case 2:
			mCollisionType1 = Circle;
		break;
		default:
			FIXME("Unsupported collision type %i set for the colliding object", c);
			mObject1 = 0;
			mObject2 = 0;
	}
}

/** Sets the type of collision for the object to collide to.
 *
 * If the type is of invalid value, both mObject1 and mObject2 properties are
 * assigned null values, so that the collision check will not be made.
 */
void CollisionCheck::setCollisionType2(uint16_t c) {
	switch (c) {
		case 1:
			mCollisionType2 = Box;
		break;
		case 2:
			mCollisionType2 = Circle;
		break;
		case 4:
			mCollisionType2 = Map;
		break;
		default:
			FIXME("Unsupported collision type %i set for the object to collide to", c);
			mObject1 = 0;
			mObject2 = 0;
	}
}

/** Tests the collision */
bool CollisionCheck::testCollision() {
	// Make sure we are not testing against null objects
	if (isNull()) {
		FIXME("Trying to test collision for null objects");
		return false;
	}

	bool collided = false;
	// What is the collision type for the colliding object
	switch (mCollisionType1) {
		case Box:
			switch (mCollisionType2) {
				case Box: collided = RectRectTest(); break;
				case Circle: collided = RectCircleTest(); break;
				case Map: collided = RectMapTest(); break;
			}
		break;
		case Circle:
			switch (mCollisionType2) {
				case Box: collided = CircleRectTest(); break;
				case Circle: collided = CircleCircleTest(); break;
				case Map: collided = CircleMapTest(); break;
			}
		break;
	}

	// If there was a collision, increment the collision count
	if (collided) {
		collisionCount++;
	}

	return collided;
}

/** Handles the collision */
void CollisionCheck::handleCollision() {

}

// ----------------------------------------------------------------------------
// Below are the private methods that calculate the collisions.
// All methods return true if there was a collision, false otherwise. It is up
// to testCollision() method to increase the count of collisions, not the
// methods below.
// ----------------------------------------------------------------------------

/** A circle - rectangle collision test */
bool CollisionCheck::CircleRectTest() {
	return false;
}

/** A rectangle - circle collision test */
bool CollisionCheck::RectCircleTest() {
	return false;
}

/** A rectangle - rectangle collision test */
bool CollisionCheck::RectRectTest() {
	return false;
}

/** A circle - circle collision test */
bool CollisionCheck::CircleCircleTest() {
	return false;
}

/** A rectangle - map collision test */
bool CollisionCheck::RectMapTest() {
	return false;
}

/** A circle - map collision test */
bool CollisionCheck::CircleMapTest() {
	return false;
}


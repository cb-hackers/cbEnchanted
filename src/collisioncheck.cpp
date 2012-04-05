#include "precomp.h"
#include "collisioncheck.h"
#include <cmath>
#include "cbmap.h"

/** An empty constructor */
CollisionCheck::CollisionCheck() : mObject1(0), mObject2(0) {

}

/** Sets the objects during construction */
CollisionCheck::CollisionCheck(CBObject *object1, CBObject *object2) {
	setObjects(object1, object2);
}

/** Tests the collision */
bool CollisionCheck::testCollision() {
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


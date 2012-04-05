#include "precomp.h"
#include "collisioncheck.h"
#include <cmath>
#include "cbmap.h"
#include "debug.h"
#include "cbenchanted.h"
#include "rendertarget.h"

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
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float r = mObject1->getSizeX() / 2;

	float x = mObject1->getX() - r;
	float y = mObject1->getY() + r;

	rendertarget->useWorldCoords(true);
	rendertarget->drawCircle(x, y, r, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}

/** A rectangle - circle collision test */
bool CollisionCheck::RectCircleTest() {
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float w = mObject1->getSizeX();
	float h = mObject1->getSizeY();

	float x = mObject1->getX() - w / 2;
	float y = mObject1->getY() + h / 2;

	rendertarget->useWorldCoords(true);
	rendertarget->drawBox(x, y, w, h, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}

/** A rectangle - rectangle collision test */
bool CollisionCheck::RectRectTest() {
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float w = mObject1->getSizeX();
	float h = mObject1->getSizeY();

	float x = mObject1->getX() - w / 2;
	float y = mObject1->getY() + h / 2;

	rendertarget->useWorldCoords(true);
	rendertarget->drawBox(x, y, w, h, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}

/** A circle - circle collision test */
bool CollisionCheck::CircleCircleTest() {
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float r = mObject1->getSizeX() / 2;

	float x = mObject1->getX();
	float y = mObject1->getY();

	rendertarget->useWorldCoords(true);
	rendertarget->drawCircle(x, y, r, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}

/** A rectangle - map collision test */
bool CollisionCheck::RectMapTest() {
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float w = mObject1->getSizeX();
	float h = mObject1->getSizeY();

	float x = mObject1->getX() - w / 2;
	float y = mObject1->getY() + h / 2;

	rendertarget->useWorldCoords(true);
	rendertarget->drawBox(x, y, w, h, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}

/** A circle - map collision test */
bool CollisionCheck::CircleMapTest() {
	// Draw collision boundaries
	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	float r = mObject1->getSizeX() / 2;

	float x = mObject1->getX();
	float y = mObject1->getY();

	rendertarget->useWorldCoords(true);
	rendertarget->drawCircle(x, y, r, false, al_map_rgb(255, 255, 255));
	rendertarget->useWorldCoords(false);

	// ...collision boundaries are now drawn.

	return false;
}


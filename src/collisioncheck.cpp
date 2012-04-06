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
	DrawCollisionBoundaries();

	return false;
}

/** A rectangle - circle collision test */
bool CollisionCheck::RectCircleTest() {
	DrawCollisionBoundaries();

	return false;
}

/** A rectangle - rectangle collision test */
bool CollisionCheck::RectRectTest() {
	DrawCollisionBoundaries();

	return false;
}

/** A circle - circle collision test */
bool CollisionCheck::CircleCircleTest() {
	DrawCollisionBoundaries();

	return false;
}

/** A rectangle - map collision test */
bool CollisionCheck::RectMapTest() {
	DrawCollisionBoundaries();

	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();




	// The current map is the object in mObject2
	CBMap *cbmap = static_cast<CBMap*>(mObject2);

	// Fetch tile width and tile height to variables so that the code is less messy
	int32_t tileWidth = cbmap->getTileWidth();
	int32_t tileHeight = cbmap->getTileHeight();

	// Same goes for object width, height and position.
	float objX = mObject1->getX();
	float objY = mObject1->getY();
	float objWidth = mObject1->getSizeX();
	float objHeight = mObject1->getSizeY();

	// Calculate tile coordinates that are one up and one left from the object
	int32_t startTileX = (int32_t) (mObject1->getX() + cbmap->getSizeX() / 2) / tileWidth - 1;
	int32_t startTileY = (int32_t) (-mObject1->getY() + cbmap->getSizeY() / 2) / tileHeight - 1;

	// Loop through an 3x3 area and check collisions to tiles that have hit data
	for (int32_t iterateX = 0; iterateX <= 2; ++iterateX) {
		for (int32_t iterateY = 0; iterateY <= 2; ++iterateY) {
			if (cbmap->getHit(startTileX + iterateX, startTileY + iterateY)) {
				float x = (startTileX + iterateX) * tileWidth - cbmap->getSizeX() / 2;
				float y = cbmap->getSizeY() / 2 - (startTileY + iterateY) * tileHeight;

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
					rendertarget->useWorldCoords(true);
					rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));
					rendertarget->useWorldCoords(false);
					//DEBUG("Collision detected at (%f, %f)", x, y);
				}
			}
		}
	}

	return false;
}

/** A circle - map collision test */
bool CollisionCheck::CircleMapTest() {
	DrawCollisionBoundaries();

	return false;
}

/** Drawing the collision box, used for debugging only. */
void CollisionCheck::DrawCollisionBoundaries() {

	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	rendertarget->useWorldCoords(true);

	// Draw the collision box for the first collision
	switch (mCollisionType1) {
		case Box: {
			float w = mObject1->getSizeX();
			float h = mObject1->getSizeY();
			float x = mObject1->getX() - w / 2;
			float y = mObject1->getY() + h / 2;
			rendertarget->drawBox(x, y, w, h, false, al_map_rgba(0, 255, 0, 128));
			break;
		}
		case Circle: {
			float x = mObject1->getX();
			float y = mObject1->getY();
			float r = mObject1->getSizeX() / 2;
			rendertarget->drawCircle(x, y, r, false, al_map_rgba(0, 255, 0, 128));
			break;
		}
	}

	// Draw the collision box for the thing to collide to
	switch (mCollisionType2) {
		case Box: break;
		case Circle: break;
		case Map: {
			CBMap *cbmap = static_cast<CBMap*>(mObject2);
			// Draw a box around the 8 tiles surrounding player
			// and the tile the player is currently on.
			int32_t startTileX = (int32_t) (mObject1->getX() + cbmap->getSizeX() / 2) / cbmap->getTileWidth() - 1;
			int32_t startTileY = (int32_t) (-mObject1->getY() + cbmap->getSizeY() / 2) / cbmap->getTileHeight() - 1;
			for (int32_t x = 0; x <= 2; ++x) {
				for (int32_t y = 0; y <= 2; ++y) {
					if (cbmap->getHit(startTileX + x, startTileY + y)) {
						float drawX = (startTileX + x) * cbmap->getTileWidth() - cbmap->getSizeX() / 2;
						float drawY = cbmap->getSizeY() / 2 - (startTileY + y) * cbmap->getTileHeight();
						rendertarget->drawBox(drawX, drawY, cbmap->getTileWidth(), cbmap->getTileHeight(), true, al_map_rgba(64, 64, 64, 64));
					}
				}
			}
			break;
		}
	}

	rendertarget->useWorldCoords(false);
}


/** Tests a rectangle - rectangle collision, with parameters. */
bool CollisionCheck::RectRectTest(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	float left1, right1, top1, bottom1;
	float left2, right2, top2, bottom2;

	left1 = x1;
	right1 = x1 + w1;
	top1 = y1;
	bottom1 = y1 + h1;

	left2 = x2;
	right2 = x2 + w2;
	top2 = y2;
	bottom2 = y2 + h2;

	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	rendertarget->useWorldCoords(true);

	rendertarget->drawBox(x1, y1, w1, h1, false, al_map_rgba(0, 0, 255, 128));
	rendertarget->drawBox(x2, y2, w2, h2, false, al_map_rgba(0, 0, 255, 128));

	rendertarget->useWorldCoords(false);

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;

	if (right1 < left2) return false;
	if (left1 > right2) return false;


	return true;
}

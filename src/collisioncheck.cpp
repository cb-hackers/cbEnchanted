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

/** Sets the objects that take part in this collision and resets safe coordinates. */
void CollisionCheck::setObjects(CBObject *a, CBObject *b) {
	mObject1 = a;
	mObject2 = b;

	safeX = mObject1->getX();
	safeY = mObject1->getY();
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
			INFO("Error: Unsupported collision type %i set for the colliding object", c);
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
			// The other collision type must also be Box
			if (mCollisionType1 == Box) {
				mCollisionType2 = Box;
			}
			else {
				INFO("Error: Unsupported colliding type %i: The other collision type must also be Box(1)", c);
				mObject1 = 0;
				mObject2 = 0;
			}
		break;
		case 2:
			// The other collision type must also be Circle
			if (mCollisionType1 == Circle) {
				mCollisionType2 = Circle;
			}
			else {
				INFO("Error: Unsupported colliding type %i: The other collision type must also be Circle(2)", c);
				mObject1 = 0;
				mObject2 = 0;
			}
		break;
		case 4:
			// The other collision type must be Box or Circle
			if (mCollisionType1 == Box || mCollisionType1 == Circle) {
				mCollisionType2 = Map;
			}
			else {
				INFO("Error: Unsupported colliding type %i: The other collision type must be Box(1) or Circle(2)", c);
				mObject1 = 0;
				mObject2 = 0;
			}
		break;
		default:
			INFO("Error: Unsupported collision type %i set for the object to collide to", c);
			mObject1 = 0;
			mObject2 = 0;
	}
}

/** Sets the way collision are handled.
 *
 * If the collision handling is of invalid type, mObject1 and mObject2 are cleared so
 * that the collision will never be checked.
 */
void CollisionCheck::setCollisionHandling(uint16_t h) {
	switch (h) {
		case 1:
			if (mCollisionType1 == Circle) {
				mCollisionHandling = Stop;
			}
			else {
				INFO("Error: Unsupported collision handling type %i: The collision type must be set to Circle(2)", h);
				mObject1 = 0;
				mObject2 = 0;
			}
		break;
		case 2:
			mCollisionHandling = Slide;
		break;
		default:
			INFO("Error: Unsupported collision handling type %i", h);
			mObject1 = 0;
			mObject2 = 0;
	}
}

/** Tests the collision */
void CollisionCheck::testCollision() {
	// Make sure we are not testing against null objects
	if (isNull()) {
		FIXME("Trying to test collision for null objects");
		return;
	}

	// If object is set to be skipped from collision checks then skip it.
	if (!mObject1->isCollisionsOn()) {
		return;
	}

	// What is the collision type for the colliding object
	switch (mCollisionType1) {
		case Box:
			switch (mCollisionType2) {
				case Box: RectRectTest(); break;
				case Circle: RectCircleTest(); break;
				case Map: RectMapTest(); break;
			}
		break;
		case Circle:
			switch (mCollisionType2) {
				case Box: CircleRectTest(); break;
				case Circle: CircleCircleTest(); break;
				case Map: CircleMapTest(); break;
			}
		break;
	}
}

/** Handles the collision */
void CollisionCheck::handleCollision() {

}

// ----------------------------------------------------------------------------
// Below are the private methods that calculate the collisions.
// ----------------------------------------------------------------------------

/** A circle - rectangle collision test */
void CollisionCheck::CircleRectTest() {
	DrawCollisionBoundaries();
}

/** A rectangle - circle collision test */
void CollisionCheck::RectCircleTest() {
	DrawCollisionBoundaries();
}

/** A rectangle - rectangle collision test */
void CollisionCheck::RectRectTest() {
	if (RectRectTest(mObject1->getX(), mObject1->getY(), mObject1->getRange1(), mObject1->getRange2(),
					 mObject2->getX(), mObject2->getY(), mObject2->getRange1(), mObject2->getRange2())) {
		// We have a collision! Calculate collision angle

		// Calculate collision angle
		float collisionAngle = atan2(mObject2->getY() - mObject1->getY(), mObject2->getX() - mObject1->getX());
		// collisionAngle is now in range -PI...PI, turn it to degrees!
		collisionAngle = ((collisionAngle + M_PI) / M_PI) * 180.0f;

		// Check the colliding direction from collisionAngle
		if (collisionAngle >= 315.0f || collisionAngle < 45.0f) {
			// Right?
			DEBUG("Box collision at right side");
			mObject1->positionObject(safeX, safeY);
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, mObject1->getX(), mObject1->getY()));
		}
		else if (collisionAngle < 135.0f) {
			// Top?
			DEBUG("Box collision at top side");
			mObject1->positionObject(safeX, safeY);
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, mObject1->getX(), mObject1->getY()));
		}
		else if (collisionAngle < 225.0f) {
			// Left?
			DEBUG("Box collision at left side");
			mObject1->positionObject(safeX, safeY);
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, mObject1->getX(), mObject1->getY()));
		}
		else {
			// Bottom?
			DEBUG("Box collision at bottom side");
			mObject1->positionObject(safeX, safeY);
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, mObject1->getX(), mObject1->getY()));
		}
	}
	safeX = mObject1->getX();
	safeY = mObject1->getY();
}

/** A circle - circle collision test */
void CollisionCheck::CircleCircleTest() {
	DrawCollisionBoundaries();
}

/** A rectangle - map collision test */
void CollisionCheck::RectMapTest() {
	DrawCollisionBoundaries();

	// Did we collide? Where?
	//  * 0 = top
	//  * 1 = right
	//  * 2 = bottom
	//  * 3 = left
	bool collided[4] = {false, false, false, false};

	// The current map is the object in mObject2
	CBMap *cbmap = static_cast<CBMap*>(mObject2);

	// Fetch tile width and tile height to variables so that the code is less messy
	int32_t tileWidth = cbmap->getTileWidth();
	int32_t tileHeight = cbmap->getTileHeight();

	// Same goes for object range and position.
	float objX = mObject1->getX();
	float objY = mObject1->getY();
	float objWidth = mObject1->getRange1();
	float objHeight = mObject1->getRange2();

	// Calculate the amount of tiles to both X- and Y-directions we need to check
	uint16_t checkTilesX = 1;
	uint16_t checkTilesY = 1;

	// Calculate tile coordinates that are one up and one left from the object
	int32_t startTileX = (int32_t) (mObject1->getX() + cbmap->getSizeX() / 2) / tileWidth - checkTilesX;
	int32_t startTileY = (int32_t) (-mObject1->getY() + cbmap->getSizeY() / 2) / tileHeight - checkTilesY;

	// First check x-directional collision with obj position as safeY
	for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
		for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2;
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight;

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, safeY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
					objX = safeX;
					// Check the direction to where we collided
					if (tileX < startTileX + checkTilesX) {
						// It seems to be left.
						collided[3] = true;
						objX = x + tileWidth + 1.0f + objWidth/2;
					}
					else if (tileX > startTileX + checkTilesX) {
						// It seems to be right.
						collided[1] = true;
						objX = x - 1.0f - objWidth/2;
					}
				}
			}
		}
	}

	// Then check y-directional collision, now with regular coordinates
	for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
		for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2;
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight;

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
					objY = safeY;
					// Check the direction to where we collided
					if (tileY < startTileY + checkTilesY) {
						// It seems to be top.
						collided[0] = true;
						objY = y - tileHeight - 1.0f - objHeight/2;
					}
					else if (tileY > startTileY + checkTilesY) {
						// It seems to be bottom.
						collided[2] = true;
						objY = y + 1.0f + objHeight/2;
					}
				}
			}
		}
	}

	safeX = objX;
	safeY = objY;

	mObject1->positionObject(safeX, safeY);

	// Let's add those collisions.
	if (collided[0]) {
		// Top
		mObject1->addCollision(new Collision(mObject1, mObject2, 270.0, objX, objY + objHeight/2 + 1.0f));
	}
	if (collided[1]) {
		// Right
		mObject1->addCollision(new Collision(mObject1, mObject2, 180.0, objX + objWidth/2 + 1.0f, objY));
	}
	if (collided[2]) {
		// Bottom
		mObject1->addCollision(new Collision(mObject1, mObject2, 90.0, objX, objY - objHeight/2 - 1.0f));
	}
	if (collided[3]) {
		// Left
		mObject1->addCollision(new Collision(mObject1, mObject2, 0.0, objX - objWidth/2 - 1.0f, objY));
	}
}

/** A circle - map collision test */
void CollisionCheck::CircleMapTest() {
	DrawCollisionBoundaries();
}

/** Drawing the collision box, used for debugging only. */
void CollisionCheck::DrawCollisionBoundaries() {
	return;

	CBEnchanted *cb = CBEnchanted::instance();
	RenderTarget *rendertarget = cb->getCurrentRenderTarget();

	rendertarget->useWorldCoords(true);

	// Draw the collision box for the first collision
	switch (mCollisionType1) {
		case Box: {
			float w = mObject1->getRange1();
			float h = mObject1->getRange2();
			float x = mObject1->getX() - w / 2;
			float y = mObject1->getY() + h / 2;
			rendertarget->drawBox(x, y, w, h, false, al_map_rgba(0, 255, 0, 128));
			break;
		}
		case Circle: {
			float x = mObject1->getX();
			float y = mObject1->getY();
			float r = mObject1->getRange1() / 2;
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
	top1 = y1 - h1;
	bottom1 = y1;

	left2 = x2;
	right2 = x2 + w2;
	top2 = y2 - h2;
	bottom2 = y2;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;

	if (right1 < left2) return false;
	if (left1 > right2) return false;

	return true;
}


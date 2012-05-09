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
	// Get the object coordinates here and modify them freely, later position the object according to them
	float objX = mObject1->getX();
	float objY = mObject1->getY();

	// Get object dimensions here so that the code is cleaner
	float objW = mObject1->getRange1();
	float objH = mObject1->getRange2();

	// First check collision in x-direction
	if (RectRectTest(objX, safeY, objW, objH,
			mObject2->getX(), mObject2->getY(), mObject2->getRange1(), mObject2->getRange2())) {
		// We have a collision! Calculate collision angle

		// Calculate collision angle
		float collisionAngle = atan2(mObject2->getY() - safeY, mObject2->getX() - objX);
		// collisionAngle is now in range -PI...PI, turn it to degrees!
		collisionAngle = ((collisionAngle + M_PI) / M_PI) * 180.0f;


		// Check the colliding direction
		if (objX > mObject2->getX()) {
			// Left?
			//DEBUG("Box collision at left side of colliding object");
			objX = mObject2->getX() + mObject2->getRange1() + 1.0f;
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, objX - mObject1->getRange1()/2 - 1.0f, objY));
		}
		else {
			// Right?
			//DEBUG("Box collision at right side of colliding object");
			objX = mObject2->getX() - mObject2->getRange1() - 1.0f;
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, objX + mObject1->getRange1()/2 + 1.0f, objY));
		}
	}

	// Then check collision in y-direction
	if (RectRectTest(objX, objY, objW, objH,
			mObject2->getX(), mObject2->getY(), mObject2->getRange1(), mObject2->getRange2())) {
		// We have a collision! Calculate collision angle

		// Calculate collision angle
		float collisionAngle = atan2(mObject2->getY() - objY, mObject2->getX() - objX);
		// collisionAngle is now in range -PI...PI, turn it to degrees!
		collisionAngle = ((collisionAngle + M_PI) / M_PI) * 180.0f;

		// Check the colliding direction
		if (objY > mObject2->getY()) {
			// Bottom?
			//DEBUG("Box collision at bottom side of colliding object");
			objY = mObject2->getY() + mObject2->getRange2() + 1.0f;
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, objX, objY - mObject1->getRange2()/2 - 1.0f));
		}
		else {
			// Top?
			//DEBUG("Box collision at top side of colliding object");
			objY = mObject2->getY() - mObject2->getRange2() - 1.0f;
			mObject1->addCollision(new Collision(mObject1, mObject2, collisionAngle, objX, objY + mObject1->getRange2()/2 + 1.0f));
		}
	}

	safeX = objX;
	safeY = objY;

	mObject1->positionObject(safeX, safeY);
}

/** A circle - circle collision test */
void CollisionCheck::CircleCircleTest() {
	// Get the object coordinates here and then modify them later on. Creates more readable code ;)
	float objX = mObject1->getX();
	float objY = mObject1->getY();

	// Also calculate radii of collision objects.
	float obj1r = mObject1->getRange1() / 2;
	float obj2r = mObject2->getRange1() / 2;

	// Calculate the differentials between object coordinates
	float dx = mObject2->getX() - objX;
	float dy = mObject2->getY() - objY;

	float dist = dx * dx + dy * dy;
	float minDist = obj1r + obj2r;

	// Check for collision and then handle properly
	if (dist < minDist * minDist) {
		float radAngle;
		if (mCollisionHandling == Stop) {
			// Check the angle from safe coordinates, so that we stop instead of sliding
			radAngle = atan2(mObject2->getY() - safeY, mObject2->getX() - safeX);
		}
		else {
			// Check the angle from new object coordinates so we slide
			radAngle = atan2(dy, dx);
		}

		// Position the object using triginometry. Thank you, mathematics.
		objX = mObject2->getX() - cos(radAngle) * (obj1r + obj2r + 0.5f);
		objY = mObject2->getY() - sin(radAngle) * (obj1r + obj2r + 0.5f);

		// Add the collision
		mObject1->addCollision(new Collision(
			mObject1,
			mObject2,
			((radAngle + M_PI) / M_PI) * 180.0f,
			objX + cos(radAngle) * (obj1r + 1.0f),
			objY + sin(radAngle) * (obj1r + 1.0f)
		));
	}

	// And then the resetion.
	safeX = objX;
	safeY = objY;
	mObject1->setPosition(safeX, safeY);
}

/** A rectangle - map collision test */
void CollisionCheck::RectMapTest() {
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
	int32_t startTileX = (int32_t) (objX - cbmap->getX() + cbmap->getSizeX() / 2) / tileWidth - checkTilesX;
	int32_t startTileY = (int32_t) (-objY + cbmap->getY() + cbmap->getSizeY() / 2) / tileHeight - checkTilesY;

	// The epsilon amount we use for floating point error corrections
	const float eps = 1e-5f;

	// First check x-directional collision with obj position as safeY
	for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
		for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2 + cbmap->getX();
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight + cbmap->getY();

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, safeY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
					objX = safeX;
					// Check the direction to where we collided
					if (tileX < startTileX + checkTilesX) {
						// It seems to be left.
						collided[3] = true;
						objX = x + tileWidth + eps + objWidth/2;
					}
					else if (tileX > startTileX + checkTilesX) {
						// It seems to be right.
						collided[1] = true;
						objX = x - eps - objWidth/2;
					}
				}
			}
		}
	}

	// Then check y-directional collision, now with regular coordinates
	for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
		for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2 + cbmap->getX();
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight + cbmap->getY();

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
					objY = safeY;
					// Check the direction to where we collided
					if (tileY < startTileY + checkTilesY) {
						// It seems to be top.
						collided[0] = true;
						objY = y - tileHeight - eps - objHeight/2;
					}
					else if (tileY > startTileY + checkTilesY) {
						// It seems to be bottom.
						collided[2] = true;
						objY = y + eps + objHeight/2;
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

	// Same goes for object radius and position.
	float objX = mObject1->getX();
	float objY = mObject1->getY();
	float objR = mObject1->getRange1() / 2;

	// Calculate the amount of tiles to both X- and Y-directions we need to check
	uint16_t checkTilesX = 1;
	uint16_t checkTilesY = 1;

	// Calculate tile coordinates that are one up and one left from the object
	int32_t startTileX = (int32_t) (objX - cbmap->getX() + cbmap->getSizeX() / 2) / tileWidth - checkTilesX;
	int32_t startTileY = (int32_t) (-objY + cbmap->getY() + cbmap->getSizeY() / 2) / tileHeight - checkTilesY;

	// Check collision to nearby tiles
	for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
		for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2 + cbmap->getX();
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight + cbmap->getY();

				float centerY = y - tileHeight/2;

				// We got ourselves some real coordinates. Now do circle-rectangle collision.
				if (this->CircleRectTest(objX, safeY + tileHeight, objR, x, y, tileWidth, tileHeight)) {
					// Should we handle this like a rectangle-map collision?
					if (centerY > safeY) {
						float cornerY = centerY - tileHeight/2;
						if (cbmap->getHit(tileX, tileY + 1) || cornerY < safeY) {
							// Rect rect
							// Check the direction to where we collided
							if (tileX < startTileX + checkTilesX) {
								// It seems to be left.
								collided[3] = true;
								objX = x + tileWidth + 0.5f + objR;
							}
							else if (tileX > startTileX + checkTilesX) {
								// It seems to be right.
								collided[1] = true;
								objX = x - 0.5f - objR;
							}
						}
						else {
							float cornerX;
							bool isCornerSet = false;
							// Check the direction to where we collided
							if (tileX < startTileX + checkTilesX) {
								// It seems to be left.
								cornerX = x + tileWidth;
								isCornerSet = true;
								collided[3] = true;
							}
							else if (tileX > startTileX + checkTilesX) {
								// It seems to be right.
								cornerX = x;
								isCornerSet = true;
								collided[1] = true;
							}

							if (isCornerSet) {
								float radAngle = atan2(cornerY - safeY, cornerX - objX);
								objX = cornerX - cos(radAngle) * (objR + 0.5f);
							}
						}
					}
					else {
						float cornerY = centerY + tileHeight/2;
						if (cbmap->getHit(tileX, tileY - 1) || cornerY > safeY) {
							// Rect rect
							// Check the direction to where we collided
							if (tileX < startTileX + checkTilesX) {
								// It seems to be left.
								objX = x + tileWidth + 0.5f + objR;
								collided[3] = true;
							}
							else if (tileX > startTileX + checkTilesX) {
								// It seems to be right.
								objX = x - 0.5f - objR;
								collided[1] = true;
							}
						}
						else {
							float cornerX;
							bool isCornerSet = false;
							// Check the direction to where we collided
							if (tileX < startTileX + checkTilesX) {
								// It seems to be left.
								cornerX = x + tileWidth;
								isCornerSet = true;
								collided[3] = true;
							}
							else if (tileX > startTileX + checkTilesX) {
								// It seems to be right.
								cornerX = x;
								isCornerSet = true;
								collided[1] = true;
							}

							if (isCornerSet) {
								float radAngle = atan2(cornerY - safeY, cornerX - objX);
								objX = cornerX - cos(radAngle) * (objR + 0.5f);
							}
						}
					}
					goto breakout1;
				}
			}
		}
	}
	breakout1:

	// Check collision to nearby tiles
	for (int32_t tileY = startTileY; tileY <= startTileY + checkTilesY*2; tileY++) {
		for (int32_t tileX = startTileX; tileX <= startTileX + checkTilesX*2; tileX++) {
			if (cbmap->getHit(tileX, tileY)) {
				float x = tileX * tileWidth - cbmap->getSizeX() / 2 + cbmap->getX();
				float y = cbmap->getSizeY() / 2 - tileY * tileHeight + cbmap->getY();

				float centerX = x + tileWidth/2;

				// We got ourselves some real coordinates. Now do circle-rectangle collision.
				if (this->CircleRectTest(objX, objY + tileHeight, objR, x, y, tileWidth, tileHeight)) {
					// Should we handle this like a rectangle-map collision?
					if (centerX > objX) {
						float cornerX = x;
						if (cbmap->getHit(tileX - 1, tileY) || cornerX < objX) {
							// Rect rect
							// Check the direction to where we collided
							if (tileY < startTileY + checkTilesY) {
								// It seems to be top.
								objY = y - tileHeight - 0.5f - objR;
								collided[0] = true;
							}
							else if (tileY > startTileY + checkTilesY) {
								// It seems to be bottom.
								objY = y + 0.5f + objR;
								collided[2] = true;
							}
						}
						else {
							float cornerY;
							bool isCornerSet = false;
							// Check the direction to where we collided
							if (tileY < startTileY + checkTilesY) {
								// It seems to be top.
								cornerY = y - tileHeight;
								isCornerSet = true;
								collided[0] = true;
							}
							else if (tileY > startTileY + checkTilesY) {
								// It seems to be bottom.
								cornerY = y;
								isCornerSet = true;
								collided[2] = true;
							}

							if (isCornerSet) {
								float radAngle = atan2(cornerY - objY, cornerX - objX);
								objY = cornerY - sin(radAngle) * (objR + 0.5f);
							}
						}
					}
					else {
						float cornerX = x + tileWidth;
						if (cbmap->getHit(tileX + 1, tileY) || cornerX > objX) {
							// Rect rect
							// Check the direction to where we collided
							if (tileY < startTileY + checkTilesY) {
								// It seems to be top.
								objY = y - tileHeight - 0.5f - objR;
								collided[0] = true;
							}
							else if (tileY > startTileY + checkTilesY) {
								// It seems to be bottom.
								objY = y + 0.5f + objR;
								collided[2] = true;
							}
						}
						else {
							float cornerY;
							bool isCornerSet = false;
							// Check the direction to where we collided
							if (tileY < startTileY + checkTilesY) {
								// It seems to be top.
								cornerY = y - tileHeight;
								isCornerSet = true;
								collided[0] = true;
							}
							else if (tileY > startTileY + checkTilesY) {
								// It seems to be bottom.
								cornerY = y;
								isCornerSet = true;
								collided[2] = true;
							}

							if (isCornerSet) {
								float radAngle = atan2(cornerY - objY, cornerX - objX);
								objY = cornerY - sin(radAngle) * (objR + 0.5f);
							}
						}
					}
					goto breakout2;
				}
			}
		}
	}
	breakout2:

	safeX = objX;
	safeY = objY;
	mObject1->setPosition(objX, objY);

	// Let's add those collisions.
	if (collided[0]) {
		// Top
		mObject1->addCollision(new Collision(mObject1, mObject2, 270.0, 0.0, 0.0));
	}
	if (collided[1]) {
		// Right
		mObject1->addCollision(new Collision(mObject1, mObject2, 180.0, 0.0, 0.0));
	}
	if (collided[2]) {
		// Bottom
		mObject1->addCollision(new Collision(mObject1, mObject2, 90.0, 0.0, 0.0));
	}
	if (collided[3]) {
		// Left
		mObject1->addCollision(new Collision(mObject1, mObject2, 0.0, 0.0, 0.0));
	}
}

/** Drawing the collision box, used for debugging only. */
void CollisionCheck::DrawCollisionBoundaries() {
	//return;

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
		case Box: {
			float w = mObject2->getRange1();
			float h = mObject2->getRange2();
			float x = mObject2->getX() - w / 2;
			float y = mObject2->getY() + h / 2;
			rendertarget->drawBox(x, y, w, h, false, al_map_rgba(0, 255, 0, 128));
			break;
		}
		case Circle: {
			float x = mObject2->getX();
			float y = mObject2->getY();
			float r = mObject2->getRange1() / 2;
			rendertarget->drawCircle(x, y, r, false, al_map_rgba(0, 255, 0, 128));
			break;
		}
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

	// The epsilon amount we use for floating point error corrections
	const float eps = 1e-5f;

	left1 = x1;
	right1 = x1 + w1;
	top1 = y1 - h1;
	bottom1 = y1;

	left2 = x2;
	right2 = x2 + w2;
	top2 = y2 - h2;
	bottom2 = y2;

	if (bottom1 < top2 + eps) return false;
	if (top1 > bottom2 - eps) return false;

	if (right1 < left2 + eps) return false;
	if (left1 > right2 - eps) return false;

	return true;
}

/** Tests a circle - rectangle collision, with parameters.
 *
 * @see http://stackoverflow.com/a/402010
 */
bool CollisionCheck::CircleRectTest(float circleX, float circleY, float circleR, float rectX, float rectY, float rectW, float rectH) {
	// Rectangle width and height are needed in every calculation, so calculate them here already
	float halfRectW = rectW / 2;
	float halfRectH = rectH / 2;

	float circleDistX = abs(circleX - rectX - halfRectW);
	float circleDistY = abs(circleY - rectY - halfRectH);

	// If we're too far away, we can't collide
	if (circleDistX > (halfRectW + circleR)) { return false; }
	if (circleDistY > (halfRectH + circleR)) { return false; }

	// If we're close enough, we always collide
	if (circleDistX <= halfRectW) { return true; }
	if (circleDistY <= halfRectH) { return true; }

	// Then the computationally more expensive part, the corner.
	float cornerDistance_sq = (circleDistX - halfRectW)*(circleDistX - halfRectW) +
			(circleDistY - halfRectH)*(circleDistY - halfRectH);

	return (cornerDistance_sq <= (circleR * circleR));
}


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

	// Calculate the amount of tiles to both X- and Y-directions we need to check
	uint16_t checkTilesX = 1;
	uint16_t checkTilesY = 1;

	// Calculate tile coordinates that are one up and one left from the object
	int32_t startTileX = (int32_t) (mObject1->getX() + cbmap->getSizeX() / 2) / tileWidth - checkTilesX;
	int32_t startTileY = (int32_t) (-mObject1->getY() + cbmap->getSizeY() / 2) / tileHeight - checkTilesY;

	// Create an array that tells if there was a collision and in which direction
	//   isCollisions[0] = top
	//   isCollisions[1] = right
	//   isCollisions[2] = bottom
	//   isCollisions[3] = left
	bool isCollisions[4] = {false, false, false, false};

	// Create an array that holds the related collision coordinate.
	// Top/bottom collision has y-coordinate while left/right collision has x-coordinate.
	float collisions[4];

	// Loop through tiles top of player
	for (int32_t iterateX = 0; iterateX <= checkTilesX * 2; ++iterateX) {
		if (cbmap->getHit(startTileX + iterateX, startTileY)) {
			float x = (startTileX + iterateX) * tileWidth - cbmap->getSizeX() / 2;
			float y = cbmap->getSizeY() / 2 - startTileY * tileHeight;

			// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
			if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
				// Colliding tile is top of player
				isCollisions[0] = true;
				collisions[0] = y - tileHeight - 1.0f;

				// Let's do some debug drawing
				rendertarget->useWorldCoords(true);
				rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));
				rendertarget->useWorldCoords(false);

				break;
			}
		}
	}

	// Check the directions to where we collided and handle them here.
	if (isCollisions[0]) {
		// Collision top
		rendertarget->drawText(cb->getCurrentFont(), "Collision top", 10, 30, al_map_rgb(255, 255, 255), 0);

		rendertarget->useWorldCoords(true);
		rendertarget->drawCircle(objX, collisions[0], 5, true, al_map_rgb(255, 0, 0));
		rendertarget->useWorldCoords(false);

		// Calculate the new y-coordinate for this object
		objY = collisions[0] - objHeight/2;
	}


	// ------------------------------------------------------------------------


	// Loop through tiles bottom of player
	for (int32_t iterateX = 0; iterateX <= checkTilesX * 2; ++iterateX) {
		if (cbmap->getHit(startTileX + iterateX, startTileY + 2)) {
			float x = (startTileX + iterateX) * tileWidth - cbmap->getSizeX() / 2;
			float y = cbmap->getSizeY() / 2 - (startTileY + 2) * tileHeight;

			// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
			if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
				// Colliding tile is to the down of player
				isCollisions[2] = true;
				collisions[2] = y + 1.0f;

				// Let's do some debug drawing
				rendertarget->useWorldCoords(true);
				rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));
				rendertarget->useWorldCoords(false);

				break;
			}
		}
	}

	// Check the directions to where we collided and handle them here.
	if (isCollisions[2]) {
		// Collision bottom
		rendertarget->drawText(cb->getCurrentFont(), "Collision bottom", 10, 54, al_map_rgb(255, 255, 255), 0);

		rendertarget->useWorldCoords(true);
		rendertarget->drawCircle(objX, collisions[2], 5, true, al_map_rgb(255, 0, 0));
		rendertarget->useWorldCoords(false);

		// Calculate the new y-coordinate for this object
		objY = collisions[2] + objHeight/2;
	}


	// ------------------------------------------------------------------------


	// Loop through tiles to the right of player
	for (int32_t iterateY = 0; iterateY <= checkTilesY * 2; ++iterateY) {
		if (cbmap->getHit(startTileX + 2, startTileY + iterateY)) {
			float x = (startTileX + 2) * tileWidth - cbmap->getSizeX() / 2;
			float y = cbmap->getSizeY() / 2 - (startTileY + iterateY) * tileHeight;

			// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
			if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
				// Colliding tile is to the right of player
				isCollisions[1] = true;
				collisions[1] = x - 1.0f;

				// Let's do some debug drawing
				rendertarget->useWorldCoords(true);
				rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));
				rendertarget->useWorldCoords(false);

				break;
			}
		}
	}

	// Check the directions to where we collided and handle them here.
	if (isCollisions[1]) {
		// Collision right
		rendertarget->drawText(cb->getCurrentFont(), "Collision right", 10, 42, al_map_rgb(255, 255, 255), 0);

		rendertarget->useWorldCoords(true);
		rendertarget->drawCircle(collisions[1], objY, 5, true, al_map_rgb(255, 0, 0));
		rendertarget->useWorldCoords(false);

		// Calculate the new x-coordinate for this object
		objX = collisions[1] - objWidth/2;
	}


	// ------------------------------------------------------------------------


	// Loop through tiles to the left of player
	for (int32_t iterateY = 0; iterateY <= checkTilesY * 2; ++iterateY) {
		if (cbmap->getHit(startTileX, startTileY + iterateY)) {
			float x = startTileX * tileWidth - cbmap->getSizeX() / 2;
			float y = cbmap->getSizeY() / 2 - (startTileY + iterateY) * tileHeight;

			// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
			if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {
				// Colliding tile is to the left of player
				isCollisions[3] = true;
				collisions[3] = x + tileWidth + 1.0f;

				// Let's do some debug drawing
				rendertarget->useWorldCoords(true);
				rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));
				rendertarget->useWorldCoords(false);

				break;
			}
		}
	}

	// Check the directions to where we collided and handle them here.
	if (isCollisions[3]) {
		// Collision left
		rendertarget->drawText(cb->getCurrentFont(), "Collision left", 10, 66, al_map_rgb(255, 255, 255), 0);

		rendertarget->useWorldCoords(true);
		rendertarget->drawCircle(collisions[3], objY, 5, true, al_map_rgb(255, 0, 0));
		rendertarget->useWorldCoords(false);

		// Calculate the new x-coordinate for this object
		objX = collisions[3] + objWidth/2;
	}



	/*
	for (int32_t iterateX = 0; iterateX <= checkTilesX * 2; ++iterateX) {
		for (int32_t iterateY = 0; iterateY <= checkTilesY * 2; ++iterateY) {
			if (cbmap->getHit(startTileX + iterateX, startTileY + iterateY)) {
				float x = (startTileX + iterateX) * tileWidth - cbmap->getSizeX() / 2;
				float y = cbmap->getSizeY() / 2 - (startTileY + iterateY) * tileHeight;

				// We got ourselves some real coordinates. Now we can just do regular rect-rect test to see if we collide.
				if (this->RectRectTest(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, x, y, tileWidth, tileHeight)) {

					if (iterateX < checkTilesX) {
						// Colliding tile is to the left of player
						isCollisions[3] = true;
						collisions[3] = x + tileWidth + 1.0f;
					}
					else if (iterateX > checkTilesY) {
						// Colliding tile is to the right of player
						isCollisions[1] = true;
						collisions[1] = x - 1.0f;
					}

					if (iterateY < checkTilesY) {
						// Colliding tile is to the up of player
						isCollisions[0] = true;
						collisions[0] = y - tileHeight - 1.0f;
					}
					else if (iterateY > checkTilesY) {
						// Colliding tile is to the down of player
						isCollisions[2] = true;
						collisions[2] = y + 1.0f;
					}

					// Let's do some debug drawing
					rendertarget->useWorldCoords(true);

					rendertarget->drawBox(objX - objWidth/2, objY + objHeight/2, objWidth, objHeight, true, al_map_rgba(255, 0, 0, 64));

					rendertarget->useWorldCoords(false);


					//objX = mCollisionX + cos(((mCollisionAngle - 180.0) / 180.0) * M_PI) * objWidth;
					//objY = mCollisionY - sin(((mCollisionAngle - 180.0) / 180.0) * M_PI) * objHeight / 2;

					//mObject1->setPosition(objX, objY);
					//DEBUG("Collision detected at (%f, %f)", x, y);

					//break;
				}
			}
		}
	}
	*/


	// Set the object position
	mObject1->setPosition(objX, objY);

	/*
		// Draw text indicating which way did we collide
		if (mCollidedX) {

		}
		else {
			mCollisionX = objX;
		}
		if (mCollidedY) {
			rendertarget->drawText(cb->getCurrentFont(), "Y-directional collision", 10, 42, al_map_rgb(255, 255, 255), 0);
		}
		else {
			mCollisionY = objY;
		}

		rendertarget->useWorldCoords(true);
		rendertarget->drawCircle(mCollisionX, mCollisionY, 5, true, al_map_rgb(255, 0, 0));
		rendertarget->useWorldCoords(false);
	}
	*/



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

	/*
	// Calculate collision angle
	if (y1 < y2) {
		// Collision from above
		mCollisionAngle = 270.0;
		mCollisionX = x1 + w1 / 2;
		mCollisionY = top1;
		rendertarget->drawText(cb->getCurrentFont(), "Collision above", 10, 30, al_map_rgb(255, 255, 255), 0);
	}
	else if (y1 > y2) {
		// Collision from below
		mCollisionAngle = 90.0;
		mCollisionX = x1 + w1 / 2;
		mCollisionY = bottom1;
		rendertarget->drawText(cb->getCurrentFont(), "Collision below", 10, 30, al_map_rgb(255, 255, 255), 0);
	}
	else if (x1 < x2) {
		// Collision from right side
		mCollisionAngle = 180.0;
		mCollisionX = right1;
		mCollisionY = y1 + h1 / 2;
		rendertarget->drawText(cb->getCurrentFont(), "Collision right", 10, 30, al_map_rgb(255, 255, 255), 0);
	}
	else if (x1 > x2) {
		// Collision from left side
		mCollisionAngle = 180.0;
		mCollisionX = left1;
		mCollisionY = y1 + h1 / 2;
		rendertarget->drawText(cb->getCurrentFont(), "Collision left", 10, 30, al_map_rgb(255, 255, 255), 0);
	}


	rendertarget->drawText(cb->getCurrentFont(), string("x1: ") + lexical_cast<string>(x1), 10, 42, al_map_rgb(255, 255, 255), 0);
	rendertarget->drawText(cb->getCurrentFont(), string("x2: ") + lexical_cast<string>(x2) , 10, 54, al_map_rgb(255, 255, 255), 0);
	rendertarget->drawText(cb->getCurrentFont(), string("y1: ") + lexical_cast<string>(y1), 10, 66, al_map_rgb(255, 255, 255), 0);
	rendertarget->drawText(cb->getCurrentFont(), string("y2: ") + lexical_cast<string>(y2), 10, 78, al_map_rgb(255, 255, 255), 0);
	*/
	return true;
}

/** Adds a new collision to the objects collision list. */
void CollisionCheck::addCollision() {
	Collision *collision = new Collision(mObject1, mObject2, mCollisionAngle, mCollisionX, mCollisionY);

	mObject1->addCollision(collision);
}

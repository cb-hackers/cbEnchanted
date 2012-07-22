#include "objectinterface.h"
#include "precomp.h"
#include "debug.h"
#include "cbimage.h"
#include "cbenchanted.h"
#include "mapinterface.h"
#include "gfxinterface.h"
#include "util.h"
#include "collisioncheck.h"
#include "errorsystem.h"

#ifndef CBE_LIB
ObjectInterface::ObjectInterface():
	lastUpdate(0),
	firstObject(0),
	firstFloorObject(0),
	lastObject(0),
	lastFloorObject(0),
	lastPickedObj(0),
	lastPickedX(0.0),
	lastPickedY(0.0),
	lastPickedAngle(0.0)
{
	cb = static_cast<CBEnchanted *>(this);
}

ObjectInterface::~ObjectInterface() {

}

void ObjectInterface::commandDeleteObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	if (object->type() == CBObject::ParticleEmitter) {
		objectMap.erase(id);
		cb->deleteParticleEmitter(static_cast<CBParticleEmitter*>(object));
		return;
	}
	// Remove possible collision checks
	removeFromCollisionCheck(object);

	if (object->type() == CBObject::Object) {
		removeFromDrawOrder(object);
		delete object;
	}
	else if (object->type() == CBObject::Map) {
		removeFromDrawOrder(object);
		cb->deleteTileMap();
	}
	objectMap.erase(id);
}

void ObjectInterface::commandClearObjects(void) {
	for (std::map<int32_t,CBObject*>::iterator i = objectMap.begin();i != objectMap.end();i++) {
		if (!i->second->isMap()) {
			delete i->second;
		}
	}
	cb->deleteTileMap();
	objectMap.clear();
	lastObject = firstObject = 0;
	firstFloorObject = lastFloorObject = 0;
}

void ObjectInterface::commandMoveObject(void) {
	float z = cb->popValue().toFloat();
	float side = cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->moveObject(fwrd,side);
}

void ObjectInterface::commandTranslateObject(void) {
	float z = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->translateObject(x,y,z);
}

void ObjectInterface::commandPositionObject(void) {
	cb->popValue(); //Z?
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->positionObject(x,y);
}

void ObjectInterface::commandScreenPositionObject(void) {
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->screenCoordToWorld(x, y);
	object->positionObject(x, y);
}

void ObjectInterface::commandTurnObject(void) {
	//Random shit...
	cb->popValue();
	cb->popValue();

	float a = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->turnObject(a);
}

void ObjectInterface::commandRotateObject(void) {
	cb->popValue(); //Something?
	cb->popValue(); //Something?
	float a = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->rotateObject(a);
}

void ObjectInterface::commandPointObject(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = getObject(id2);
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = getObject(id1);

	object1->rotateObject((M_PI - atan2f(-object1->getY() + object2->getY(), object1->getX() - object2->getX())) / M_PI * 180.0);
}

void ObjectInterface::commandCloneObjectPosition(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = getObject(id2);
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = getObject(id1);
	object1->positionObject(object2->getX(),object2->getY());
}

void ObjectInterface::commandCloneObjectOrientation(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = getObject(id2);
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = getObject(id1);
	object1->rotateObject(object2->getAngle());
}

void ObjectInterface::commandObjectOrder(void) {
	int32_t select = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	if (object->isFloorObject()) {
		// Don't do nothing if there is only one floor object
		if (firstFloorObject == lastFloorObject) {
			return;
		}
		// Remove the object from drawing order completely
		//  - "CBObject *a" is the object that is drawn before "object"
		//  - "CBObject *b" is the object that is drawn after "object"
		CBObject *a = object->beforeObj;
		CBObject *b = object->afterObj;
		if (object == firstFloorObject) {
			firstFloorObject = b;
			b->beforeObj = 0;
		}
		else if (object == lastFloorObject) {
			lastFloorObject = a;
			a->afterObj = 0;
		}
		else {
			// Remove from middle
			a->afterObj = b;
			b->beforeObj = a;
		}
		// Forget the old links for "object"
		object->afterObj = 0;
		object->beforeObj = 0;

		if (select == -1) { //Move to last
			lastFloorObject->afterObj = object;
			object->beforeObj = lastFloorObject;
			lastFloorObject = object;
		}
		else if (select == 1) { //Move to top
			firstFloorObject->beforeObj = object;
			object->afterObj = firstFloorObject;
			firstFloorObject = object;
		}
	}
	else {
		// Don't do nothing if there is only one object
		if (firstObject == lastObject) {
			return;
		}
		// Remove the object from drawing order completely
		//  - "CBObject *a" is the object that is drawn before "object"
		//  - "CBObject *b" is the object that is drawn after "object"
		CBObject *a = object->beforeObj;
		CBObject *b = object->afterObj;
		if (object == firstObject) {
			firstObject = b;
			b->beforeObj = 0;
		}
		else if (object == lastObject) {
			lastObject = a;
			a->afterObj = 0;
		}
		else {
			// Remove from middle
			a->afterObj = b;
			b->beforeObj = a;
		}
		// Forget the old links for "object"
		object->afterObj = 0;
		object->beforeObj = 0;

		if (select == -1) { // Move to bottom
			firstObject->beforeObj = object;
			object->afterObj = firstObject;
			firstObject = object;
		}
		else if (select == 1) { //Move to top
			lastObject->afterObj = object;
			object->beforeObj = lastObject;
			lastObject = object;
		}
	}
}

void ObjectInterface::commandMaskObject(void) {
	uint8_t b = cb->popValue().toByte();
	uint8_t g = cb->popValue().toByte();
	uint8_t r = cb->popValue().toByte();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->maskObject(r,g,b);
}

void ObjectInterface::commandShowObject(void) {
	bool t = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->showObject(t);
}

void ObjectInterface::commandDefaultVisible(void) {
	bool t = cb->popValue().toInt();
	CBObject::setDefaultVisible(t);
}

void ObjectInterface::commandPaintObject(void) {
	int p = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);

	if (object->isMap() || object->isFloorObject()) {
		// Maps and floor objects can only be painted with an image
		CBImage *img = cb->getImage(p);
		object->paintObject(*img->getRenderTarget());
	}
	else {
		// Painting a regular object
		if (p > 0 && !object->isFloorObject()) { //Object
			CBObject *object2 = getObject(p);
			object->paintObject(*object2);
		}
		else { //Image
			CBImage *img = cb->getImage(-p);
			object->paintObject(*img->getRenderTarget());
		}
	}
}

void ObjectInterface::commandGhostObject(void) {
	float alpha = cb->popValue().toFloat() / 100.0f;
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);

	if (alpha < 0.0f) {
		alpha = 0.0f;
	}
	else if (alpha > 1.0f) {
		alpha = 1.0f;
	}

	object->ghostObject(alpha);
}

void ObjectInterface::commandMirrorObject(void) {
	int32_t dir = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	CBObject *obj = getObject(id);
	if (!obj) {
		return;
	}

	if (dir < 0 || dir > 2) {
		cb->errors->createError("MirrorObject failed!", "Valid values for MirrorObject are 0, 1 and 2.\nYour value was " + boost::lexical_cast<string>(dir) + ".");
	}
	else if (obj->type() != CBObject::Object) {
		cb->errors->createError("MirrorObject failed!", "Only regular objects can be mirrored.");
	}
	else {
		obj->mirrorObject(dir);
	}
}

void ObjectInterface::commandObjectRange(void) {
	float range2 = cb->popValue().toFloat();
	float range1 = cb->popValue().toFloat();
	if (range2 < 0.001f) {
		range2 = range1;
	}
	int32_t id = cb->popValue().getInt();
	CBObject *obj = getObject(id);

	obj->setRange(range1, range2);
}

void ObjectInterface::commandObjectInteger(void) {
	int32_t i = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	getObject(id)->setObjectInteger(i);
}

void ObjectInterface::commandObjectFloat(void) {
	float f = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	getObject(id)->setObjectFloat(f);
}

void ObjectInterface::commandObjectString(void) {
	const ISString &s = cb->popValue().toString();
	int32_t id = cb->popValue().getInt();
	getObject(id)->setObjectString(s);
}

void ObjectInterface::commandObjectPickable(void) {
	int32_t pickStyle = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	CBObject *obj = getObject(id);

	if (pickStyle == 0) {
		// Zero means delete.
		std::vector<CBObject*>::iterator i;
		for (i = pickableObjects.begin(); i != pickableObjects.end(); i++) {
			if ((*i)->getID() == id) {
				// Yeah, this one should be deleted.
				pickableObjects.erase(i);
				return;
			}
		}
	}

	if (obj->setPickStyle(pickStyle)) {
		// If pickStyle is valid, setPickStyle returns true
		pickableObjects.push_back(obj);
	}
}

void ObjectInterface::commandObjectPick(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *obj = getObject(id);

	// Init lastPickedObj to nothing.
	lastPickedObj = 0;

	// Loop through every pickable object in pickableObjects vector and do the raycast,
	// setting end coordinates to the following variables
	float endX, endY;
	// Save distance square to this, so we can perform fast distance checks without sqrt
	float distSqr = -1.0f;
	std::vector<CBObject*>::iterator i;
	for (i = pickableObjects.begin(); i != pickableObjects.end(); i++) {
		if ((*i)->getID() != id) {
			if ((*i)->rayCast(obj, endX, endY)) {
				// Picked object, find out if it's the nearest
				float tmpDistSqr = (obj->getX() - endX) * (obj->getX() - endX) + (obj->getY() - endY) * (obj->getY() - endY);
				if (distSqr < -0.5f || tmpDistSqr < distSqr) {
					distSqr = tmpDistSqr;
					lastPickedObj = (*i)->getID();
					lastPickedX = endX;
					lastPickedY = endY;
				}
			}
		}
	}

	lastPickedAngle = atan2(endY - obj->getY(), endX - obj->getX());
}

void ObjectInterface::commandPixelPick(void) {
	STUB;
}

void ObjectInterface::commandObjectLife(void) {
	int32_t life = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->setLife(life);
}

void ObjectInterface::commandPlayObject(void) {
	bool continuous = cb->popValue().toInt();
	float speed = cb->popValue().toFloat();
	int32_t endf = cb->popValue().toInt();
	int32_t startf = cb->popValue().toInt();
	int32_t id = cb->popValue().toInt();
	CBObject *object = getObject(id);

	if (endf == -1) {
		// Stop playing and reset current frame to 0
		object->stopPlaying(false);
	}
	else {
		object->startPlaying(startf, endf, speed, continuous);
		object->setLooping(false);
	}
}

void ObjectInterface::commandLoopObject(void) {
	bool continuous = cb->popValue().toInt();
	float speed = cb->popValue().toFloat();
	uint16_t endf = cb->popValue().toInt();
	uint16_t startf = cb->popValue().toInt();
	int32_t id = cb->popValue().toInt();
	CBObject *object = getObject(id);

	object->startPlaying(startf, endf, speed, continuous);
	object->setLooping(true);
}

void ObjectInterface::commandStopObject(void) {
	int32_t id = cb->popValue().toInt();
	CBObject *object = getObject(id);
	// Stop playing and keep current frame
	object->stopPlaying(true);
}

void ObjectInterface::commandResetObjectCollision(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	object->setCollisionChecking(false);
	object->eraseCollisions();
}

void ObjectInterface::commandSetupCollision(void) {
	// The way collisions are handled
	uint16_t handling = cb->popValue().toInt();
	// Type of collision
	uint16_t obj2colType = cb->popValue().toInt();
	uint16_t obj1colType = cb->popValue().toInt();
	// Object IDs
	int32_t obj2id = cb->popValue().toInt();
	int32_t obj1id = cb->popValue().toInt();

	// Fetch CBObject's based on object IDs
	CBObject *obj1 = getObject(obj1id);
	CBObject *obj2 = getObject(obj2id);

	// Create a new collision check
	CollisionCheck *colChk = new CollisionCheck(obj1, obj2);

	// Initialize the collision check
	colChk->setCollisionType1(obj1colType);
	colChk->setCollisionType2(obj2colType);
	colChk->setCollisionHandling(handling);

	// Check that collisions are set OK
	if (!colChk->isNull()) {
		// Add the collision check to collisions vector
		collisionChecks.push_back(colChk);
	}
}

void ObjectInterface::commandClearCollisions(void) {
	collisionChecks.clear();
}

void ObjectInterface::commandInitObjectList(void) {
	iter = objectMap.begin();
}

void ObjectInterface::functionLoadObject(void) {
	cb->popValue(); //Rotation...
	ALLEGRO_PATH *path = cb->popValue().getString().getPath();
	const char *cpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	CBObject *obj = new CBObject;
	if (!obj->load(cpath)) {
		cb->errors->createError("LoadObject() failed!", "Failed to load file \"" + string(cpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(path);
		return;
	}
	al_destroy_path(path);
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	obj->setID(id);
	cb->pushValue(id);
}

void ObjectInterface::functionLoadAnimObject(void) {
	cb->popValue().toInt();
	uint16_t frames = cb->popValue().getInt();
	uint16_t startf = cb->popValue().getInt();
	uint16_t frameH = cb->popValue().getInt();
	uint16_t frameW = cb->popValue().getInt();
	ALLEGRO_PATH *path = cb->popValue().getString().getPath();
	const char *cpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	CBObject *obj = new CBObject;
	if(!obj->loadAnimObject(cpath, frameW, frameH, startf, frames)){
		cb->errors->createError("LoadAnimObject() failed!", "Failed to load file \"" + string(cpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(path);
		return;
	}
	al_destroy_path(path);
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	obj->setID(id);
	cb->pushValue(id);
}

void ObjectInterface::functionMakeObject(void) {
	CBObject *obj = new CBObject;
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	obj->setID(id);
	cb->pushValue(id);
}

void ObjectInterface::functionMakeObjectFloor(void) {
	CBObject *obj = new CBObject(true);
	addToFloorDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	obj->setID(id);
	cb->pushValue(id);
}

void ObjectInterface::functionCloneObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	if (object->isMap()) {
		cb->errors->createError("CloneObject() failed!", "You can't clone a map object");
		return;
	}
	CBObject *obj = object->copyObject();
	if (obj->isFloorObject()) {
		addToFloorDrawOrder(obj);
	}
	else {
		addToDrawOrder(obj);
	}
	int32_t id2 = nextObjectId();
	objectMap[id2] = obj;
	obj->setID(id2);
	cb->pushValue(id2);
}

void ObjectInterface::functionObjectInteger(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getObjectInteger());
}

void ObjectInterface::functionObjectFloat(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getObjectFloat());
}

void ObjectInterface::functionObjectString(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getObjectString());
}

void ObjectInterface::functionObjectLife(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);

	cb->pushValue((int32_t)object->getLife());
}

void ObjectInterface::functionPickedObject(void) {
	cb->pushValue(lastPickedObj);
}

void ObjectInterface::functionPickedX(void) {
	cb->pushValue(lastPickedX);
}

void ObjectInterface::functionPickedY(void) {
	cb->pushValue(lastPickedY);
}

void ObjectInterface::functionPickedAngle(void) {
	cb->pushValue(lastPickedAngle);
}

void ObjectInterface::functionGetAngle2(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = getObject(id2);
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = getObject(id1);

	cb->pushValue((float)((M_PI - atan2f(-object1->getY() + object2->getY(), object1->getX() - object2->getX())) / M_PI * 180.0));
}
inline double square(float num) {
	return (double)num * (double)num;
}

void ObjectInterface::functionDistance2(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = getObject(id2);
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = getObject(id1);
	cb->pushValue((float)sqrt(square(object2->getX()-object1->getX())+square(object2->getY()-object1->getY())));
}

void ObjectInterface::functionObjectX(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getX());
}

void ObjectInterface::functionObjectY(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getY());
}

void ObjectInterface::functionObjectAngle(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getAngle());
}

void ObjectInterface::functionObjectSizeX(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	if (object->type() == CBObject::Map)
	{
		// Maps have their own width function
		// For them getObjectSizeX() returns the texture size!
		cb->pushValue(reinterpret_cast<CBMap *>(object)->getSizeX());
	}
	else {
		if (object->getAngle() == 0.0f) { // If object hasn't been rotated we can skip the expensive trigonometry
			cb->pushValue(object->getObjectSizeX());
		}
		else {
			float size = fabs(object->getObjectSizeX() * cos(object->getAngle() / 180.0 * M_PI)) +
					fabs(sin(object->getAngle() / 180.0 * M_PI) * object->getObjectSizeY());
			cb->pushValue(int32_t(size + 0.5f));
		}
	}
}

void ObjectInterface::functionObjectSizeY(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	if (object->type() == CBObject::Map)
	{
		cb->pushValue(reinterpret_cast<CBMap *>(object)->getSizeY());
	}
	else {
		if (object->getAngle() == 0.0f) { // See functionObjectSizeX
			cb->pushValue(object->getObjectSizeY());
		}
		else {
			float size = fabs(object->getObjectSizeY() * cos(object->getAngle() / 180.0 * M_PI)) +
					fabs(sin((object->getAngle()) / 180.0 * M_PI) * object->getObjectSizeX());
			cb->pushValue(int32_t(size + 0.5f));
		}
	}
}

void ObjectInterface::functionObjectPlaying(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue((int32_t)object->isPlaying());
}

void ObjectInterface::functionObjectFrame(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	cb->pushValue(object->getCurrentFrame());
}

void ObjectInterface::functionObjectsOverlap(void) {
	int32_t touchType = cb->popValue().getInt();
	int32_t id2 = cb->popValue().getInt();
	int32_t id1 = cb->popValue().getInt();
	CBObject *obj1 = getObject(id1);
	CBObject *obj2 = getObject(id2);

	if (obj1 == 0 || obj2 == 0 || touchType < 1 || touchType > 3) {
		cb->pushValue(0);
		return;
	}
	switch (touchType) {
		case 1: { // Box-pick
			float w1 = obj1->getRange1();
			float h1 = obj1->getRange2();
			float x1 = obj1->getX() - w1 / 2;
			float y1 = obj1->getY() + h1 / 2;

			float w2 = obj2->getRange1();
			float h2 = obj2->getRange2();
			float x2 = obj2->getX() - w2 / 2;
			float y2 = obj2->getY() + h2 / 2;

			if (CollisionCheck::RectRectTest(x1, y1, w1, h1, x2, y2, w2, h2)) {
				cb->pushValue(1);
			}
			else {
				cb->pushValue(0);
			}
			break;
		}
		case 2: { // Circle-pick
			float x1 = obj1->getX();
			float y1 = obj1->getY();
			float r1 = obj1->getRange1() / 2.0f;

			float x2 = obj2->getX();
			float y2 = obj2->getY();
			float r2 = obj2->getRange1() / 2.0f;

			if (CollisionCheck::CircleCircleTest(x1, y1, r1, x2, y2, r2)) {
				cb->pushValue(1);
			}
			else {
				cb->pushValue(0);
			}
			break;
		}
		case 3: { // Pixel-pick
			cb->errors->createError("ObjectsOverlap() with pixel picking is not working yet.");
			cb->pushValue(0);
			break;
		}
	}
}

void ObjectInterface::functionObjectSight(void) {
	int32_t objId2 = cb->popValue().getInt();
	int32_t objId1 = cb->popValue().getInt();

	CBObject *obj1 = getObject(objId1);
	CBObject *obj2 = getObject(objId2);

	CBMap* tileMap = cb->getTileMap();

	float x1 = obj1->getX();
	float y1 = obj1->getY();
	float x2 = obj2->getX();
	float y2 = obj2->getY();

	tileMap->worldCoordinatesToMapCoordinates(x1, y1);
	tileMap->worldCoordinatesToMapCoordinates(x2, y2);

	if (!tileMap->mapRayCast(x1, y1, x2, y2)) {
		// No wall between points
		cb->pushValue(1);
	}
	else {
		cb->pushValue(0);
	}
}

void ObjectInterface::functionCountCollisions(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = getObject(id);
	int32_t collisionCount = object->getCollisionCount();
	cb->pushValue(collisionCount);
}

void ObjectInterface::functionGetCollision(void) {
	int32_t collisionId = cb->popValue().getInt();
	int32_t objId = cb->popValue().getInt();
	CBObject *obj = getObject(objId);
	Collision *collision = obj->getCollision(collisionId);
	if (collision == NULL) {
		cb->pushValue(0);
	}
	else {
		cb->pushValue(collision->b->getID());
	}
}

void ObjectInterface::functionCollisionX(void) {
	int32_t collisionId = cb->popValue().getInt();
	int32_t objId = cb->popValue().getInt();
	CBObject *obj = getObject(objId);
	Collision *collision = obj->getCollision(collisionId);
	if (collision == NULL) {
		cb->pushValue(0);
	}
	else {
		cb->pushValue(collision->x);
	}
}

void ObjectInterface::functionCollisionY(void) {
	int32_t collisionId = cb->popValue().getInt();
	int32_t objId = cb->popValue().getInt();
	CBObject *obj = getObject(objId);
	Collision *collision = obj->getCollision(collisionId);
	if (collision == NULL) {
		cb->pushValue(0);
	}
	else {
		cb->pushValue(collision->y);
	}
}

void ObjectInterface::functionCollisionAngle(void) {
	int32_t collisionId = cb->popValue().getInt();
	int32_t objId = cb->popValue().getInt();
	CBObject *obj = getObject(objId);
	Collision *collision = obj->getCollision(collisionId);
	if (collision == NULL) {
		cb->pushValue(0);
	}
	else {
		cb->pushValue(collision->angle);
	}
}

void ObjectInterface::functionNextObject(void) {
	if(iter == objectMap.end()){
		cb->pushValue(0);
		return;
	}
	int32_t handle = (*iter).first;
	iter++;
	cb->pushValue(handle);
}
#endif

CBObject* ObjectInterface::getObject(int32_t key) {
	if (objectMap.count(key) == 0) {
		cb->errors->createError("Object Access Violation", "Could not find object with ID " + boost::lexical_cast<string>(key));
		return 0;
	}
	return objectMap[key];
}

void ObjectInterface::drawObjects(RenderTarget &target) {
	if (cb->getTileMap() == 0 && firstFloorObject == 0 && firstObject == 0) {
		// No objects to be drawn
		return;
	}
	target.setAsCurrent();
	//al_hold_bitmap_drawing(true); //Little speed up

	// All objects are drawn at world coordinates.
	target.useWorldCoords(true);

	// Draw floor objects, including map back layer
	CBObject *currentObject = firstFloorObject;
	while (currentObject != 0) {
		currentObject->render(target);
		currentObject = currentObject->afterObj;
	}

	// Draw normal objects
	currentObject = firstObject;
	while (currentObject != 0) {
		currentObject->render(target);
		currentObject = currentObject->afterObj;
	}

	// Draw map over layer
	if (cb->getTileMap()) {
		cb->getTileMap()->drawLayer(1, target);
	}

	//al_hold_bitmap_drawing(false);

	// Reset drawing to screen
	target.useWorldCoords(false);
}

void ObjectInterface::addToDrawOrder(CBObject *o) {
	if (lastObject == 0) {
		lastObject = firstObject = o;
		o->beforeObj = o->afterObj = 0;
		return;
	}
	lastObject->afterObj = o;
	o->afterObj = 0;
	o->beforeObj = lastObject;
	lastObject = o;
}

void ObjectInterface::addToFloorDrawOrder(CBObject *o) {
	if (firstFloorObject == 0) {
		firstFloorObject = lastFloorObject = o;
		o->afterObj = o->beforeObj = 0;
		return;
	}
	firstFloorObject->beforeObj = o;
	o->beforeObj = 0;
	o->afterObj = firstFloorObject;
	firstFloorObject = o;
}

void ObjectInterface::removeFromDrawOrder(CBObject *o) {
	if (o->isFloorObject()) {
		if (o == lastFloorObject) {
			if (o == firstFloorObject) {
				lastFloorObject = firstFloorObject = 0;
				return;
			}
			o->beforeObj->afterObj = 0;
			lastFloorObject = o->beforeObj;
			return;
		}
		if (o == firstFloorObject) {
			firstFloorObject = o->afterObj;
			o->afterObj->beforeObj = 0;
			return;
		}

		o->afterObj->beforeObj = o->beforeObj;
		o->beforeObj->afterObj = o->afterObj;
	}
	else {
		if (o == lastObject) {
			if (o == firstObject) {
				lastObject = firstObject = 0;
				return;
			}
			o->beforeObj->afterObj = 0;
			lastObject = o->beforeObj;
			return;
		}
		if (o == firstObject) {
			firstObject = o->afterObj;
			o->afterObj->beforeObj = 0;
			return;
		}

		o->afterObj->beforeObj = o->beforeObj;
		o->beforeObj->afterObj = o->afterObj;
	}
}


int32_t ObjectInterface::addMap(CBMap *mapObj){
	int32_t id = nextObjectId();
	objectMap[id] = mapObj;
	mapObj->setID(id);
	return id;
}


void ObjectInterface::updateObjects(){
	int64_t currentTime = mtimer();
	float updateTime = (float)(currentTime-lastUpdate)/1000.0f;
	lastUpdate = currentTime;
	std::map<int32_t,CBObject*>::iterator i;
	for (i = objectMap.begin(); i != objectMap.end();) {
		CBObject *obj = (*i).second;
		if(obj->updateObject(updateTime)){ //updateObject returns true if object should be deleted
			removeFromDrawOrder(obj);
			delete obj;
			i = objectMap.erase(i);
		}
		else {
			++i;
		}
	}
	cb->updateRogueParticles();

	// Iterate over every collision check that is set and run testCollision() on them
	std::vector<CollisionCheck*>::iterator cChkI;
	for (cChkI = collisionChecks.begin(); cChkI != collisionChecks.end(); cChkI++) {
		(*cChkI)->testCollision();
	}

	// Iterate again over every object and reset setCollisionChecking on them
	for (i = objectMap.begin(); i != objectMap.end();) {
		// Reset object collision check to true
		(*i).second->setCollisionChecking(true);
		++i;
	}
}

/** Removes an object from collisionChecks. */
void ObjectInterface::removeFromCollisionCheck(CBObject *obj) {
	std::vector<CollisionCheck*>::iterator iter = collisionChecks.begin();

	while (iter != collisionChecks.end()) {
		if ((*iter)->getObject1() == obj || (*iter)->getObject2() == obj) {
			delete (*iter);
			iter = collisionChecks.erase(iter);
		}
		else {
			++iter;
		}
	}
}

/** Tries to pick an object from given world coordinates. */
void ObjectInterface::pickObject(float x, float y) {
	std::vector<CBObject*>::iterator iter;

	// Init last picked object
	lastPickedObj = 0;

	for (iter = pickableObjects.begin(); iter != pickableObjects.end(); iter++) {
		if ((*iter)->type() == CBObject::Object && (*iter)->canPick(x, y)) {
			lastPickedObj = (*iter)->getID();
			break;
		}
	}
}

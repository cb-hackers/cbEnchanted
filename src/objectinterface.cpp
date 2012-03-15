#include "objectinterface.h"
#include "precomp.h"
#include "debug.h"
#include "cbimage.h"
#include "cbenchanted.h"
#include "mapinterface.h"
#include "gfxinterface.h"
#include "util.h"

ObjectInterface::ObjectInterface():
	lastUpdate(0),
	firstObject(0),
	firstFloorObject(0),
	lastObject(0),
	lastFloorObject(0)
{
	cb = static_cast<CBEnchanted*>(this);
}

ObjectInterface::~ObjectInterface() {
	
}

void ObjectInterface::commandDeleteObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	if (object->type() == CBObject::ParticleEmitter) {
		objectMap.erase(id);
		cb->deleteParticleEmitter(static_cast<CBParticleEmitter*>(object));
		return;
	}
	removeFromDrawOrder(object);
	delete object;
	objectMap.erase(id);
}

void ObjectInterface::commandClearObjects(void) {
	for (std::map<int32_t,CBObject*>::iterator i = objectMap.begin();i != objectMap.end();i++)
	{
		delete i->second;
	}
	objectMap.clear();
	objectDrawOrder.clear();
}

void ObjectInterface::commandMoveObject(void) {
	float z = cb->popValue().toFloat();
	float side = cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->moveObject(fwrd,side);
}

void ObjectInterface::commandTranslateObject(void) {
	float z = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->translateObject(x,y,z);
}

void ObjectInterface::commandPositionObject(void) {
	cb->popValue(); //Z?
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->positionObject(x,y);
}

void ObjectInterface::commandScreenPositionObject(void) {
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->positionObject(cb->screenCoordToWorld(sf::Vector2f(x,y)));
}

void ObjectInterface::commandTurnObject(void) {
	//Random shit...
	cb->popValue();
	cb->popValue();

	float a = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->turnObject(a);
}

void ObjectInterface::commandRotateObject(void) {
	cb->popValue(); //Something?
	cb->popValue(); //Something?
	float a = cb->popValue().toFloat();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->rotateObject(a);
}

void ObjectInterface::commandPointObject(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = objectMap[id2];
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = objectMap[id1];

	object1->rotateObject((3.14159265358979323 - atan2f(-object2->getY() + object1->getY(), object1->getX() - object2->getX())) / 3.14159265358979323 * 180.0);
}

void ObjectInterface::commandCloneObjectPosition(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = objectMap[id2];
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = objectMap[id1];
	object1->positionObject(object2->getPos());
}

void ObjectInterface::commandCloneObjectOrientation(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = objectMap[id2];
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = objectMap[id1];
	object1->rotateObject(object2->getAngle());
}

void ObjectInterface::commandObjectOrder(void) {
	int32_t select = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	if (object->isFloorObject()) {
		if (select == 1) {//Move to top
			if (object == firstFloorObject) return;
			if (object == lastFloorObject) {
				object->lastObj->nextObj = 0;
				lastFloorObject = object->lastObj;
				firstFloorObject->lastObj = object;
				object->nextObj = firstFloorObject;
				object->lastObj = 0;
				firstFloorObject = object;

				return;
			}

			object->nextObj->lastObj = object->lastObj;
			object->lastObj->nextObj = object->nextObj;
			firstFloorObject->lastObj = object;
			object->nextObj = firstFloorObject;
			object->lastObj = 0;
		}
		else if (select == -1) { //Move to last
			if (object == lastObject) return;
			if (object == firstObject) {
				object->nextObj->lastObj = 0;
				firstFloorObject = object->nextObj;

				lastFloorObject->nextObj = object;
				object->nextObj = 0;
				object->lastObj = lastFloorObject;
				lastFloorObject = object;
				return;
			}
			object->nextObj->lastObj = object->lastObj;
			object->lastObj->nextObj = object->nextObj;
			lastFloorObject->nextObj = object;
			object->lastObj = lastFloorObject;
			object->nextObj = 0;
			lastFloorObject = object;
		}
	}
	else {
		if (select == 1) {//Move to top
			if (object == firstObject) return;
			if (object == lastObject) {
				object->lastObj->nextObj = 0;
				firstObject->lastObj = object;
				object->nextObj = firstObject;
				object->lastObj = 0;
				firstObject = object;

				return;
			}

			object->nextObj->lastObj = object->lastObj;
			object->lastObj->nextObj = object->nextObj;
			firstObject->lastObj = object;
			object->nextObj = firstObject;
			object->lastObj = 0;
		}
		else if (select == -1) { //Move to last
			if (object == lastObject) return;
			if (object == firstObject) {
				object->nextObj->lastObj = 0;
				lastObject = object->lastObj;
				firstObject = object->nextObj;

				lastObject->nextObj = object;
				object->nextObj = 0;
				object->lastObj = lastObject;
				lastObject = object;
				return;
			}
			object->nextObj->lastObj = object->lastObj;
			object->lastObj->nextObj = object->nextObj;
			lastObject->nextObj = object;
			object->lastObj = lastObject;
			object->nextObj = 0;
			lastObject = object;
		}
	}
}

void ObjectInterface::commandMaskObject(void) {
	uint8_t b = cb->popValue().toByte();
	uint8_t g = cb->popValue().toByte();
	uint8_t r = cb->popValue().toByte();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->maskObject(r,g,b);
}

void ObjectInterface::commandShowObject(void) {
	bool t = cb->popValue().toInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->showObject(t);
}

void ObjectInterface::commandDefaultVisible(void) {
	bool t = cb->popValue().toInt();
	CBObject::setDefaultVisible(t);
}

void ObjectInterface::commandPaintObject(void) {
	int p = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];

    if (p > 0 && !object->isFloorObject()) { //Object
		CBObject *object2 = objectMap[p];
		object->paintObject(*object2);
	}
	else { //Image
		CBImage *img = cb->getImage(p);
		object->paintObject(img->getRenderTarget()->getSurface()->getTexture());
	}
}

void ObjectInterface::commandGhostObject(void) {
	float a = cb->popValue().toFloat() / 100.0;
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->ghostObject((uint8_t)(a*255.0));

}

void ObjectInterface::commandMirrorObject(void) {
	STUB;
}

void ObjectInterface::commandObjectRange(void) {
	STUB;
}

void ObjectInterface::commandObjectInteger(void) {
	STUB;
}

void ObjectInterface::commandObjectFloat(void) {
	STUB;
}

void ObjectInterface::commandObjectString(void) {
	STUB;
}

void ObjectInterface::commandObjectPickable(void) {
	STUB;
}

void ObjectInterface::commandObjectPick(void) {
	STUB;
}

void ObjectInterface::commandPixelPick(void) {
	STUB;
}

void ObjectInterface::commandObjectLife(void) {
	STUB;
}

void ObjectInterface::commandPlayObject(void) {
	bool continuous = cb->popValue().toInt();
	float speed = cb->popValue().toFloat();
	uint16_t endf = cb->popValue().toInt();
	uint16_t startf = cb->popValue().toInt();
	int32_t id = cb->popValue().toInt();
	CBObject *object = objectMap[id];

	object->startPlaying(startf, endf, speed, continuous);
	object->setLooping(false);
}

void ObjectInterface::commandLoopObject(void) {
	bool continuous = cb->popValue().toInt();
	float speed = cb->popValue().toFloat();
	uint16_t endf = cb->popValue().toInt();
	uint16_t startf = cb->popValue().toInt();
	int32_t id = cb->popValue().toInt();
	CBObject *object = objectMap[id];

	object->startPlaying(startf, endf, speed, continuous);
	object->setLooping(true);
}

void ObjectInterface::commandStopObject(void) {
	STUB;
}

void ObjectInterface::commandResetObjectCollision(void) {
	STUB;
}

void ObjectInterface::commandSetupCollision(void) {
	STUB;
}

void ObjectInterface::commandClearCollisions(void) {
	STUB;
}

void ObjectInterface::commandInitObjectList(void) {
	iter = objectMap.begin();
}

void ObjectInterface::functionLoadObject(void) {
	cb->popValue(); //Rotation...
	const string &path = cb->popValue().getString().getStdString();
	CBObject *obj = new CBObject;
	if (!obj->load(path)) {
		FIXME("Can't load object: %s",path.c_str());
		cb->pushValue(0);
		return;
	}
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionLoadAnimObject(void) {
	cb->popValue().toInt();
	uint16_t frames = cb->popValue().toInt();
	uint16_t startf = cb->popValue().toInt();
	uint16_t frameH = cb->popValue().toInt();
	uint16_t frameW = cb->popValue().toInt();
	const string &path = cb->popValue().getString().getStdString();
	CBObject *obj = new CBObject;
	if(!obj->loadAnimObject(path, frameW, frameH, startf, frames)){
		FIXME("Can't load object: %s", path.c_str());
		cb->pushValue(0);
		return;
	}
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionMakeObject(void) {
	CBObject *obj = new CBObject;
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionMakeObjectFloor(void) {
	CBObject *obj = new CBObject(true);
	addToDrawOrder(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionCloneObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	CBObject *obj = object->copyObject();
	addToDrawOrder(obj);
	int32_t id2 = nextObjectId();
	objectMap[id2] = obj;
	cb->pushValue(id2);
}

void ObjectInterface::functionObjectInteger(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getObjectInteger());
}

void ObjectInterface::functionObjectFloat(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getObjectFloat());
}

void ObjectInterface::functionObjectString(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getObjectString());
}

void ObjectInterface::functionObjectLife(void) {
	int32_t life = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	object->setLife(life);
}

void ObjectInterface::functionPickedObject(void) {
	STUB;
}

void ObjectInterface::functionPickedX(void) {
	STUB;
}

void ObjectInterface::functionPickedY(void) {
	STUB;
}

void ObjectInterface::functionPickedAngle(void) {
	STUB;
}

void ObjectInterface::functionGetAngle2(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = objectMap[id2];
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = objectMap[id1];

	cb->pushValue((float)((3.14159265358979323 - atan2f(-object2->getY() + object1->getY(), object1->getX() - object2->getX())) / 3.14159265358979323 * 180.0));
}
inline double square(float num) {
	return (double)num * (double)num;
}

void ObjectInterface::functionDistance2(void) {
	int32_t id2 = cb->popValue().getInt();
	CBObject *object2 = objectMap[id2];
	int32_t id1 = cb->popValue().getInt();
	CBObject *object1 = objectMap[id1];
	cb->pushValue((float)sqrt(square(object2->getX()-object1->getX())+square(object2->getY()-object1->getY())));
}

void ObjectInterface::functionObjectX(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getX());
}

void ObjectInterface::functionObjectY(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getY());
}

void ObjectInterface::functionObjectAngle(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getAngle());
}

void ObjectInterface::functionObjectSizeX(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getObjectSizeX());
}

void ObjectInterface::functionObjectSizeY(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getObjectSizeY());
}

void ObjectInterface::functionObjectPlaying(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue((int32_t)object->isPlaying());
}

void ObjectInterface::functionObjectFrame(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	cb->pushValue(object->getCurrentFrame());
}

void ObjectInterface::functionObjectsOverlap(void) {
	STUB;
}

void ObjectInterface::functionObjectSight(void) {
	STUB;
}

void ObjectInterface::functionCountCollisions(void) {
	STUB;
}

void ObjectInterface::functionGetCollision(void) {
	STUB;
}

void ObjectInterface::functionCollisionX(void) {
	STUB;
}

void ObjectInterface::functionCollisionY(void) {
	STUB;
}

void ObjectInterface::functionCollisionAngle(void) {
	STUB;
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

void ObjectInterface::drawObjects(RenderTarget &target) {
	target.setViewTo(false);
	CBObject *currentObject = firstFloorObject;
	while (currentObject != 0) {
		currentObject->render(target);
		currentObject = currentObject->nextObj;
	}
	if (cb->getTileMap()) cb->getTileMap()->drawLayer(0, target);
	target.setViewTo(true);
	currentObject = firstObject;
	while (currentObject != 0) {
		currentObject->render(target);
		currentObject = currentObject->nextObj;
	}
	target.setViewTo(false);
	if (cb->getTileMap()) cb->getTileMap()->drawLayer(1, target);
}


int32_t ObjectInterface::addMap(CBMap *mapObj){
	int32_t id = nextObjectId();
	objectMap[id] = mapObj;
	return id;
}


void ObjectInterface::updateObjects(){
	int64_t currentTime = mtimer();
	float updateTime = (float)(currentTime-lastUpdate)/1000.0f;
	lastUpdate = currentTime;
	std::map<int32_t,CBObject*>::iterator i;
	for (i = objectMap.begin(); i != objectMap.end();) {
		if((*i).second->updateObject(updateTime)){ //updateObject returns true if object should be deleted
			removeFromDrawOrder((*i).second);
			delete (*i).second;
			//i = objectMap.erase(i);
		}
		else {
			++i;
		}
	}
	cb->updateRogueParticles();
}

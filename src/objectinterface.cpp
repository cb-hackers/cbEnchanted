#include "objectinterface.h"
#include "precomp.h"
#include "debug.h"
#include "cbimage.h"
#include "cbenchanted.h"

ObjectInterface::ObjectInterface() {
	cb = static_cast<CBEnchanted*>(this);
}

ObjectInterface::~ObjectInterface() {
	
}

void ObjectInterface::commandDeleteObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	std::vector<CBObject*>::const_iterator i = objectDrawOrder.cbegin() + object->getDrawOrderNumber();
	objectDrawOrder.erase(i);
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
	int32_t oldPlace = object->getDrawOrderNumber();
	if (select == 1) {//move to first
		std::vector<CBObject*>::iterator it = --objectDrawOrder.end();
		for (int32_t i = (*it)->getDrawOrderNumber(); i != oldPlace;)
		{
			(*it)->setDrawOrderNumber(--i);
			--it;
		}
		objectDrawOrder.erase(it);//remove old
		objectDrawOrder.push_back(object);//insert to end
	}
	else if (select == -1) {
		std::vector<CBObject*>::iterator it = objectDrawOrder.begin();
		for (int32_t i = 0; i != oldPlace;)
		{
			(*it)->setDrawOrderNumber(++i);
			--it;
		}
		objectDrawOrder.erase(it);//remove old
		objectDrawOrder.insert(objectDrawOrder.begin(),object);//insert to begin
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

	if (p > 0) { //Object
		CBObject *object2 = objectMap[p];
		object->paintObject(*object2);
	}
	else { //Image
		CBImage *img = cb->getImage(p);
		object->paintObject(img->getRenderTarget()->getSurface()->GetTexture());
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
	STUB;
}

void ObjectInterface::commandLoopObject(void) {
	STUB;
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
	STUB;
}

void ObjectInterface::functionLoadObject(void) {
	cb->popValue(); //Rotation...
	string path = cb->popValue().getString();
	CBObject *obj = new CBObject;
	if (!obj->load(path)) {
		FIXME("Can't load object: %s",path.c_str());
		cb->pushValue(0);
		return;
	}
	obj->setDrawOrderNumber(objectDrawOrder.size());
	objectDrawOrder.push_back(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionLoadAnimObject(void) {

}

void ObjectInterface::functionMakeObject(void) {
	CBObject *obj = new CBObject;
	obj->setDrawOrderNumber(objectDrawOrder.size());
	objectDrawOrder.push_back(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionMakeObjectFloor(void) {
	CBObject *obj = new CBObject(true);
	obj->setDrawOrderNumber(objectDrawOrder.size());
	objectDrawOrder.push_back(obj);
	int32_t id = nextObjectId();
	objectMap[id] = obj;
	cb->pushValue(id);
}

void ObjectInterface::functionCloneObject(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *object = objectMap[id];
	CBObject *obj = object->copyObject();
	obj->setDrawOrderNumber(objectDrawOrder.size());
	objectDrawOrder.push_back(obj);
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
	STUB;
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
	STUB;
}

void ObjectInterface::functionObjectFrame(void) {
	STUB;
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
	STUB;
}

void ObjectInterface::drawObjects(RenderTarget &target) {
	target.setViewTo(true);
	for (std::vector<CBObject*>::iterator i = objectDrawOrder.begin();i != objectDrawOrder.end();i++) {
		(*i)->render(target);
	}
}

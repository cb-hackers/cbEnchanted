#include "precomp.h"
#include "camerainterface.h"
#include "cbenchanted.h"
#include "cbobject.h"

CameraInterface::CameraInterface(): cb(static_cast<CBEnchanted*>(this)),cameraX(0),cameraY(0),cameraAngle(0){
	
}

CameraInterface::~CameraInterface() {
	
}

void CameraInterface::commandCloneCameraPosition(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *obj = cb->getObject(id);
	cameraX = obj->getX();
	cameraY = obj->getY();
	if (cb->getCurrentRenderTarget()->isDrawToWorldViewOn()) cb->getCurrentRenderTarget()->setViewTo(true,true);
}

void CameraInterface::commandCloneCameraOrientation(void) {
<<<<<<< HEAD
    int32_t id = cb->popValue().getInt();
    CBObject *obj = cb->getObject(id);
    cameraAngle = obj->getAngle();
=======
	int32_t id = cb->popValue().getInt();
	CBObject *obj = cb->getObject(id);
	cameraAngle = obj->getX();
>>>>>>> e12a924043602aac1b66de14f4fbf301f1e51a94
}

void CameraInterface::commandCameraFollow(void) {
	STUB;
}

void CameraInterface::commandCameraPick(void) {
	STUB;
}

void CameraInterface::commandPointCamera(void) {
	int32_t id = cb->popValue().toInt();
	CBObject *obj = cb->getObject(id);
	cameraAngle =  (3.14159265358979323 - atan2f(cameraY - obj->getY(), cameraX - obj->getY())) / 3.14159265358979323 * 180.0;
}

void CameraInterface::commandTurnCamera(void) {
	cameraAngle += cb->popValue().toFloat();
}

void CameraInterface::commandRotateCamera(void) {
	cameraAngle = cb->popValue().toFloat();
}

void CameraInterface::commandMoveCamera(void) {
	float right = cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();
	cameraX += cosf(cameraAngle*M_PI/180.0)*fwrd;
	cameraY += sinf(cameraAngle*M_PI/180.0)*fwrd;
	if (cb->getCurrentRenderTarget()->isDrawToWorldViewOn()) cb->getCurrentRenderTarget()->setViewTo(true,true);
}

void CameraInterface::commandTranslateCamera(void) {
	cameraX += cb->popValue().toFloat();
	cameraY += cb->popValue().toFloat();
<<<<<<< HEAD

	INFO("%f, %f", cameraX, cameraY);
=======
	if (cb->getCurrentRenderTarget()->isDrawToWorldViewOn()) cb->getCurrentRenderTarget()->setViewTo(true,true);
>>>>>>> e12a924043602aac1b66de14f4fbf301f1e51a94
}

void CameraInterface::commandPositionCamera(void) {
	cameraY = cb->popValue().toFloat();
	cameraX = cb->popValue().toFloat();
	if (cb->getCurrentRenderTarget()->isDrawToWorldViewOn()) cb->getCurrentRenderTarget()->setViewTo(true,true);
}

void CameraInterface::functionCameraX(void) {
	cb->pushValue(cameraX);
}

void CameraInterface::functionCameraY(void) {
	cb->pushValue(cameraY);
}

void CameraInterface::functionCameraAngle(void) {
	cb->pushValue(cameraAngle);
}

sf::Vector2f CameraInterface::screenCoordToWorld(const sf::Vector2f &v) {
	return sf::Vector2f(v.x-cb->getWindow()->GetWidth()/2.0f+cameraX,-v.y+cb->getWindow()->GetHeight()/2.0f+cameraY);
}

sf::Vector2f CameraInterface::worldCoordToScreen(const sf::Vector2f &v) {
	return sf::Vector2f(v.x+cb->getWindow()->GetWidth()/2.0f-cameraX,-v.y+cb->getWindow()->GetHeight()/2.0f+cameraY);
}

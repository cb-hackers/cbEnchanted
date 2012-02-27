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
	int32_t id = cb->popValue().getInt();
	CBObject *obj = cb->getObject(id);
	cameraAngle = obj->getX();
}

void CameraInterface::commandCameraFollow(void) {
	STUB;
}

void CameraInterface::commandCameraPick(void) {
	STUB;
}

void CameraInterface::commandPointCamera(void) {
	STUB;
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
	if (cb->getCurrentRenderTarget()->isDrawToWorldViewOn()) cb->getCurrentRenderTarget()->setViewTo(true,true);
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

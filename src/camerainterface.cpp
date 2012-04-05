#include "precomp.h"
#include "camerainterface.h"
#include "cbenchanted.h"
#include "cbobject.h"

CameraInterface::CameraInterface(): cameraX(0), cameraY(0), cameraAngle(0) {
	cb = static_cast <CBEnchanted *> (this);
}

CameraInterface::~CameraInterface() {

}

void CameraInterface::commandCloneCameraPosition(void) {
	int32_t id = cb->popValue().getInt();

	CBObject *obj = cb->getObject(id);
	cameraX = obj->getX();
	cameraY = obj->getY();
}

void CameraInterface::commandCloneCameraOrientation(void) {
	int32_t id = cb->popValue().getInt();
	CBObject *obj = cb->getObject(id);
	cameraAngle = obj->getAngle();
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
	cameraAngle = (M_PI - atan2f(cameraY - obj->getY(), cameraX - obj->getY())) / M_PI * 180.0;
}

void CameraInterface::commandTurnCamera(void) {
	cameraAngle += cb->popValue().toFloat();
}

void CameraInterface::commandRotateCamera(void) {
	cameraAngle = cb->popValue().toFloat();
}

void CameraInterface::commandMoveCamera(void) {
	cameraZ += cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();
	float right = cb->popValue().toFloat();

	cameraX += cosf(cameraAngle * M_PI / 180.0) * fwrd;
	cameraY += sinf(cameraAngle * M_PI / 180.0) * fwrd;
}

void CameraInterface::commandTranslateCamera(void) {
	cameraZ += cb->popValue().toFloat();
	cameraY += cb->popValue().toFloat();
	cameraX += cb->popValue().toFloat();
}

void CameraInterface::commandPositionCamera(void) {
	cameraZ = cb->popValue().toFloat();
	cameraY = cb->popValue().toFloat();
	cameraX = cb->popValue().toFloat();
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
float CameraInterface::screenCoordToWorldX(float a) {
	return a - al_get_display_width(cb->getWindow()) *0.5f + cameraX;
}

float CameraInterface::screenCoordToWorldY(float a) {
	return -a + al_get_display_height(cb->getWindow()) / 2.0f + cameraY;
}

float CameraInterface::worldCoordToScreenX(float a) {
	return a + al_get_display_width(cb->getWindow()) / 2.0f - cameraX;
}

float CameraInterface::worldCoordToScreenY(float a) {
	return -a + al_get_display_height(cb->getWindow()) / 2.0f + cameraY;
}

#include "precomp.h"
#include "camerainterface.h"
#include "interfacesystem.h"
#include "cbobject.h"

CameraInterface::CameraInterface(): cameraX(0), cameraY(0), cameraAngle(0) {
	sys = static_cast <InterfaceSystem *> (this);
}

CameraInterface::~CameraInterface() {

}

void CameraInterface::commandCloneCameraPosition(void) {
	int32_t id = sys->popValue().getInt();

	CBObject *obj = sys->getObject(id);
	cameraX = obj->getX();
	cameraY = obj->getY();
}

void CameraInterface::commandCloneCameraOrientation(void) {
	int32_t id = sys->popValue().getInt();
	CBObject *obj = sys->getObject(id);
	cameraAngle = obj->getAngle();
}

void CameraInterface::commandCameraFollow(void) {
	STUB;
}

void CameraInterface::commandCameraPick(void) {
	STUB;
}

void CameraInterface::commandPointCamera(void) {
	int32_t id = sys->popValue().toInt();
	CBObject *obj = sys->getObject(id);
	cameraAngle = (M_PI - atan2f(cameraY - obj->getY(), cameraX - obj->getY())) / M_PI * 180.0;
}

void CameraInterface::commandTurnCamera(void) {
	cameraAngle += sys->popValue().toFloat();
}

void CameraInterface::commandRotateCamera(void) {
	cameraAngle = sys->popValue().toFloat();
}

void CameraInterface::commandMoveCamera(void) {
	cameraZ += sys->popValue().toFloat();
	float fwrd = sys->popValue().toFloat();
	float right = sys->popValue().toFloat();

	cameraX += cosf(cameraAngle * M_PI / 180.0) * fwrd;
	cameraY += sinf(cameraAngle * M_PI / 180.0) * fwrd;
}

void CameraInterface::commandTranslateCamera(void) {
	cameraZ += sys->popValue().toFloat();
	cameraY += sys->popValue().toFloat();
	cameraX += sys->popValue().toFloat();
}

void CameraInterface::commandPositionCamera(void) {
	cameraZ = sys->popValue().toFloat();
	cameraY = sys->popValue().toFloat();
	cameraX = sys->popValue().toFloat();
}

void CameraInterface::functionCameraX(void) {
	sys->pushValue(cameraX);
}

void CameraInterface::functionCameraY(void) {
	sys->pushValue(cameraY);
}

void CameraInterface::functionCameraAngle(void) {
	sys->pushValue(cameraAngle);
}
float CameraInterface::screenCoordToWorldX(float a) {
	return a - al_get_display_width(sys->getWindow()) *0.5f + cameraX;
}

float CameraInterface::screenCoordToWorldY(float a) {
	return -a + al_get_display_height(sys->getWindow()) / 2.0f + cameraY;
}

float CameraInterface::worldCoordToScreenX(float a) {
	return a + al_get_display_width(sys->getWindow()) / 2.0f - cameraX;
}

float CameraInterface::worldCoordToScreenY(float a) {
	return -a + al_get_display_height(sys->getWindow()) / 2.0f + cameraY;
}

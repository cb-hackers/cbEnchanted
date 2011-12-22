#include "precomp.h"
#include "camerainterface.h"
#include "cbenchanted.h"

CameraInterface::CameraInterface(): cb(static_cast<CBEnchanted*>(this)){
	
}

CameraInterface::~CameraInterface() {
	
}

void CameraInterface::commandCloneCameraPosition(void) {
	
}

void CameraInterface::commandCloneCameraOrientation(void) {
	
}

void CameraInterface::commandCameraFollow(void) {
	
}

void CameraInterface::commandCameraPick(void) {
	
}

void CameraInterface::commandPointCamera(void) {
	
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
}

void CameraInterface::commandTranslateCamera(void) {
    cameraX += cb->popValue().toFloat();
    cameraY += cb->popValue().toFloat();
}

void CameraInterface::commandPositionCamera(void) {
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

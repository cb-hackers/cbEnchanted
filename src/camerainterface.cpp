#include "precomp.h"
#include "camerainterface.h"
#include "mathinterface.h"
#include "cbenchanted.h"
#include "cbobject.h"

CameraInterface::CameraInterface():
	cameraX(0),
	cameraY(0),
	cameraAngle(0),
	isFollowing(false),
	followTarget(NULL),
	followStyle(0),
	followSetting(0.0f)
{
	cb = static_cast <CBEnchanted *> (this);
}

CameraInterface::~CameraInterface() {

}

void CameraInterface::commandCloneCameraPosition(void) {
	int32_t id = cb->popValue().getInt();
	isFollowing = false;
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
	isFollowing = true;
	followSetting = cb->popValue().toFloat();
	followStyle = cb->popValue().toInt();
	followTarget = cb->getObject(cb->popValue().toInt());
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
	cb->popValue();
	cb->popValue();
	cameraAngle = MathInterface::wrapAngle(cameraAngle + cb->popValue().toFloat());
}

void CameraInterface::commandRotateCamera(void) {
	cb->popValue();
	cb->popValue();
	cameraAngle = MathInterface::wrapAngle(cb->popValue().toFloat());
}

void CameraInterface::commandMoveCamera(void) {
	cameraZ += cb->popValue().toFloat();
	float side = cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();

	cameraX += cosf(cameraAngle * M_PI / 180.0f) * fwrd;
	cameraY += sinf(cameraAngle * M_PI / 180.0f) * fwrd;
	cameraX += cosf((cameraAngle + 90.0f) * M_PI / 180.0f) * side;
	cameraY += sinf((cameraAngle + 90.0f) * M_PI / 180.0f) * side;
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

void CameraInterface::updateCamFollow() {
	switch (followStyle) {
		case 1: {
			cameraX = cameraX + float(followTarget->getX() - cameraX) / followSetting;
			cameraY = cameraY + float(followTarget->getY() - cameraY) / followSetting;
		}
		break;
		case 2: {

			if (followTarget->getX() < cameraX - cb->screenWidth()/2 + followSetting) {
				cameraX = cameraX + (followTarget->getX() - (cameraX - cb->screenWidth()/2 + followSetting));
			}
			if (followTarget->getX() > cameraX + cb->screenWidth()/2 - followSetting) {
				cameraX = cameraX + (followTarget->getX() - (cameraX + cb->screenWidth()/2 - followSetting));
			}
			if (followTarget->getY() < cameraY - cb->screenHeight()/2 + followSetting) {
				cameraY = cameraY + (followTarget->getY() - (cameraY - cb->screenHeight()/2 + followSetting));
			}
			if (followTarget->getY() > cameraY + cb->screenHeight()/2 - followSetting) {
				cameraY = cameraY + (followTarget->getY() - (cameraY + cb->screenHeight()/2 - followSetting));
			}
		}
		break;
		case 3: {

			float radAng = followTarget->getAngle() / 180.0 * M_PI;
			cameraX = followTarget->getX() + cos(radAng) * followSetting;
			cameraY = followTarget->getY() + sin(radAng) * followSetting;
		}
		break;
	}
}

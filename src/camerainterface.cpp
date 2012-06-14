#include "precomp.h"
#include "camerainterface.h"
#include "mathinterface.h"
#include "cbenchanted.h"
#include "cbobject.h"
#define MIN_CAMERA_ZOOM 0.00001f

CameraInterface::CameraInterface():
	cameraX(0),
	cameraY(0),
	cameraAngle(0),
	isFollowing(false),
	followTarget(NULL),
	followStyle(0),
	followSetting(0.0f),
	worldTransformDirty(true),
	inverseWorldTransformDirty(true),
	pixelPreciseWorldTransformDirty(true),
	cameraRadAngle(0),
	cameraZoom(1.0f)
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
	cameraMoved();
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
	cameraRadAngle += (cb->popValue().toFloat() / 180.0f) * M_PI;
	while (cameraRadAngle < 0) {
		cameraRadAngle += 2 * M_PI;
	}
	while (cameraRadAngle > 2 * M_PI) {
		cameraRadAngle -= 2* M_PI;
	}
	cameraAngle = MathInterface::wrapAngle(cameraAngle + cb->popValue().toFloat());
	cameraMoved();
}

void CameraInterface::commandRotateCamera(void) {
	cb->popValue();
	cameraRadAngle = (MathInterface::wrapAngle(cb->popValue().toFloat()) / 180.0f) * M_PI;
	cameraAngle = MathInterface::wrapAngle(cb->popValue().toFloat());
	cameraMoved();
}

void CameraInterface::commandMoveCamera(void) {
	cameraZoom += cb->popValue().toFloat();
	if (cameraZoom < MIN_CAMERA_ZOOM) cameraZoom = MIN_CAMERA_ZOOM;
	float side = cb->popValue().toFloat();
	float fwrd = cb->popValue().toFloat();
	float moveAngle = (cameraAngle / 180.0f) * M_PI + cameraRadAngle;
	cameraX += cosf(moveAngle) * fwrd;
	cameraY += sinf(moveAngle) * fwrd;
	cameraX += cosf(moveAngle + M_PI * 0.5f) * side;
	cameraY += sinf(moveAngle + M_PI * 0.5f) * side;
	cameraMoved();
}

void CameraInterface::commandTranslateCamera(void) {
	cameraZoom += cb->popValue().toFloat();
	if (cameraZoom < MIN_CAMERA_ZOOM) cameraZoom = MIN_CAMERA_ZOOM;
	cameraY += cb->popValue().toFloat();
	cameraX += cb->popValue().toFloat();
	cameraMoved();
}

void CameraInterface::commandPositionCamera(void) {
	cameraZoom = cb->popValue().toFloat();
	if (cameraZoom < MIN_CAMERA_ZOOM) cameraZoom = MIN_CAMERA_ZOOM;
	cameraY = cb->popValue().toFloat();
	cameraX = cb->popValue().toFloat();
	cameraMoved();
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

void CameraInterface::screenCoordToWorld(float &x, float &y) {
	al_transform_coordinates(getInverseWorldTransform(), &x, &y);
	y = -y;
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
	cameraMoved();
}



/** Returns the width of the drawing area after all transformations are applied */
float CameraInterface::getDrawAreaWidth() {
	return (
		fabs(cos(cameraRadAngle) * cb->screenWidth()) +
		fabs(sin(cameraRadAngle) * cb->screenHeight())
	) * (1 / cameraZoom);
}

/** Returns the height of the drawing area after all transformations are applied */
float CameraInterface::getDrawAreaHeight() {
	return (
		fabs(cos(cameraRadAngle) * cb->screenHeight()) +
		fabs(sin(cameraRadAngle) * cb->screenWidth())
	) * (1 / cameraZoom);
}

ALLEGRO_TRANSFORM *CameraInterface::getWorldTransform() {
	if (worldTransformDirty) {
		al_identity_transform(&worldTransform);
		al_translate_transform(&worldTransform, -cameraX, cameraY);
		al_rotate_transform(&worldTransform, cameraRadAngle);
		al_scale_transform(&worldTransform, cameraZoom, cameraZoom);
		al_translate_transform(&worldTransform, al_get_display_width(cb->getWindow()) / 2, al_get_display_height(cb->getWindow()) / 2);

		worldTransformDirty = false;
	}
	return &worldTransform;
}

ALLEGRO_TRANSFORM *CameraInterface::getInverseWorldTransform() {
	if (inverseWorldTransformDirty) {
		al_copy_transform(&inverseWorldTransform, &worldTransform);
		al_invert_transform(&inverseWorldTransform);
		inverseWorldTransformDirty = false;
	}
	return &inverseWorldTransform;
}

/** Fix for drawings working weirdly on some machines, when draw
  * coordinate is in the middle of a pixel.
  */
ALLEGRO_TRANSFORM *CameraInterface::getPixelPreciseWorldTransform() {
	if (pixelPreciseWorldTransformDirty) {
		al_copy_transform(&pixelPreciseWorldTransform, &worldTransform);
		pixelPreciseWorldTransform.m[3][0] = floorf(pixelPreciseWorldTransform.m[3][0]);
		pixelPreciseWorldTransform.m[3][1] = floorf(pixelPreciseWorldTransform.m[3][1]);
		pixelPreciseWorldTransformDirty = false;
	}
	return &pixelPreciseWorldTransform;
}

void CameraInterface::cameraMoved() {
	inverseWorldTransformDirty = true;
	worldTransformDirty = true;
	pixelPreciseWorldTransformDirty = true;
}

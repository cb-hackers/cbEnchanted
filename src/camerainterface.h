#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H
class CBEnchanted;
#include "precomp.h"

class CBObject;

class CameraInterface {
	public:
#ifndef CBE_LIB
		CameraInterface();
		~CameraInterface();

		void commandCloneCameraPosition(void);
		void commandCloneCameraOrientation(void);
		void commandCameraFollow(void);
		void commandCameraPick(void);
		void commandPointCamera(void);
		void commandTurnCamera(void);
		void commandRotateCamera(void);
		void commandMoveCamera(void);
		void commandTranslateCamera(void);
		void commandPositionCamera(void);

		void functionCameraX(void);
		void functionCameraY(void);
		void functionCameraAngle(void);
#endif
		float getCameraX(void) { return cameraX; }
		float getCameraY(void) { return cameraY; }
		float getCameraZoom(void) { return cameraZoom; }
		float getCameraRealAngle(void) { return cameraRadAngle * 180.0f / M_PI; }
		void screenCoordToWorld(float &x, float &y);
		void worldCoordToScreen(float &x, float &y);

		void updateCamFollow();
		inline bool isCamFollowing() {return isFollowing;}

		ALLEGRO_TRANSFORM *getWorldTransform();
		ALLEGRO_TRANSFORM *getInverseWorldTransform();
		ALLEGRO_TRANSFORM *getPixelPreciseWorldTransform();

		/** Returns the width of the drawing area after all transformations are applied */
		float getDrawAreaWidth();
		/** Returns the height of the drawing area after all transformations are applied */
		float getDrawAreaHeight();
	private:
		void cameraMoved();
		CBEnchanted *cb;
		float cameraX;
		float cameraY;
		float cameraAngle;
		float cameraRadAngle;
		float cameraZoom;

		bool isFollowing;
		CBObject* followTarget;
		uint8_t followStyle;
		float followSetting;
		ALLEGRO_TRANSFORM worldTransform;
		ALLEGRO_TRANSFORM inverseWorldTransform;
		ALLEGRO_TRANSFORM pixelPreciseWorldTransform;
		bool worldTransformDirty;
		bool inverseWorldTransformDirty;
		bool pixelPreciseWorldTransformDirty;
};

#endif

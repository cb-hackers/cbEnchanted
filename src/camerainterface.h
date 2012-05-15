#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H
class CBEnchanted;
#include "precomp.h"
#include "cbobject.h"
class CameraInterface {
	public:
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

		float getCameraX(void) { return cameraX; }
		float getCameraY(void) { return cameraY; }
		float screenCoordToWorldX(float a);
		float screenCoordToWorldY(float a);
		float worldCoordToScreenX(float a);
		float worldCoordToScreenY(float a);

		void updateCamFollow();
		inline bool isCamFollowing() {return isFollowing;}

	private:
		CBEnchanted *cb;
		float cameraX;
		float cameraY;
		float cameraZ;
		float cameraAngle;

		bool isFollowing;
		CBObject* followTarget;
		uint8_t followStyle;
		float followSetting;
};

#endif

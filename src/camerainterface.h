#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H
class InterfaceSystem;
#include "precomp.h"

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
	private:
		InterfaceSystem *sys;
		float cameraX;
		float cameraY;
		float cameraZ;
		float cameraAngle;
};

#endif

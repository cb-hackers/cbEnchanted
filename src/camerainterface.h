#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H
class CBEnchanted;
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
		sf::Vector2f screenCoordToWorld(const sf::Vector2f &v);
		sf::Vector2f worldCoordToScreen(const sf::Vector2f &v);
	private:
		CBEnchanted *cb;
		float cameraX;
		float cameraY;
		float cameraAngle;

};

#endif

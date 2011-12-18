#ifndef OBJECTINTERFACE_H
#define OBJECTINTERFACE_H

class CBEnchanted;

class ObjectInterface {
	public:
		ObjectInterface();
		~ObjectInterface();

		void commandDeleteObject(void);
		void commandClearObjects(void);
		void commandMoveObject(void);
		void commandTranslateObject(void);
		void commandPositionObject(void);
		void commandScreenPositionObject(void);
		void commandTurnObject(void);
		void commandRotateObject(void);
		void commandPointObject(void);
		void commandCloneObjectPosition(void);
		void commandCloneObjectOrientation(void);
		void commandObjectOrder(void);
		void commandMaskObject(void);
		void commandShowObject(void);
		void commandDefaultVisible(void);
		void commandPaintObject(void);
		void commandGhostObject(void);
		void commandMirrorObject(void);
		void commandObjectRange(void);
		void commandObjectInteger(void);
		void commandObjectFloat(void);
		void commandObjectString(void);
		void commandObjectPickable(void);
		void commandObjectPick(void);
		void commandPixelPick(void);
		void commandObjectLife(void);
		void commandPlayObject(void);
		void commandLoopObject(void);
		void commandStopObject(void);
		void commandResetObjectCollision(void);
		void commandSetupCollision(void);
		void commandClearCollisions(void);
		void commandInitObjectList(void);
		
		void functionLoadObject(void);
		void functionLoadAnimObject(void);
		void functionMakeObject(void);
		void functionMakeObjectFloor(void);
		void functionCloneObject(void);
		void functionObjectInteger(void);
		void functionObjectFloat(void);
		void functionObjectString(void);
		void functionObjectLife(void);
		void functionPickedObject(void);
		void functionPickedX(void);
		void functionPickedY(void);
		void functionPickedAngle(void);
		void functionGetAngle2(void);
		void functionDistance2(void);
		void functionObjectX(void);
		void functionObjectY(void);
		void functionObjectAngle(void);
		void functionObjectSizeX(void);
		void functionObjectSizeY(void);
		void functionObjectPlaying(void);
		void functionObjectFrame(void);
		void functionObjectsOverlap(void);
		void functionObjectSight(void);
		void functionCountCollisions(void);
		void functionGetCollision(void);
		void functionCollisionX(void);
		void functionCollisionY(void);
		void functionCollisionAngle(void);
		void functionNextObject(void);
	private:
	
};

#endif

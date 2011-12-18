#ifndef OBJECTINTERFACE_H
#define OBJECTINTERFACE_H

class CBEmu;

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
	private:
	
};

#endif

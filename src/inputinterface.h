#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H
#include "precomp.h"

class CBEnchanted;


class InputInterface {
	public:
		InputInterface();
		
		void commandCloseInput(void);
		void commandWaitKey(void);
		void commandClearKeys(void);
		void commandPositionMouse(void);
		void commandWaitMouse(void);
		void commandShowMouse(void);
		void commandClearMouse(void);
		void commandSAFEEXIT(void);
		
		void functionInput(void);
		void functionKeyDown(void);
		void functionKeyHit(void);
		void functionKeyUp(void);
		void functionGetKey(void);
		void functionWaitKey(void);
		void functionMouseDown(void);
		void functionMouseHit(void);
		void functionMouseUp(void);
		void functionGetMouse(void);
		void functionWaitMouse(void);
		void functionMouseX(void);
		void functionMouseY(void);
		void functionMouseWX(void);
		void functionMouseWY(void);
		void functionMouseZ(void);
		void functionMouseMoveX(void);
		void functionMouseMoveY(void);
		void functionMouseMoveZ(void);
		void functionLeftKey(void);
		void functionRightKey(void);
		void functionUpKey(void);
		void functionDownKey(void);
		void functionEscapeKey(void);
		bool initializeInputs(void);

		void updateInputs();
	private:
		CBEnchanted *cb;
		int32_t cbKeyMap[222];
		ALLEGRO_KEYBOARD_STATE *currentKeyboardState;
		ALLEGRO_KEYBOARD_STATE *lastKeyboardState;
		ALLEGRO_MOUSE_STATE *currentMouseState;
		ALLEGRO_MOUSE_STATE *lastMouseState;

		//For MouseMove*
		int32_t lastMouseX,lastMouseY,lastMouseZ;
};

#endif

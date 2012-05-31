#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H
#include "precomp.h"
#include "rendertarget.h"

class CBEnchanted;
class CBInput;

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

		/** Handles KEY_CHAR events that most likely happen during DrawScreen */
		void handleKeyChar(ALLEGRO_EVENT *e);

		/** Returns the current CBInput */
		CBInput* getInput() const { return input; }
		/** Renders the current input */
		void renderInput(RenderTarget &r) const;
	private:
		CBEnchanted *cb;

		/** Holds the mappings from CB SCAN-codes to Allegro equivelants. */
		int32_t cbKeyMap[222];
		ALLEGRO_KEYBOARD_STATE *currentKeyboardState;
		ALLEGRO_KEYBOARD_STATE *lastKeyboardState;
		ALLEGRO_MOUSE_STATE *currentMouseState;
		ALLEGRO_MOUSE_STATE *lastMouseState;
		ALLEGRO_MOUSE_CURSOR* cursor;
		//For MouseMove*
		int32_t lastMouseX,lastMouseY,lastMouseZ;

		/** Holds the current running Input */
		CBInput *input;
};

#endif

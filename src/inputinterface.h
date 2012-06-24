#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H
#include "precomp.h"
#include "rendertarget.h"

#define MAX_MOUSE_BUTTONS 32

class CBEnchanted;
class CBInput;
class CBImage;

class InputInterface {
	public:
		#ifndef CBE_LIB
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
#endif

		/** Updates keystates and mouse buttons. Must been called before event loop. */
		void preEventLoopUpdate();

		/** Updates keystates and mouse buttons. Must been called after event loop. */
		void postEventLoopUpdate();

		/** Handles keyboard events. Returns true if CBEnchanted::stop() is called.*/
		bool handleKeyboardEvent(ALLEGRO_EVENT *e, bool addToQueue = true);

		/** Handles mouse events*/
		void handleMouseEvent(ALLEGRO_EVENT *e, bool addToQueue = true);

		/** Updates keyStates */
		void updateKeyState(ALLEGRO_KEYBOARD_EVENT *e);

		/** Returns the current CBInput */
		CBInput* getInput() const { return input; }

		/** Renders the current input */
		void renderInput(RenderTarget &r) const;

		/** Renders cursor */
		void renderCursor(RenderTarget &r) const;

		void setCustomCursor(CBImage *c) {cursor = c; }

		void setCursorVisible(bool t);
	private:
		CBEnchanted *cb;

		/** Holds the mappings from CB SCAN-codes to Allegro equivelants. */
		int32_t cbKeyMap[222];

		/** Current custom cursor */
		CBImage* cursor;
		/** Is the cursor visible */
		bool cursorVisible;

		bool clearKeyboard;
		bool clearMouse;

		/** Key states, used as bitmask values */
		enum KeyState {
			Up = 0,			//00
			Down = 1,		//01
			Released = 2,	//10
			Pressed = 3		//11
		};

		/** Key states bitmasks */
		uint8_t keyStates[ALLEGRO_KEY_MAX];

		/** Mouse button states bitmasks */
		uint8_t mouseButtonStates[MAX_MOUSE_BUTTONS];

		/** X-coordinate of a mouse */
		int32_t mouseX;
		/** Y-coordinate of a mouse */
		int32_t mouseY;
		/** Z-coordinate of a mouse - mouse wheel */
		int32_t mouseZ;

		/** Last mouse positions for MouseMove* */
		int32_t lastMouseX, lastMouseY, lastMouseZ;

		/** Holds the current running Input */
		CBInput *input;

		/** Event queue */
		list<ALLEGRO_EVENT> eventQueue;
		list<ALLEGRO_EVENT> charEventQueue;
		list<ALLEGRO_EVENT> mouseEventQueue;
};

#endif

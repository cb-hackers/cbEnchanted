#include "precomp.h"
#include "cbenchanted.h"
#include "inputinterface.h"
#include "cbimage.h"
#include "errorsystem.h"

InputInterface::InputInterface():lastMouseX(0),lastMouseY(0),lastMouseZ(0), cursor(NULL){
	cb = static_cast <CBEnchanted *> (this);
	currentKeyboardState = new ALLEGRO_KEYBOARD_STATE;
	lastKeyboardState = new ALLEGRO_KEYBOARD_STATE;
	currentMouseState = new ALLEGRO_MOUSE_STATE;
	lastMouseState = new ALLEGRO_MOUSE_STATE;

	// Init cbKeyMap values to 0
	for (int i = 0; i < 222; ++i) {
		cbKeyMap[i] = 0;
	}

	cbKeyMap[1] = ALLEGRO_KEY_ESCAPE;
	cbKeyMap[2] = ALLEGRO_KEY_1;
	cbKeyMap[3] = ALLEGRO_KEY_2;
	cbKeyMap[4] = ALLEGRO_KEY_3;
	cbKeyMap[5] = ALLEGRO_KEY_4;
	cbKeyMap[6] = ALLEGRO_KEY_5;
	cbKeyMap[7] = ALLEGRO_KEY_6;
	cbKeyMap[8] = ALLEGRO_KEY_7;
	cbKeyMap[9] = ALLEGRO_KEY_8;
	cbKeyMap[10] = ALLEGRO_KEY_9;
	cbKeyMap[11] = ALLEGRO_KEY_0;
	cbKeyMap[12] = ALLEGRO_KEY_EQUALS;
	cbKeyMap[13] = ALLEGRO_KEY_OPENBRACE;
	cbKeyMap[14] = ALLEGRO_KEY_BACKSPACE;
	cbKeyMap[15] = ALLEGRO_KEY_TAB;
	cbKeyMap[16] = ALLEGRO_KEY_Q;
	cbKeyMap[17] = ALLEGRO_KEY_W;
	cbKeyMap[18] = ALLEGRO_KEY_E;
	cbKeyMap[19] = ALLEGRO_KEY_R;
	cbKeyMap[20] = ALLEGRO_KEY_T;
	cbKeyMap[21] = ALLEGRO_KEY_Y;
	cbKeyMap[22] = ALLEGRO_KEY_U;
	cbKeyMap[23] = ALLEGRO_KEY_I;
	cbKeyMap[24] = ALLEGRO_KEY_O;
	cbKeyMap[25] = ALLEGRO_KEY_P;
	cbKeyMap[26] = ALLEGRO_KEY_CLOSEBRACE;
	cbKeyMap[27] = ALLEGRO_KEY_SEMICOLON;
	cbKeyMap[28] = ALLEGRO_KEY_ENTER;
	cbKeyMap[29] = ALLEGRO_KEY_LCTRL;
	cbKeyMap[30] = ALLEGRO_KEY_A;
	cbKeyMap[31] = ALLEGRO_KEY_S;
	cbKeyMap[32] = ALLEGRO_KEY_D;
	cbKeyMap[33] = ALLEGRO_KEY_F;
	cbKeyMap[34] = ALLEGRO_KEY_G;
	cbKeyMap[35] = ALLEGRO_KEY_H;
	cbKeyMap[36] = ALLEGRO_KEY_J;
	cbKeyMap[37] = ALLEGRO_KEY_K;
	cbKeyMap[38] = ALLEGRO_KEY_L;
	cbKeyMap[39] = ALLEGRO_KEY_TILDE;
	cbKeyMap[40] = ALLEGRO_KEY_QUOTE;
	cbKeyMap[41] = ALLEGRO_KEY_BACKSLASH;
	cbKeyMap[42] = ALLEGRO_KEY_LSHIFT;
	cbKeyMap[43] = ALLEGRO_KEY_SLASH;
	cbKeyMap[44] = ALLEGRO_KEY_Z;
	cbKeyMap[45] = ALLEGRO_KEY_X;
	cbKeyMap[46] = ALLEGRO_KEY_C;
	cbKeyMap[47] = ALLEGRO_KEY_V;
	cbKeyMap[48] = ALLEGRO_KEY_B;
	cbKeyMap[49] = ALLEGRO_KEY_N;
	cbKeyMap[50] = ALLEGRO_KEY_M;
	cbKeyMap[51] = ALLEGRO_KEY_COMMA;
	cbKeyMap[52] = ALLEGRO_KEY_FULLSTOP;
	cbKeyMap[53] = ALLEGRO_KEY_MINUS;
	cbKeyMap[54] = ALLEGRO_KEY_RSHIFT;
	cbKeyMap[55] = ALLEGRO_KEY_PAD_ASTERISK;
	cbKeyMap[56] = ALLEGRO_KEY_ALT;
	cbKeyMap[57] = ALLEGRO_KEY_SPACE;
	cbKeyMap[58] = ALLEGRO_KEY_CAPSLOCK;
	cbKeyMap[59] = ALLEGRO_KEY_F1;
	cbKeyMap[60] = ALLEGRO_KEY_F2;
	cbKeyMap[61] = ALLEGRO_KEY_F3;
	cbKeyMap[62] = ALLEGRO_KEY_F4;
	cbKeyMap[63] = ALLEGRO_KEY_F5;
	cbKeyMap[64] = ALLEGRO_KEY_F6;
	cbKeyMap[65] = ALLEGRO_KEY_F7;
	cbKeyMap[66] = ALLEGRO_KEY_F8;
	cbKeyMap[67] = ALLEGRO_KEY_F9;
	cbKeyMap[68] = ALLEGRO_KEY_F10;
	cbKeyMap[69] = ALLEGRO_KEY_PAUSE;
	cbKeyMap[70] = ALLEGRO_KEY_SCROLLLOCK;
	cbKeyMap[71] = ALLEGRO_KEY_PAD_7;
	cbKeyMap[72] = ALLEGRO_KEY_PAD_8;
	cbKeyMap[73] = ALLEGRO_KEY_PAD_9;
	cbKeyMap[74] = ALLEGRO_KEY_PAD_MINUS;
	cbKeyMap[75] = ALLEGRO_KEY_PAD_4;
	cbKeyMap[76] = ALLEGRO_KEY_PAD_5;
	cbKeyMap[77] = ALLEGRO_KEY_PAD_6;
	cbKeyMap[78] = ALLEGRO_KEY_PAD_PLUS;
	cbKeyMap[79] = ALLEGRO_KEY_PAD_1;
	cbKeyMap[80] = ALLEGRO_KEY_PAD_2;
	cbKeyMap[81] = ALLEGRO_KEY_PAD_3;
	cbKeyMap[82] = ALLEGRO_KEY_PAD_0;
	cbKeyMap[83] = ALLEGRO_KEY_PAD_DELETE;
	cbKeyMap[86] = ALLEGRO_KEY_BACKSLASH2;
	cbKeyMap[87] = ALLEGRO_KEY_F11;
	cbKeyMap[88] = ALLEGRO_KEY_F12;
	cbKeyMap[156] = ALLEGRO_KEY_PAD_ENTER;
	cbKeyMap[157] = ALLEGRO_KEY_RCTRL;
	cbKeyMap[181] = ALLEGRO_KEY_PAD_SLASH;
	cbKeyMap[183] = ALLEGRO_KEY_PRINTSCREEN;
	cbKeyMap[184] = ALLEGRO_KEY_ALTGR;
	cbKeyMap[197] = ALLEGRO_KEY_NUMLOCK;
	cbKeyMap[199] = ALLEGRO_KEY_HOME;
	cbKeyMap[200] = ALLEGRO_KEY_UP;
	cbKeyMap[201] = ALLEGRO_KEY_PGUP;
	cbKeyMap[203] = ALLEGRO_KEY_LEFT;
	cbKeyMap[205] = ALLEGRO_KEY_RIGHT;
	cbKeyMap[207] = ALLEGRO_KEY_END;
	cbKeyMap[208] = ALLEGRO_KEY_DOWN;
	cbKeyMap[209] = ALLEGRO_KEY_PGDN;
	cbKeyMap[210] = ALLEGRO_KEY_INSERT;
	cbKeyMap[211] = ALLEGRO_KEY_DELETE;
	cbKeyMap[219] = ALLEGRO_KEY_LWIN;
	cbKeyMap[220] = ALLEGRO_KEY_RWIN;
	cbKeyMap[221] = ALLEGRO_KEY_MENU;
}

void InputInterface::commandCloseInput(void) {
	STUB;
}

/*
 * InputInterface::commandWaitKey - Wait for key press
 */
void InputInterface::commandWaitKey(void) {
	ALLEGRO_EVENT e;
	while(true)
	{
		al_wait_for_event(cb->getEventQueue(),&e);
		switch (e.type)
		{
			case ALLEGRO_EVENT_KEY_DOWN:
			return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				cb->stop();
			return;
		}
	}
}

void InputInterface::commandClearKeys(void) {
	memset(currentKeyboardState->__key_down__internal__,0,((ALLEGRO_KEY_MAX + 31) / 32)*4);
	memset(lastKeyboardState->__key_down__internal__,0,((ALLEGRO_KEY_MAX + 31) / 32)*4);
}

void InputInterface::commandPositionMouse(void) {
	int32_t mouseY = cb->popValue().toInt();
	int32_t mouseX = cb->popValue().toInt();

	al_set_mouse_xy(cb->getWindow(),mouseX,mouseY);
}

void InputInterface::commandWaitMouse(void) {
	ALLEGRO_EVENT e;
	while(true)
	{
		al_wait_for_event(cb->getEventQueue(),&e);
		switch (e.type) {
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				cb->stop();
			return;
		}
	}
}

void InputInterface::commandShowMouse(void) {
	int32_t mouse = cb->popValue().toInt();
	INFO("Doing some stuff with mouse, given parameter is %i", mouse)
	switch (mouse) {
		case 0:
			al_hide_mouse_cursor(cb->getWindow());
		break;
		case 1:
			al_show_mouse_cursor(cb->getWindow());
		break;
		default: {
			if (mouse > 0) {
				if (cursor != NULL) {
					al_destroy_mouse_cursor(cursor);
					cursor = NULL;
				}
				CBImage* img = cb->getImage(mouse);
				if (img == NULL) {
					cb->errors->createError("ShowMouse failed!", "Could not find an image with ID " + boost::lexical_cast<string>(mouse));
					al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					return;
				}

				cursor = al_create_mouse_cursor(img->getMaskedBitmap(), img->width() / 2, img->height()/2);

				if (cursor == NULL) {
					cb->errors->createError("ShowMouse failed!", "Could not make a cursor out of an image with ID " + boost::lexical_cast<string>(mouse));
					al_show_mouse_cursor(cb->getWindow());
					al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
					return;
				}

				al_set_mouse_cursor(cb->getWindow(), cursor);
			}
			else if(mouse < 0) {
				al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			}
		}
		break;
	}

}

void InputInterface::commandClearMouse(void) {
	currentMouseState->buttons = 0;
	lastMouseState->buttons = 0;
}

void InputInterface::commandSAFEEXIT(void) {
	bool toggled = cb->popValue().toBool();
	cb->setSafeExit(toggled);
}

void InputInterface::functionInput(void) {
	cb->pushValue(ISString(""));
	STUB;
}

void InputInterface::functionKeyDown(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(al_key_down(currentKeyboardState,keyC));
}

void InputInterface::functionKeyHit(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(al_key_down(currentKeyboardState,keyC) && !al_key_down(lastKeyboardState,keyC));
}

void InputInterface::functionKeyUp(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(!al_key_down(currentKeyboardState,keyC) && al_key_down(lastKeyboardState,keyC));
}

void InputInterface::functionGetKey(void) {
	cb->pushValue(0);
	STUB;
}

/*
 *  InputInterface::funtionWaitKey - Wait for key press and return pressed key
 */
void InputInterface::functionWaitKey(void) {
	ALLEGRO_EVENT e;
	while(true)
	{
		al_wait_for_event(cb->getEventQueue(),&e);
		switch (e.type) {
			case ALLEGRO_EVENT_KEY_DOWN: {
				int32_t key = e.keyboard.keycode;
				for (int32_t i = 0; i < 222; ++i) {
					if (cbKeyMap[i] == key) {
						cb->pushValue(i);
						return;
					}
				}
				cb->pushValue(0);
				return;
			}
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				cb->stop();
				cb->pushValue(0);
			return;
		}
	}
}

void InputInterface::functionMouseDown(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(al_mouse_button_down(currentMouseState,button));
}

void InputInterface::functionMouseHit(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(al_mouse_button_down(currentMouseState,button) && !al_mouse_button_down(lastMouseState,button));
}

void InputInterface::functionMouseUp(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(!al_mouse_button_down(currentMouseState,button) && al_mouse_button_down(lastMouseState,button));
}

void InputInterface::functionGetMouse(void) {
	STUB;
}

void InputInterface::functionWaitMouse(void) {
	ALLEGRO_EVENT e;
	while(true)
	{
		al_wait_for_event(cb->getEventQueue(),&e);
		switch (e.type) {
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				cb->pushValue((int32_t)e.mouse.button);
			return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				cb->stop();
				cb->pushValue(0);
			return;
		}
	}
}

void InputInterface::functionMouseX(void) {
	cb->pushValue(al_get_mouse_state_axis(currentMouseState,0));
}

void InputInterface::functionMouseY(void) {
	cb->pushValue(al_get_mouse_state_axis(currentMouseState,1));
}

void InputInterface::functionMouseWX(void) {
	cb->pushValue(cb->screenCoordToWorldX(al_get_mouse_state_axis(currentMouseState,0)));
}

void InputInterface::functionMouseWY(void) {
	cb->pushValue(cb->screenCoordToWorldY(al_get_mouse_state_axis(currentMouseState,1)));
}

void InputInterface::functionMouseZ(void) {
	cb->pushValue(al_get_mouse_state_axis(currentMouseState,2));
}

void InputInterface::functionMouseMoveX(void) {
	int32_t a = al_get_mouse_state_axis(currentMouseState,0);
	cb->pushValue(a-lastMouseX);
	lastMouseX = a;
}

void InputInterface::functionMouseMoveY(void) {
	int32_t a = al_get_mouse_state_axis(currentMouseState,1);
	cb->pushValue(a-lastMouseY);
	lastMouseY = a;
}

void InputInterface::functionMouseMoveZ(void) {
	int32_t a = al_get_mouse_state_axis(currentMouseState,2);
	cb->pushValue(a-lastMouseZ);
	lastMouseZ = a;
}

void InputInterface::functionLeftKey(void) {
	cb->pushValue(al_key_down(currentKeyboardState,ALLEGRO_KEY_LEFT));
}

void InputInterface::functionRightKey(void) {
	cb->pushValue(al_key_down(currentKeyboardState,ALLEGRO_KEY_RIGHT));
}

void InputInterface::functionUpKey(void) {
	cb->pushValue(al_key_down(currentKeyboardState,ALLEGRO_KEY_UP));
}

void InputInterface::functionDownKey(void) {
	cb->pushValue(al_key_down(currentKeyboardState,ALLEGRO_KEY_DOWN));
}

void InputInterface::functionEscapeKey(void) {
	cb->pushValue(al_key_down(currentKeyboardState,ALLEGRO_KEY_ESCAPE));
}

bool InputInterface::initializeInputs() {
	if (!al_install_keyboard()) return false;
	if (!al_install_mouse()) return false;
	al_register_event_source(cb->getEventQueue(),al_get_keyboard_event_source());
	al_register_event_source(cb->getEventQueue(),al_get_mouse_event_source());
	updateInputs();
	return true;
}


void InputInterface::updateInputs(){
	ALLEGRO_KEYBOARD_STATE *temp1 = lastKeyboardState;
	lastKeyboardState = currentKeyboardState;
	currentKeyboardState = temp1;
	al_get_keyboard_state(currentKeyboardState);
	ALLEGRO_MOUSE_STATE *temp2 = lastMouseState;
	lastMouseState = currentMouseState;
	currentMouseState = temp2;
	al_get_mouse_state(currentMouseState);
}


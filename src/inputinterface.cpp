#include "precomp.h"
#include "cbenchanted.h"
#include "inputinterface.h"
#include "cbimage.h"
#include "cbinput.h"
#include "errorsystem.h"
#include "utf8.h"
#include "util.h"

InputInterface::InputInterface() :
	lastMouseX(0),
	lastMouseY(0),
	lastMouseZ(0),
	cursor(0),
	input(0),
	clearKeyboard(false),
	clearMouse(false),
	cursorVisible(true)
{
	cb = static_cast <CBEnchanted *> (this);

	memset(keyStates, 0, ALLEGRO_KEY_MAX);

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

	// Init ASCII keymap
	for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
		cbAsciiMap[i] = 0;
	}

	cbAsciiMap[ALLEGRO_KEY_A] = 'a';
	cbAsciiMap[ALLEGRO_KEY_B] = 'b';
	cbAsciiMap[ALLEGRO_KEY_C] = 'c';
	cbAsciiMap[ALLEGRO_KEY_D] = 'd';
	cbAsciiMap[ALLEGRO_KEY_E] = 'e';
	cbAsciiMap[ALLEGRO_KEY_F] = 'f';
	cbAsciiMap[ALLEGRO_KEY_G] = 'g';
	cbAsciiMap[ALLEGRO_KEY_H] = 'h';
	cbAsciiMap[ALLEGRO_KEY_I] = 'i';
	cbAsciiMap[ALLEGRO_KEY_J] = 'j';
	cbAsciiMap[ALLEGRO_KEY_K] = 'k';
	cbAsciiMap[ALLEGRO_KEY_L] = 'l';
	cbAsciiMap[ALLEGRO_KEY_M] = 'm';
	cbAsciiMap[ALLEGRO_KEY_N] = 'n';
	cbAsciiMap[ALLEGRO_KEY_O] = 'o';
	cbAsciiMap[ALLEGRO_KEY_P] = 'p';
	cbAsciiMap[ALLEGRO_KEY_Q] = 'q';
	cbAsciiMap[ALLEGRO_KEY_R] = 'r';
	cbAsciiMap[ALLEGRO_KEY_S] = 's';
	cbAsciiMap[ALLEGRO_KEY_T] = 't';
	cbAsciiMap[ALLEGRO_KEY_U] = 'u';
	cbAsciiMap[ALLEGRO_KEY_V] = 'v';
	cbAsciiMap[ALLEGRO_KEY_W] = 'w';
	cbAsciiMap[ALLEGRO_KEY_X] = 'x';
	cbAsciiMap[ALLEGRO_KEY_Y] = 'y';
	cbAsciiMap[ALLEGRO_KEY_Z] = 'z';

	cbAsciiMap[ALLEGRO_KEY_1] = '1';
	cbAsciiMap[ALLEGRO_KEY_2] = '2';
	cbAsciiMap[ALLEGRO_KEY_3] = '3';
	cbAsciiMap[ALLEGRO_KEY_4] = '4';
	cbAsciiMap[ALLEGRO_KEY_5] = '5';
	cbAsciiMap[ALLEGRO_KEY_6] = '6';
	cbAsciiMap[ALLEGRO_KEY_7] = '7';
	cbAsciiMap[ALLEGRO_KEY_8] = '8';
	cbAsciiMap[ALLEGRO_KEY_9] = '9';
	cbAsciiMap[ALLEGRO_KEY_0] = '0';

	cbAsciiMap[ALLEGRO_KEY_ESCAPE] = 27;
	cbAsciiMap[ALLEGRO_KEY_BACKSPACE] = 8;
	cbAsciiMap[ALLEGRO_KEY_PGDN] = 6;
	cbAsciiMap[ALLEGRO_KEY_PGUP] = 5;
	cbAsciiMap[ALLEGRO_KEY_HOME] = 1;
	cbAsciiMap[ALLEGRO_KEY_END] = 2;
	cbAsciiMap[ALLEGRO_KEY_INSERT] = 3;
	cbAsciiMap[ALLEGRO_KEY_DELETE] = 4;

	cbAsciiMap[ALLEGRO_KEY_OPENBRACE] = 180; // ´
	cbAsciiMap[ALLEGRO_KEY_EQUALS] = '+';
	cbAsciiMap[ALLEGRO_KEY_SEMICOLON] = 168; // ¨
	cbAsciiMap[ALLEGRO_KEY_SLASH] = '\'';
	cbAsciiMap[ALLEGRO_KEY_CLOSEBRACE] = 229; // å
	cbAsciiMap[ALLEGRO_KEY_QUOTE] = 228; // ä
	cbAsciiMap[ALLEGRO_KEY_TILDE] = 246; // ö
	cbAsciiMap[ALLEGRO_KEY_MINUS] = '-';
	cbAsciiMap[ALLEGRO_KEY_FULLSTOP] = '.';
	cbAsciiMap[ALLEGRO_KEY_COMMA] = ',';
	cbAsciiMap[ALLEGRO_KEY_BACKSLASH2] = '<';
	cbAsciiMap[ALLEGRO_KEY_BACKSLASH] = 167; // §
	cbAsciiMap[ALLEGRO_KEY_PAD_SLASH] = '-';
	cbAsciiMap[ALLEGRO_KEY_PAD_ASTERISK] = '*';
	cbAsciiMap[ALLEGRO_KEY_PAD_MINUS] = '-';
	cbAsciiMap[ALLEGRO_KEY_PAD_PLUS] = '+';
	cbAsciiMap[ALLEGRO_KEY_PAD_ENTER] = '\r';
	cbAsciiMap[ALLEGRO_KEY_ENTER] = '\r';

	// Init shift array to same values as without and altgr array to empty
	for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
		cbAsciiMapShift[i] = cbAsciiMap[i];
		cbAsciiMapAltGr[i] = 0;
	}

	// ------------------------------------------------------------------
	// Shift map
	// ------------------------------------------------------------------
	cbAsciiMapShift[ALLEGRO_KEY_A] = 'A';
	cbAsciiMapShift[ALLEGRO_KEY_B] = 'B';
	cbAsciiMapShift[ALLEGRO_KEY_C] = 'C';
	cbAsciiMapShift[ALLEGRO_KEY_D] = 'D';
	cbAsciiMapShift[ALLEGRO_KEY_E] = 'E';
	cbAsciiMapShift[ALLEGRO_KEY_F] = 'F';
	cbAsciiMapShift[ALLEGRO_KEY_G] = 'G';
	cbAsciiMapShift[ALLEGRO_KEY_H] = 'H';
	cbAsciiMapShift[ALLEGRO_KEY_I] = 'I';
	cbAsciiMapShift[ALLEGRO_KEY_J] = 'J';
	cbAsciiMapShift[ALLEGRO_KEY_K] = 'K';
	cbAsciiMapShift[ALLEGRO_KEY_L] = 'L';
	cbAsciiMapShift[ALLEGRO_KEY_M] = 'M';
	cbAsciiMapShift[ALLEGRO_KEY_N] = 'N';
	cbAsciiMapShift[ALLEGRO_KEY_O] = 'O';
	cbAsciiMapShift[ALLEGRO_KEY_P] = 'P';
	cbAsciiMapShift[ALLEGRO_KEY_Q] = 'Q';
	cbAsciiMapShift[ALLEGRO_KEY_R] = 'R';
	cbAsciiMapShift[ALLEGRO_KEY_S] = 'S';
	cbAsciiMapShift[ALLEGRO_KEY_T] = 'T';
	cbAsciiMapShift[ALLEGRO_KEY_U] = 'U';
	cbAsciiMapShift[ALLEGRO_KEY_V] = 'V';
	cbAsciiMapShift[ALLEGRO_KEY_W] = 'W';
	cbAsciiMapShift[ALLEGRO_KEY_X] = 'X';
	cbAsciiMapShift[ALLEGRO_KEY_Y] = 'Y';
	cbAsciiMapShift[ALLEGRO_KEY_Z] = 'Z';

	cbAsciiMapShift[ALLEGRO_KEY_1] = '!';
	cbAsciiMapShift[ALLEGRO_KEY_2] = '"';
	cbAsciiMapShift[ALLEGRO_KEY_3] = '#';
	cbAsciiMapShift[ALLEGRO_KEY_4] = 164; // ¤
	cbAsciiMapShift[ALLEGRO_KEY_5] = '%';
	cbAsciiMapShift[ALLEGRO_KEY_6] = '&';
	cbAsciiMapShift[ALLEGRO_KEY_7] = '/';
	cbAsciiMapShift[ALLEGRO_KEY_8] = '(';
	cbAsciiMapShift[ALLEGRO_KEY_9] = ')';
	cbAsciiMapShift[ALLEGRO_KEY_0] = '=';

	cbAsciiMapShift[ALLEGRO_KEY_EQUALS] = '?';
	cbAsciiMapShift[ALLEGRO_KEY_OPENBRACE] = '`';
	cbAsciiMapShift[ALLEGRO_KEY_BACKSLASH] = 189; // ½
	cbAsciiMapShift[ALLEGRO_KEY_BACKSLASH2] = '>';
	cbAsciiMapShift[ALLEGRO_KEY_COMMA] = ';';
	cbAsciiMapShift[ALLEGRO_KEY_FULLSTOP] = ':';
	cbAsciiMapShift[ALLEGRO_KEY_MINUS] = '_';
	cbAsciiMapShift[ALLEGRO_KEY_SLASH] = '*';
	cbAsciiMapShift[ALLEGRO_KEY_CLOSEBRACE] = 197; // Å
	cbAsciiMapShift[ALLEGRO_KEY_QUOTE] = 196; // Ä
	cbAsciiMapShift[ALLEGRO_KEY_TILDE] = 214; // Ö
	cbAsciiMapShift[ALLEGRO_KEY_SEMICOLON] = '^';
	cbAsciiMapShift[ALLEGRO_KEY_PAD_SLASH] = '_';

	// ------------------------------------------------------------------
	// Alt Gr map
	// ------------------------------------------------------------------
	cbAsciiMapAltGr[ALLEGRO_KEY_E] = 128; // €
	cbAsciiMapAltGr[ALLEGRO_KEY_SEMICOLON] = '~';
	cbAsciiMapAltGr[ALLEGRO_KEY_BACKSLASH2] = '|';
	cbAsciiMapAltGr[ALLEGRO_KEY_M] = 181; // µ
	cbAsciiMapAltGr[ALLEGRO_KEY_2] = '@';
	cbAsciiMapAltGr[ALLEGRO_KEY_3] = 163; // £
	cbAsciiMapAltGr[ALLEGRO_KEY_4] = '$';
	cbAsciiMapAltGr[ALLEGRO_KEY_5] = 128; // €
	cbAsciiMapAltGr[ALLEGRO_KEY_7] = '{';
	cbAsciiMapAltGr[ALLEGRO_KEY_8] = '[';
	cbAsciiMapAltGr[ALLEGRO_KEY_9] = ']';
	cbAsciiMapAltGr[ALLEGRO_KEY_0] = '}';
	cbAsciiMapAltGr[ALLEGRO_KEY_EQUALS] = '\\';

	cbAsciiMapAltGr[ALLEGRO_KEY_PGDN] = 6;
	cbAsciiMapAltGr[ALLEGRO_KEY_PGUP] = 5;
	cbAsciiMapAltGr[ALLEGRO_KEY_HOME] = 1;
	cbAsciiMapAltGr[ALLEGRO_KEY_END] = 2;
	cbAsciiMapAltGr[ALLEGRO_KEY_INSERT] = 3;
	cbAsciiMapAltGr[ALLEGRO_KEY_DELETE] = 4;
}

void InputInterface::commandCloseInput(void) {
	if (input) {
		delete input;
		input = 0;
	}
}

/*
 * InputInterface::commandWaitKey - Wait for key press
 */
void InputInterface::commandWaitKey(void) {
	ALLEGRO_EVENT e;
	while (true) {
		al_wait_for_event(cb->getEventQueue(),&e);
		switch (e.type) {
			case ALLEGRO_EVENT_KEY_DOWN:
				updateKeyState(&e.keyboard);
				return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
				break;
			default:
				handleMouseEvent(&e);
				if (handleKeyboardEvent(&e)) {
					return;
				}
		}
	}
}

void InputInterface::commandClearKeys(void) {
	memset(keyStates, 0, ALLEGRO_KEY_MAX);
	clearKeyboard = true;
}

void InputInterface::commandPositionMouse(void) {
	int32_t mY = cb->popValue().toInt();
	int32_t mX = cb->popValue().toInt();

	al_set_mouse_xy(cb->getWindow(),mX,mY);
	mouseX = mX;
	mouseY = mY;
}

void InputInterface::commandWaitMouse(void) {
	ALLEGRO_EVENT e;
	while (true) {
		al_wait_for_event(cb->getEventQueue(),&e);
		handleMouseEvent(&e);
		if (handleKeyboardEvent(&e)) {
			cb->stop();
			return;
		}
		switch (e.type) {
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
				break;
		}
	}
}

void InputInterface::commandShowMouse(void) {
	int32_t id = cb->popValue().getInt();
	if (id < 0) {
		cb->errors->createError("ShowMouse failed!", "Parameter must be positive.");
		return;
	}

	if (id == 0) { // OFF
		if (cursorVisible) {
			al_hide_mouse_cursor(cb->getWindow());
			cursorVisible = false;
		}
		cursor = 0;
	}
	else if (id == 1) { // ON
		if (cursor != 0) {
			cursor = 0;
			al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
		}
		if (!cursorVisible) {
			al_show_mouse_cursor(cb->getWindow());
			cursorVisible = true;
		}
	}
	else { // Image
		if (cursorVisible) {
			al_hide_mouse_cursor(cb->getWindow());
			cursorVisible = false;
		}

		CBImage* img = cb->getImage(id);
		if (img == 0) {
			cb->errors->createError("ShowMouse failed!", "Could not find an image with ID " + boost::lexical_cast<string>(id));
			return;
		}
		cursor = img;
	}
}

void InputInterface::commandClearMouse(void) {
	memset(mouseButtonStates, 0, MAX_MOUSE_BUTTONS);
	clearMouse = true;
}

void InputInterface::commandSAFEEXIT(void) {
	bool toggled = cb->popValue().toBool();
	cb->setSafeExit(toggled);
}

void InputInterface::functionInput(void) {
	if (input == 0) {
		// Make a new input
		string pwString = cb->popValue().getString().getRef();
		string prompt = cb->popValue().getString().getRef();
		input = new CBInput(prompt, pwString);
		input->setLocation(cb->getLocationX(), cb->getLocationY());
	}
	else {
		// Just pop out the unnecessary parameters.
		cb->popValue();
		cb->popValue();
	}
	input->setColor(cb->getDrawColor());
	cb->pushValue(input->getContent());
}

void InputInterface::functionKeyDown(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(keyStates[keyC] & Down);
}

void InputInterface::functionKeyHit(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(keyStates[keyC] == Pressed);
}

void InputInterface::functionKeyUp(void) {
	int32_t keyC = cbKeyMap[cb->popValue().toInt()];
	cb->pushValue(keyStates[keyC] == Released);
}

void InputInterface::functionGetKey(void) {
	for (list<ALLEGRO_EVENT>::iterator i = charEventQueue.begin(); i != charEventQueue.begin(); i++) {
		if (!i->keyboard.repeat) {
			string utf8str;
			vector<unsigned int> utf32char(1);
			utf32char[0] = i->keyboard.unichar;
			try {
				utf8::utf32to8(utf32char.begin(), utf32char.end(), back_inserter(utf8str));
				cb->pushValue((int32_t)*(uint8_t*)utf8toCP1252(utf8str).c_str());
				charEventQueue.erase(i);
				return;
			}
			catch(utf8::invalid_code_point &) {
				// The char point contained malformed UTF-8
			}
		}
	}
	ALLEGRO_EVENT e;
	while (al_get_next_event(cb->getEventQueue(), &e)) {
		eventQueue.push_back(e);
		switch (e.type) {
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				mouseEventQueue.push_back(e);
				break;
			case ALLEGRO_EVENT_KEY_CHAR: {
				if (!e.keyboard.repeat) {
					string utf8str;
					vector<unsigned int> utf32char(1);
					utf32char[0] = e.keyboard.unichar;
					try {
						utf8::utf32to8(utf32char.begin(), utf32char.end(), back_inserter(utf8str));
						cb->pushValue((int32_t)*(uint8_t*)utf8toCP1252(utf8str).c_str());
						return;
					}
					catch(utf8::invalid_code_point &) {
						// The char point contained malformed UTF-8
					}
				}
				break;
			}
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
			break;
		}
	}
	cb->pushValue(0);
}

/*
 *  InputInterface::funtionWaitKey - Wait for key press and return pressed key
 */
void InputInterface::functionWaitKey(void) {
	ALLEGRO_EVENT e;
	while(true) {
		al_wait_for_event(cb->getEventQueue(),&e);
		if (handleKeyboardEvent(&e)) {
			return;
		}
		handleMouseEvent(&e);
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
				if (cb->askForExit()) {
					cb->stop();
					cb->pushValue(0);
					return;
				}
			break;
		}
	}
}

void InputInterface::functionMouseDown(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(mouseButtonStates[button - 1] & Down);
}

void InputInterface::functionMouseHit(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(mouseButtonStates[button - 1] == Pressed);
}

void InputInterface::functionMouseUp(void) {
	int32_t button = cb->popValue().toInt();
	cb->pushValue(mouseButtonStates[button - 1] == Released);
}

void InputInterface::functionGetMouse(void) {
	for (list<ALLEGRO_EVENT>::iterator i = mouseEventQueue.begin(); i != mouseEventQueue.end(); i++) {
		cb->pushValue((int32_t)i->mouse.button);
		mouseEventQueue.erase(i);
		return;
	}
	ALLEGRO_EVENT e;
	while (al_get_next_event(cb->getEventQueue(), &e)) {
		eventQueue.push_back(e);
		switch (e.type) {
			case ALLEGRO_EVENT_KEY_CHAR:
				charEventQueue.push_back(e);
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				cb->pushValue((int32_t)e.mouse.button);
				return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
			break;
		}
	}
	cb->pushValue(0);
}

void InputInterface::functionWaitMouse(void) {
	ALLEGRO_EVENT e;
	while (true) {
		al_wait_for_event(cb->getEventQueue(),&e);
		handleMouseEvent(&e);
		if (handleKeyboardEvent(&e)) {
			return;
		}
		switch (e.type) {
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				cb->pushValue((int32_t)e.mouse.button);
				return;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
			break;
		}
	}
}

void InputInterface::functionMouseX(void) {
	cb->pushValue(mouseX);
}

void InputInterface::functionMouseY(void) {
	cb->pushValue(mouseY);
}

void InputInterface::functionMouseWX(void) {
	cb->pushValue(cb->screenCoordToWorldX(mouseX));
}

void InputInterface::functionMouseWY(void) {
	cb->pushValue(cb->screenCoordToWorldY(mouseY));
}

void InputInterface::functionMouseZ(void) {
	cb->pushValue(mouseZ);
}

void InputInterface::functionMouseMoveX(void) {
	cb->pushValue(mouseX - lastMouseX);
	lastMouseX = mouseX;
}

void InputInterface::functionMouseMoveY(void) {
	cb->pushValue(mouseY - lastMouseY);
	lastMouseY = mouseY;
}

void InputInterface::functionMouseMoveZ(void) {
	cb->pushValue(mouseZ - lastMouseZ);
	lastMouseZ = mouseZ;
}

void InputInterface::functionLeftKey(void) {
	cb->pushValue(keyStates[ALLEGRO_KEY_LEFT] & Down);
}

void InputInterface::functionRightKey(void) {
	cb->pushValue(keyStates[ALLEGRO_KEY_RIGHT] & Down);
}

void InputInterface::functionUpKey(void) {
	cb->pushValue(keyStates[ALLEGRO_KEY_UP] & Down);
}

void InputInterface::functionDownKey(void) {
	cb->pushValue(keyStates[ALLEGRO_KEY_DOWN] & Down);
}

void InputInterface::functionEscapeKey(void) {
	cb->pushValue(keyStates[ALLEGRO_KEY_ESCAPE] & Down);
}

bool InputInterface::initializeInputs() {
	if (!al_install_keyboard()) return false;
	if (!al_install_mouse()) return false;
	al_register_event_source(cb->getEventQueue(),al_get_keyboard_event_source());
	al_register_event_source(cb->getEventQueue(),al_get_mouse_event_source());
	return true;
}


void InputInterface::preEventLoopUpdate() {
	for (uint32_t i = 0; i < ALLEGRO_KEY_MAX; i++) {
		//Clear second bit
		//Pressed -> Down
		//Released -> Up
		keyStates[i] &= 0x01;
	}

	for (uint32_t i = 0; i < MAX_MOUSE_BUTTONS; i++) {
		//Clear second bit
		//Pressed -> Down
		//Released -> Up
		mouseButtonStates[i] &= 0x01;
	}
	bool windowResized = false;
	for (list<ALLEGRO_EVENT>::iterator i = eventQueue.begin(); i != eventQueue.end(); i++) {
		switch (i->type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
					return;
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				windowResized = true;
				break;
			default:
				if (handleKeyboardEvent(&*i)) {
					return;
				}
				handleMouseEvent(&*i);
		}
	}
	eventQueue.clear();
	mouseEventQueue.clear();
	charEventQueue.clear();
	if (windowResized) al_acknowledge_resize(cb->getWindow());
}

bool InputInterface::handleKeyboardEvent(ALLEGRO_EVENT *e, bool addToQueue) {
	if (clearKeyboard) {
		return false;
	}
	if (e->type == ALLEGRO_EVENT_KEY_CHAR) {
		if (addToQueue) {
			charEventQueue.push_back(*e);
		}
		if (input && e->keyboard.keycode != ALLEGRO_KEY_ESCAPE) {
			input->keyChar(e);
			return false;
		}
	}
	if (e->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (e->keyboard.keycode == ALLEGRO_KEY_ESCAPE && cb->isSafeExit()) {
			cb->stop();
			return true;
		}
		updateKeyState(&e->keyboard);
	}
	if (e->type == ALLEGRO_EVENT_KEY_UP) {
		updateKeyState(&e->keyboard);
	}
	return false;
}

void InputInterface::handleMouseEvent(ALLEGRO_EVENT *e, bool addToQueue) {
	if (clearMouse) return;
	switch (e->type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if (addToQueue) {
				mouseEventQueue.push_back(*e);
			}
			mouseButtonStates[e->mouse.button - 1] = Pressed;
			return;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouseButtonStates[e->mouse.button - 1] = Released;
			return;
		case ALLEGRO_EVENT_MOUSE_AXES:
			mouseX = e->mouse.x;
			mouseY = e->mouse.y;
			mouseZ = e->mouse.z;
			return;
	}
}

void InputInterface::updateKeyState(ALLEGRO_KEYBOARD_EVENT *e) {
	if (e->type == ALLEGRO_EVENT_KEY_DOWN) {
		keyStates[e->keycode] = Pressed;
	}
	if (e->type == ALLEGRO_EVENT_KEY_UP) {
		keyStates[e->keycode] = Released;
	}
}

/** Renders the current input */
void InputInterface::renderInput(RenderTarget &r) const {
	if (input == 0) {
		return;
	}
	string text = input->getDisplayString();
	if (text.empty()) {
		return;
	}

	ISString str = ISString(text);

	r.setAsCurrent();
	r.useWorldCoords(false);
	r.drawText(cb->getCurrentFont(), str, input->getLocationX(), input->getLocationY(), input->getColor());
}

void InputInterface::postEventLoopUpdate() {
	clearMouse = false;
	clearKeyboard = false;
	const double deleteTimeout = 5.0;
	double currentTime = al_get_time();
	for (list<ALLEGRO_EVENT>::iterator i = charEventQueue.begin(); i != charEventQueue.end();) {
		if ((currentTime - i->any.timestamp) > deleteTimeout) {
			i = charEventQueue.erase(i);
		}
		else {
			i++;
		}
	}
	for (list<ALLEGRO_EVENT>::iterator i = mouseEventQueue.begin(); i != mouseEventQueue.end();) {
		if ((currentTime - i->any.timestamp) > deleteTimeout) {
			i = mouseEventQueue.erase(i);
		}
		else {
			i++;
		}
	}
}

/** Renders cursor */
void InputInterface::renderCursor(RenderTarget &r) const {
	if (cursor != 0) {
		r.useWorldCoords(false);
		r.drawBitmap(
			cursor->getRenderTarget()->getBitmap(),
			mouseX - cursor->width() * 0.5f,
			mouseY - cursor->height() * 0.5f
		);
	}
}

void InputInterface::setCursorVisible(bool t) {
	cursorVisible = t;
	if (t) {
		al_show_mouse_cursor(cb->getWindow());
	}
	else {
		al_hide_mouse_cursor(cb->getWindow());
	}
}

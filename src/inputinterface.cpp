#include "precomp.h"
#include "cbenchanted.h"
#include "inputinterface.h"

InputInterface::InputInterface() {
	cb = static_cast <CBEnchanted *> (this);
	key[1] = sf::Keyboard::Escape;
	key[2] = sf::Keyboard::Num1;
	key[3] = sf::Keyboard::Num2;
	key[4] = sf::Keyboard::Num4;
	key[5] = sf::Keyboard::Num5;
	key[6] = sf::Keyboard::Num6;
	key[7] = sf::Keyboard::Num7;
	key[8] = sf::Keyboard::Num8;
	key[10] = sf::Keyboard::Num9;
	key[11] = sf::Keyboard::Num0;
	key[12] = sf::Keyboard::Subtract;
	key[13] = sf::Keyboard::Insert;
	key[14] = sf::Keyboard::Back;
	key[15] = sf::Keyboard::Tab;
	key[16] = sf::Keyboard::Q;
	key[17] = sf::Keyboard::W;
	key[18] = sf::Keyboard::E;
	key[19] = sf::Keyboard::R;
	key[20] = sf::Keyboard::T;
	key[21] = sf::Keyboard::Y;
	key[22] = sf::Keyboard::U;
	key[23] = sf::Keyboard::I;
	key[24] = sf::Keyboard::O;
	key[25] = sf::Keyboard::P;
	key[26] = sf::Keyboard::LBracket;
	key[27] = sf::Keyboard::RBracket;
	key[28] = sf::Keyboard::Return;
	key[29] = sf::Keyboard::LAlt;
	key[30] = sf::Keyboard::A;
	key[31] = sf::Keyboard::S;
	key[32] = sf::Keyboard::D;
	key[33] = sf::Keyboard::F;
	key[34] = sf::Keyboard::G;
	key[35] = sf::Keyboard::H;
	key[36] = sf::Keyboard::J;
	key[37] = sf::Keyboard::K;
	key[38] = sf::Keyboard::L;
	key[39] = sf::Keyboard::SemiColon;
	key[40] = sf::Keyboard::Quote;
	key[41] = sf::Keyboard::Tilde;
	key[42] = sf::Keyboard::LShift;
	key[43] = sf::Keyboard::BackSlash;
	key[44] = sf::Keyboard::Z;
	key[45] = sf::Keyboard::X;
	key[46] = sf::Keyboard::C;
	key[47] = sf::Keyboard::V;
	key[48] = sf::Keyboard::B;
	key[49] = sf::Keyboard::N;
	key[50] = sf::Keyboard::M;
	key[51] = sf::Keyboard::Comma;
	key[52] = sf::Keyboard::Period;
	key[53] = sf::Keyboard::Slash;
	key[54] = sf::Keyboard::RShift;
	key[55] = sf::Keyboard::Multiply;
	key[56] = sf::Keyboard::LAlt;
	key[57] = sf::Keyboard::Space;
	//key[58] = sf::Keyboard::; <= Caps Lock
	key[59] = sf::Keyboard::F1;
	key[60] = sf::Keyboard::F2;
	key[61] = sf::Keyboard::F3;
	key[62] = sf::Keyboard::F4;
	key[63] = sf::Keyboard::F5;
	key[64] = sf::Keyboard::F6;
	key[65] = sf::Keyboard::F7;
	key[66] = sf::Keyboard::F8;
	key[67] = sf::Keyboard::F9;
	key[68] = sf::Keyboard::F10;
	//key[69] = sf::Keyboard::; <= Num Lock
	//key[70] = sf::Keyboard::; <= Scroll Lock
	key[71] = sf::Keyboard::Numpad7;
	key[72] = sf::Keyboard::Numpad8;
	key[73] = sf::Keyboard::Numpad9;
	key[74] = sf::Keyboard::Subtract;
	key[75] = sf::Keyboard::Numpad4;
	key[76] = sf::Keyboard::Numpad5;
	key[77] = sf::Keyboard::Numpad6;
	key[78] = sf::Keyboard::Add;
	key[79] = sf::Keyboard::Numpad1;
	key[80] = sf::Keyboard::Numpad2;
	key[81] = sf::Keyboard::Numpad3;
	key[82] = sf::Keyboard::Numpad0;
	//key[83] = sf::Keyboard::;
	//key[86] = sf::Keyboard::;
	key[87] = sf::Keyboard::F11;
	key[88] = sf::Keyboard::F12;
	//key[156] = sf::Keyboard::;
	key[157] = sf::Keyboard::LControl;
	key[181] = sf::Keyboard::Divide;
	//key[182] = sf::Keyboard::;
	key[182] = sf::Keyboard::RAlt;
	key[197] = sf::Keyboard::Pause;
	key[199] = sf::Keyboard::Home;
	key[200] = sf::Keyboard::Up;
	key[201] = sf::Keyboard::PageUp;
	key[203] = sf::Keyboard::Left;
	key[205] = sf::Keyboard::Right;
	key[207] = sf::Keyboard::End;
	key[208] = sf::Keyboard::Down;
	key[209] = sf::Keyboard::PageDown;
	key[210] = sf::Keyboard::Insert;
	key[211] = sf::Keyboard::Delete;
	key[219] = sf::Keyboard::LSystem;
	key[220] = sf::Keyboard::RSystem;
	key[221] = sf::Keyboard::Menu;

 }

void InputInterface::commandCloseInput(void) {
	STUB;
}

/*
 * InputInterface::commandWaitKey - Wait for key press
 */
void InputInterface::commandWaitKey(void) {
	sf::Event e;
	while(true) {
		cb->getWindow()->WaitEvent(e);
		switch (e.Type) {
			case sf::Event::KeyPressed:
				return;
			case sf::Event::Closed:
				cb->stop();
				return;
		}
	}
}

void InputInterface::commandClearKeys(void) {
	STUB;
}

void InputInterface::commandPositionMouse(void) {
	int32_t mouseY = cb->popValue().toInt();
	int32_t mouseX = cb->popValue().toInt();

	sf::Mouse::SetPosition(sf::Vector2i(mouseX, mouseY), *cb->getWindow());
}

void InputInterface::commandWaitMouse(void) {
	STUB;
}

void InputInterface::commandShowMouse(void) {
	STUB;
}

void InputInterface::commandClearMouse(void) {
	STUB;
}

void InputInterface::commandSAFEEXIT(void) {
	STUB;
}

void InputInterface::functionInput(void) {
	STUB;
}

void InputInterface::functionKeyDown(void) {
	int32_t keyC = cb->popValue().toInt();
	cb->pushValue(keyState[keyC]);
}

void InputInterface::functionKeyHit(void) {
	/*
	int32_t keyC = cb->popValue().toInt();
	if(keyState[keyC] != 0)
		return;

	if(keyState[keyC] = 0)
		cb->pushValue(1);*/
}

void InputInterface::functionKeyUp(void) {
	/*
	int32_t keyC = cb->popValue().toInt();
	if(keyState[keyC] != 1)
		return;

	if(keyState[keyC] = 1)
		cb->pushValue(1);*/
}

void InputInterface::functionGetKey(void) {
	STUB;
}

/*
 *  InputInterface::funtionWaitKey - Wait for key press and return pressed key
 */
void InputInterface::functionWaitKey(void) {
	sf::Event e;
	while(true)
	{
		cb->getWindow()->WaitEvent(e);
		switch (e.Type)
		{
		case sf::Event::KeyPressed:
			cb->pushValue((int32_t)e.Key.Code);
			return;
		case sf::Event::Closed:
			cb->stop();
			return;
		}
	}
}

void InputInterface::functionMouseDown(void) {
	STUB;
}

void InputInterface::functionMouseHit(void) {
	STUB;
}

void InputInterface::functionMouseUp(void) {
	STUB;
}

void InputInterface::functionGetMouse(void) {
	STUB;
}

void InputInterface::functionWaitMouse(void) {
	STUB;
}

void InputInterface::functionMouseX(void) {
	sf::Vector2i pos = sf::Mouse::GetPosition(*cb->getWindow());
	cb->pushValue(pos.x);
}

void InputInterface::functionMouseY(void) {
	sf::Vector2i pos = sf::Mouse::GetPosition(*cb->getWindow());
	cb->pushValue(pos.y);
}

void InputInterface::functionMouseWX(void) {
	sf::Vector2i pos = sf::Mouse::GetPosition(*cb->getWindow());
}

void InputInterface::functionMouseWY(void) {
	sf::Vector2i pos = sf::Mouse::GetPosition(*cb->getWindow());
}

void InputInterface::functionMouseZ(void) {
	STUB;
}

void InputInterface::functionMouseMoveX(void) {
	STUB;
}

void InputInterface::functionMouseMoveY(void) {
	STUB;
}

void InputInterface::functionMouseMoveZ(void) {
	STUB;
}

void InputInterface::functionLeftKey(void) {
	int32_t press = int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Left));
	cb->pushValue(press);
}

void InputInterface::functionRightKey(void) {
	int32_t press = int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Right));
	cb->pushValue(press);
}

void InputInterface::functionUpKey(void) {
	int32_t press = int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Up));
	cb->pushValue(press);
}

void InputInterface::functionDownKey(void) {
	int32_t press = int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Down));
	cb->pushValue(press);
}

void InputInterface::functionEscapeKey(void) {
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Escape)));
}

<<<<<<< HEAD

void InputInterface::setKeyState(uint8_t inptKey){
	keyState[inptKey] = sf::Keyboard::IsKeyPressed(key[inptKey]);
}
=======
void InputInterface::setKeyState(uint8_t inptKey) {}
>>>>>>> 0c8925ed49a96b277bcbeca5bad556ae29c8d9ef

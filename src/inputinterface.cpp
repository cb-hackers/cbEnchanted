#include "precomp.h"
#include "cbenchanted.h"
#include "inputinterface.h"

InputInterface::InputInterface() {
	cb = static_cast <CBEnchanted *> (this);
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
	STUB;
}

void InputInterface::functionKeyHit(void) {
	STUB;
}

void InputInterface::functionKeyUp(void) {
	STUB;
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
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Left)));
}

void InputInterface::functionRightKey(void) {
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Right)));
}

void InputInterface::functionUpKey(void) {
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Up)));
}

void InputInterface::functionDownKey(void) {
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Down)));
}

void InputInterface::functionEscapeKey(void) {
	cb->pushValue(int(sf::Keyboard::IsKeyPressed(sf::Keyboard::Escape)));
}

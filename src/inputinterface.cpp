#include "precomp.h"
#include "cbenchanted.h"
#include "inputinterface.h"

InputInterface::InputInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

void InputInterface::commandCloseInput(void) {
	
}

/*
 * InputInterface::commandWaitKey - Wait for key press
 */
void InputInterface::commandWaitKey(void) {
	sf::Event e;
	bool stayIn = true;
	while(stayIn) {
		cb->getWindow()->WaitEvent(e);
		switch (e.Type) {
			case sf::Event::KeyPressed:
				stayIn = false;
				break;
			case sf::Event::Closed:
				cb->stop();
				stayIn = false;
				break;
		}
	}
}

void InputInterface::commandClearKeys(void) {
	
}

void InputInterface::commandPositionMouse(void) {
	int32_t mouseY = cb->popValue().toInt();
	int32_t mouseX = cb->popValue().toInt();

	sf::Mouse::SetPosition(sf::Vector2i(mouseX, mouseY), *cb->getWindow());
}

void InputInterface::commandWaitMouse(void) {
	
}

void InputInterface::commandShowMouse(void) {
	
}

void InputInterface::commandClearMouse(void) {
	
}

void InputInterface::commandSAFEEXIT(void) {
	
}

void InputInterface::functionInput(void) {
	
}

void InputInterface::functionKeyDown(void) {
	
}

void InputInterface::functionKeyHit(void) {
	
}

void InputInterface::functionKeyUp(void) {
	
}

void InputInterface::functionGetKey(void) {
	
}

/*
 *  InputInterface::funtionWaitKey - Wait for key press and return pressed key
 */
void InputInterface::functionWaitKey(void) {
	sf::Event e;
	bool stayIn = true;
	while(stayIn)
	{
		cb->getWindow()->WaitEvent(e);
		switch (e.Type)
		{
		case sf::Event::KeyPressed:
			stayIn = false;
			cb->pushValue((int32_t)e.Key.Code);
			break;
		case sf::Event::Closed:
			cb->stop();
			stayIn = false;
			break;
		}
	}
}

void InputInterface::functionMouseDown(void) {
	
}

void InputInterface::functionMouseHit(void) {
	
}

void InputInterface::functionMouseUp(void) {
	
}

void InputInterface::functionGetMouse(void) {
	
}

void InputInterface::functionWaitMouse(void) {
	
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
	
}

void InputInterface::functionMouseWY(void) {
	
}

void InputInterface::functionMouseZ(void) {
	
}

void InputInterface::functionMouseMoveX(void) {
	
}

void InputInterface::functionMouseMoveY(void) {
	
}

void InputInterface::functionMouseMoveZ(void) {
	
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

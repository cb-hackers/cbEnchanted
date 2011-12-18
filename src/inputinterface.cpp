#include "precomp.h"
#include "cbemu.h"
#include "inputinterface.h"

InputInterface::InputInterface() {
	cb = static_cast <CBEmu *> (this);
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
				cb->getWindow()->Close();
				stayIn = false;
				break;
		}
	}
}

void InputInterface::commandClearKeys(void) {
	
}

void InputInterface::commandPositionMouse(void) {
	
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
			cb->getWindow()->Close();
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
	
}

void InputInterface::functionMouseY(void) {
	
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
	
}

void InputInterface::functionRightKey(void) {
	
}

void InputInterface::functionUpKey(void) {
	
}

void InputInterface::functionDownKey(void) {
	
}

void InputInterface::functionEscapeKey(void) {
	
}

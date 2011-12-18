#include "precomp.h"
#include "cbemu.h"
#include "inputinterface.h"

InputInterface::InputInterface() {
	cb = static_cast <CBEmu *> (this);
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


#include "precomp.h"
#include "cbemu.h"
#include "inputinterface.h"

InputInterface::InputInterface() : keyWait(1) {
	cb = static_cast <CBEmu *> (this);
}

void InputInterface::commandWaitKey(void) {
	while (keyWait) {
		CL_KeepAlive::process(0);
		CL_System::sleep(10);
	}

	keyWait = 0;
}

void InputInterface::functionWaitKey(void) {
	while (!keyWait) {
		CL_KeepAlive::process();
		CL_System::sleep(10);
	}
	cb->pushValue(keyWait);

	keyWait = 0;
}

void InputInterface::onKeyDown(const CL_InputEvent &key, const CL_InputState &state) {
	keyWait = false;
}

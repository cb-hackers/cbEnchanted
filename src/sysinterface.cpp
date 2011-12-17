#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"
#include "sysinterface.h"

SysInterface::SysInterface() {
	cb = static_cast <CBEmu *> (this);
}
	
void SysInterface::commandSetWindow(void) {
	string quit = cb->popValue<string>();
	uint32_t mode = cb->popValue<int32_t>();
	string caption = "CBEmu: " + cb->popValue<string>();
	
	if (quit != "") {
		cout << "FIXME: setWindow quitmsg" << endl;
	}
	
	static_cast<CBEmu *>(this)->getWindow()->set_title(caption);
}

void SysInterface::commandEnd(void) {
	static_cast<CBEmu *> (this)->stop();
}

void SysInterface::functionTimer(void) {
	cb->pushValue((int32_t)CL_System::get_time());
}

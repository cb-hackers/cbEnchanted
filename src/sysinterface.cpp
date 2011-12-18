#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include <time.h>

SysInterface::SysInterface() {
	cb = static_cast <CBEmu *> (this);
}

void SysInterface::commandWait(void) {
	
}

void SysInterface::commandMakeError(void) {
	
}

void SysInterface::commandSaveProgram(void) {
	
}

void SysInterface::commandLoadProgram(void) {
	
}

void SysInterface::commandGotoSavedLocation(void) {
	
}

void SysInterface::commandFrameLimit(void) {
	
}

void SysInterface::commandEncrypt(void) {
	
}

void SysInterface::commandDecrypt(void) {
	
}

void SysInterface::commandCallDLL(void) {
	
}

void SysInterface::commandErrors(void) {
	
}

void SysInterface::commandSetWindow(void) {
	string quit = cb->popValue<string>();
	uint32_t mode = cb->popValue<int32_t>();
	string caption = "CBEmu: " + cb->popValue<string>();
	
	if (quit != "") {
		cout << "FIXME: setWindow quitmsg" << endl;
	}
	
	static_cast<CBEmu *>(this)->getWindow()->SetTitle(caption);
}

void SysInterface::commandEnd(void) {
	static_cast<CBEmu *> (this)->stop();
}

void SysInterface::functionTimer(void) {
	cb->pushValue((int32_t)clock());
}

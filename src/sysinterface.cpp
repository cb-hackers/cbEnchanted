#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include <time.h>
#include <iostream>

SysInterface::SysInterface() {
	cb = static_cast <CBEnchanted *> (this);
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
    string quit = cb->popValue().toString();
    uint32_t mode = cb->popValue().toInt();
    string caption = "CBEnchanted: " + cb->popValue().toString();
	
	if (quit != "") {
		cout << "FIXME: setWindow quitmsg" << endl;
	}
	
	cb->getWindow()->SetTitle(caption);
}

void SysInterface::commandEnd(void) {
	cb->stop();
}

void SysInterface::functionDate(void) {
	
}

void SysInterface::functionTime(void) {
	
}

void SysInterface::functionTimer(void) {
	cb->pushValue((int32_t)clock());
}

void SysInterface::functionCommandLine(void) {
	
}

void SysInterface::functionGetEXEName(void) {
	
}

void SysInterface::functionFPS(void) {
    cb->pushValue(cb->getFPS());
}

void SysInterface::functionCrc32(void) {
	
}

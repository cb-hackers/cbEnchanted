#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include <time.h>
#include <iostream>
#ifdef WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif

SysInterface::SysInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

void SysInterface::commandWait(void) {
#ifdef WIN32
	Sleep(cb->popValue().toInt());
#else
	uint32_t sleepTime = cb->popValue().toInt();
	usleep(sleepTime * 1000);
#endif
}

void SysInterface::commandMakeError(void) {
#ifdef WIN32
	MessageBoxA(NULL,cb->popValue().toString().c_str(),"Error",MB_OK);
#else
	assert("Linux MakeError uncomplete" != 0);
#endif
}

void SysInterface::commandSaveProgram(void) {
	STUB;
}

void SysInterface::commandLoadProgram(void) {
	STUB;
}

void SysInterface::commandGotoSavedLocation(void) {
	STUB;
}

void SysInterface::commandFrameLimit(void) {
	cb->getWindow()->SetFramerateLimit(cb->popValue().toInt());
}

void SysInterface::commandEncrypt(void) {
	STUB;
}

void SysInterface::commandDecrypt(void) {
	STUB;
}

void SysInterface::commandCallDLL(void) {
	STUB;
}

void SysInterface::commandErrors(void) {
	STUB;
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
	STUB;
}

void SysInterface::functionTime(void) {
	STUB;
}

void SysInterface::functionTimer(void) {
#ifdef WIN32	
	cb->pushValue((int32_t)clock());
#else
	struct timeval tv;

	gettimeofday(&tv, NULL);

	//mtime = (tiemz.tv_sec * 1000 + tiemz.tv_usec / 1000.0) + 0.5;
	cb->pushValue((int32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
#endif
}

void SysInterface::functionCommandLine(void) {
#ifdef WIN32
	cb->pushValue(string(GetCommandLineA()));
#else
	assert("Linux CommandLine uncomplete" != 0);
#endif
}

void SysInterface::functionGetEXEName(void) {
	STUB;
}

void SysInterface::functionFPS(void) {
    cb->pushValue(cb->getFPS());
}

void SysInterface::functionCrc32(void) {
	STUB;
}

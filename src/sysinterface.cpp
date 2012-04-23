#include "precomp.h"
#include "interfacesystem.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include <time.h>
#include <iostream>
#include <boost/crc.hpp>
#ifdef WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif
#include "util.h"

SysInterface::SysInterface() {
	sys = static_cast <InterfaceSystem *> (this);
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
	MessageBoxA(NULL,cb->popValue().toString().getRef().c_str(), "Error", MB_OK);
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
	int32_t fps_limit = cb->popValue().toInt();
	cb->setFrameLimit(fps_limit);
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
	string quit = cb->popValue().toString().getRef();
	uint32_t mode = cb->popValue().toInt();
	string caption = cb->popValue().toString().getRef();

	if (quit != "") {
		FIXME("FIXME: setWindow quitmsg");
	}
	al_set_window_title(cb->getWindow(),caption.c_str());
}

void SysInterface::commandEnd(void) {
	cb->stop();
}

void SysInterface::functionDate(void) {
	time_t rtime;
	struct tm * timei;
	time (&rtime);
	timei = localtime(&rtime);

	string d(ctime(&rtime));

	short day  = timei->tm_mday;

	stringstream ssdate;
	ssdate << day << " " << d.substr(4, 3) << " " << d.substr(d.length() - 5, 4);

	cb->pushValue(ssdate.str());
}

void SysInterface::functionTime(void) {
	time_t rtime;
	time (&rtime);
	string t(ctime(&rtime));

	cb->pushValue(t.substr(11, 8));
}

void SysInterface::functionTimer(void) {
	cb->pushValue((int32_t)mtimer());
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
	const ISString &str = cb->popValue().getString();
	boost::crc_32_type result;
	result.process_bytes(str.getRef().c_str(), str.getRef().length());
	cb->pushValue((int32_t)result.checksum());
}

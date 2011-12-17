#include "precomp.h"
#include "cbemu.h"
#include "textinterface.h"

TextInterface::TextInterface() : cb(static_cast <CBEmu *> (this)) {
	
}	

void TextInterface::commandPrint(void) {
	string s = cb->popValue<string>();
}

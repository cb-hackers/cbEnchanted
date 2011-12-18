#include "precomp.h"
#include "cbemu.h"
#include "textinterface.h"

TextInterface::TextInterface() : cb(static_cast <CBEmu *> (this)) {
	
}
		
void TextInterface::commandSetFont(void) {
	
}

void TextInterface::commandDeleteFont(void) {
	
}

void TextInterface::commandText(void) {
	
}

void TextInterface::commandCenterText(void) {
	
}

void TextInterface::commandVerticalText(void) {
	
}

void TextInterface::commandPrint(void) {
	string s = cb->popValue<string>();
}

void TextInterface::commandWrite(void) {
	
}

void TextInterface::commandLocate(void) {
	
}

void TextInterface::commandAddText(void) {
	
}

void TextInterface::commandClearText(void) {
	
}

#include "precomp.h"
#include "cbenchanted.h"
#include "textinterface.h"
#include "mathoperations.h"
#ifdef WIN32
	#include <Windows.h>
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
#endif

#ifdef WIN32
#define DEFAULT_FONT "C:/Windows/Fonts/cour.ttf"
#else
#define DEFAULT_FONT "/usr/share/fonts/TTF/cour.ttf"
#endif

TextInterface::TextInterface() : cb(static_cast <CBEnchanted *> (this)) {
	currentFont = new CBFont;
	currentFont->font.LoadFromFile(DEFAULT_FONT);
	currentFont->fontSize = 12;
	currentFont->style = sf::Text::Regular;
}

TextInterface::~TextInterface() {
	delete currentFont;
}

	
void TextInterface::commandSetFont(void) {
	STUB;
}

void TextInterface::commandDeleteFont(void) {
	STUB;
}

void TextInterface::commandText(void) {
	string txt = cb->popValue().toString();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();

	sf::Text text(txt, currentFont->font, currentFont->fontSize);
	text.SetStyle(currentFont->style);
	text.SetColor(cb->getDrawColor());
	text.SetPosition(x, y);

	cb->getCurrentRenderTarget()->draw(text);
}

void TextInterface::commandCenterText(void) {
	STUB;
}

void TextInterface::commandVerticalText(void) {
	STUB;
}

void TextInterface::commandPrint(void) {
	string s = cb->popValue().getString();
}

void TextInterface::commandWrite(void) {
	STUB;
}

void TextInterface::commandLocate(void) {
	STUB;
}

void TextInterface::commandAddText(void) {
	STUB;
}

void TextInterface::commandClearText(void) {
	STUB;
}

void TextInterface::functionLoadFont(void) {
	STUB;
}

void TextInterface::functionTextWidth(void) {
	STUB;
}

void TextInterface::functionTextHeight(void) {
	STUB;
}

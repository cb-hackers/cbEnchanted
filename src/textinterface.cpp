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
#define FONT_PATH "C:/Windows/Fonts/"
#else
#define DEFAULT_FONT "/usr/share/fonts/TTF/cour.ttf"
#define FONT_PATH "/usr/share/fonts/"
#endif

TextInterface::TextInterface() : cb(static_cast <CBEnchanted *> (this)) {
	currentFont = new CBFont;
	currentFont->font.LoadFromFile(DEFAULT_FONT);
	currentFont->fontSize = 12;
	currentFont->style = sf::Text::Regular;
	fontMap[0] = currentFont;
}

TextInterface::~TextInterface() {
}

	
void TextInterface::commandSetFont(void) {
	int32_t id = cb->popValue().toInt();
	currentFont = fontMap[id];
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
	string file = cb->popValue().toString();
	uint16_t size = cb->popValue().toInt();
	uint8_t bold = cb->popValue().toInt();
	uint8_t italic = cb->popValue().toInt();
	uint8_t underLine = cb->popValue().toInt();

	string path = FONT_PATH + file;

	int styles = sf::Text::Regular;

	if(bold > 0){
		styles = styles | sf::Text::Bold;
	}
	if(italic > 0){
		styles = styles | sf::Text::Italic;
	}
	if(underLine > 0){
		styles = styles | sf::Text::Underlined;
	}

	CBFont *font = new CBFont;
	font->font.LoadFromFile(path);
	font->fontSize = size;
	font->style = (sf::Text::Style)styles;

	int32_t keyId = nextfontid();
	fontMap[keyId] = font;
}

void TextInterface::functionTextWidth(void) {
	STUB;
}

void TextInterface::functionTextHeight(void) {
	STUB;
}

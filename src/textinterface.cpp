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

TextInterface::TextInterface() : cb(static_cast <CBEnchanted *> (this)), locationX(0), locationY(0) {
	currentFont = new CBFont;
	currentFont->font.loadFromFile(DEFAULT_FONT);
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
	string txt = cb->popValue().toString().getRef();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();

	sf::Text text(txt, currentFont->font, currentFont->fontSize);
	text.setStyle(currentFont->style);
	text.setColor(cb->getDrawColor());
	text.setPosition(x, y);

	cb->getCurrentRenderTarget()->draw(text);
}

void TextInterface::commandCenterText(void) {
	STUB;
}

void TextInterface::commandVerticalText(void) {
	STUB;
}

void TextInterface::commandPrint(void) {
	STUB;
	string s = cb->popValue().toString().getRef();
}

void TextInterface::commandWrite(void) {
	STUB;
}

void TextInterface::commandLocate(void) {
	STUB;
}

void TextInterface::commandAddText(void) {
	string str = cb->popValue().toString().getRef();
	addTexts *newtxt = new addTexts;
	newtxt->font = currentFont;
	newtxt->txt = str;
	newtxt->txtX = locationX;
	newtxt->txtY = locationY;
	newtxt->col = cb->getDrawColor();
	texts.push_back(newtxt);
	bool bold = (newtxt->font->style == 1 << 0);
	locationY+=currentFont->font.getGlyph('A', currentFont->fontSize, bold).textureRect.height;
}

void TextInterface::commandClearText(void) {
	locationX = 0;
	locationY = 0;
	vector<addTexts*>::iterator i;
	for(i = texts.begin(); i != texts.end(); i++){
		delete (*i);
	}
	texts.clear();
}

void TextInterface::functionLoadFont(void) {
	string file = cb->popValue().toString().getRef();
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
	font->font.loadFromFile(path);
	font->fontSize = size;
	font->style = (sf::Text::Style)styles;

	int32_t keyId = nextfontid();
	fontMap[keyId] = font;
}

void TextInterface::renderAddTexts(){
	vector<addTexts*>::iterator i;
	for(i = texts.begin(); i != texts.end(); i++){
		int32_t x = (*i)->txtX;
		int32_t y = (*i)->txtY;
		string str = (*i)->txt;


		sf::Text text(str, (*i)->font->font, (*i)->font->fontSize);
		text.setStyle((*i)->font->style);
		text.setColor((*i)->col);
		text.setPosition(x, y);

		cb->getWindow()->draw(text);
	}
}

void TextInterface::functionTextWidth(void) {
	STUB;
}

void TextInterface::functionTextHeight(void) {
	STUB;
}

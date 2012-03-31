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
#include <allegro5/allegro_ttf.h>
#include <iostream>

TextInterface::TextInterface() : cb(static_cast <CBEnchanted *> (this)), locationX(0), locationY(0) {

}

TextInterface::~TextInterface() {

}
bool TextInterface::initializeFonts() {
	al_init_font_addon();
	al_init_ttf_addon();
	currentFont = al_load_font(DEFAULT_FONT,12,0);
	if (currentFont == 0) return false;
	fontMap[0] = currentFont;
	return true;
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
	cb->getCurrentRenderTarget()->drawText(currentFont,txt,x,y,cb->getDrawColor());
}

void TextInterface::commandCenterText(void) {
	uint8_t style = cb->popValue().toInt();
	string str = cb->popValue().getString().getRef();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	switch(style){
		case 0:{
			cb->getCurrentRenderTarget()->drawText(currentFont,str,x,y,cb->getDrawColor(),RenderTarget::HCenter);
		}
		break;
		case 1:{
			cb->getCurrentRenderTarget()->drawText(currentFont,str,x,y,cb->getDrawColor(),RenderTarget::VCenter);
		}
		case 2:{
			cb->getCurrentRenderTarget()->drawText(currentFont,str,x,y,cb->getDrawColor(),RenderTarget::Center);
		}
		break;
	}
}

void TextInterface::commandVerticalText(void) {
	STUB;
	/*
	string str = cb->popValue().toString().getRef();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	uint16_t chars = str.length();
	bool bold = (currentFont->style == 1 << 0);
	uint16_t fontH = currentFont->font.getGlyph((char)str[0], currentFont->fontSize, bold).textureRect.height;
	for(uint16_t i = 0; i < chars; i++){
		sf::Text text(str[i], currentFont->font, currentFont->fontSize);
		text.setStyle(currentFont->style);
		text.setColor(cb->getDrawColor());
		text.setPosition(x, y+i*fontH);
		cb->getCurrentRenderTarget()->draw(text);
	}*/
}

void TextInterface::commandPrint(void) {
	std::cout << cb->popValue().toString().getRef() << std::endl;
}

void TextInterface::commandWrite(void) {
	std::cout << cb->popValue().toString().getRef();
}

void TextInterface::commandLocate(void) {
	locationY = cb->popValue().toInt();
	locationX = cb->popValue().toInt();
}

void TextInterface::commandAddText(void) {
	AddText *newtxt = new AddText;
	newtxt->font = currentFont;
	newtxt->txt = cb->popValue().toString();
	newtxt->txtX = locationX;
	newtxt->txtY = locationY;
	newtxt->col = cb->getDrawColor();
	texts.push_back(newtxt);
	locationY+=al_get_font_line_height(currentFont);
}

void TextInterface::commandClearText(void) {
	locationX = 0;
	locationY = 0;
	vector<AddText*>::iterator i;
	for(i = texts.begin(); i != texts.end(); i++){
		delete (*i);
	}
	texts.clear();
}

void TextInterface::functionLoadFont(void) {
	//TODO find fonts
	string file = cb->popValue().toString().getRef();
	uint16_t size = cb->popValue().toInt();
	uint8_t bold = cb->popValue().toInt();
	uint8_t italic = cb->popValue().toInt();
	uint8_t underLine = cb->popValue().toInt();

	string path = FONT_PATH + file;

	ALLEGRO_FONT *font = al_load_font(file.c_str(),size,ALLEGRO_TTF_MONOCHROME);
	int32_t keyId = nextfontid();
	fontMap[keyId] = font;
}

void TextInterface::renderAddTexts(RenderTarget &r){
	if (texts.empty()) return;
	r.setAsCurrent();
	//al_hold_bitmap_drawing(true);

	vector<AddText*>::iterator i;
	r.useWorldCoords(false);
	for(i = texts.begin(); i != texts.end(); i++){
		r.drawText((*i)->font,(*i)->txt.getRef(),(*i)->txtX,(*i)->txtY,(*i)->col);
	}
	//al_hold_bitmap_drawing(false);
}

void TextInterface::functionTextWidth(void) {
	cb->pushValue(al_get_text_width(currentFont,cb->popValue().getString().getRef().c_str()));
}

void TextInterface::functionTextHeight(void) {
	int dummy;
	int height;
	al_get_text_dimensions(currentFont,cb->popValue().getString().getRef().c_str(),&dummy,&dummy,&dummy,&height);
	cb->pushValue(height);
}

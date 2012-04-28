#include "precomp.h"
#include "cbenchanted.h"
#include "textinterface.h"
#include "util.h"
#include "errorsystem.h"
#include <string>
#ifdef WIN32
	#include <Windows.h>
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
#endif

#ifdef WIN32
#define DEFAULT_FONT "C:/Windows/Fonts/cour.ttf"
#define FONT_PATH "C:/Windows/Fonts/"
#define FALLBACK_FONT "C:/Windows/Fonts/DejaVuSansMono.ttf"
#else
#define DEFAULT_FONT "/usr/share/fonts/TTF/cour.ttf"
#define FONT_PATH "/usr/share/fonts/"
#define FALLBACK_FONT "/usr/share/fonts/TTF/DejaVuSansMono.ttf"
#endif
#include <allegro5/allegro_ttf.h>
#include <iostream>

#ifdef FONTCONFIG_FOUND
#include <fontconfig.h>
#endif // FONTCONFIG_FOUND

TextInterface::TextInterface() : locationX(0), locationY(0) {
	cb = static_cast <CBEnchanted *> (this);
}

TextInterface::~TextInterface() {

}

bool TextInterface::initializeFonts() {
	al_init_font_addon();
	al_init_ttf_addon();
#ifdef FONTCONFIG_FOUND
	if (!FcInit()) {
		INFO("Failed to initialize fontconfig");
		return false;
	}
	FcInitLoadConfigAndFonts();
	currentFont = al_load_font(findfont("Courier New"), 12, ALLEGRO_TTF_MONOCHROME);
#else
	currentFont = al_load_font(DEFAULT_FONT, 12, ALLEGRO_TTF_MONOCHROME);
#endif
	if (currentFont == 0) {
		// Unable to load courier. Try FALLBACK_FONT
		INFO("Failed to load font \"%s\"", DEFAULT_FONT);
		INFO("-> Trying to load \"%s\"", FALLBACK_FONT);
		currentFont = al_load_font(FALLBACK_FONT, 12, ALLEGRO_TTF_MONOCHROME);
		if (currentFont == 0) {
			INFO(" -> Failed to load even the fallback font!");
			INFO(" -> If you are using text in your program without first setting")
			INFO("    a font with LoadFont and SetFont, your program might crash!");
			return true;
		}
	}
	fontMap[0] = currentFont;
	return true;
}

void TextInterface::commandSetFont(void) {
	int32_t id = cb->popValue().toInt();
	currentFont = 0;
	if (fontMap.count(id)) {
		currentFont = fontMap[id];
	}
	if (currentFont == 0) {
		if (cb->errors->createError("SetFont() failed!", "Failed to load a font with id " + boost::lexical_cast<string>(id))) {
			// Try to continue
			currentFont = fontMap[0];
		}
	}
}

void TextInterface::commandDeleteFont(void) {
	int32_t id = cb->popValue().toInt();
	FIXME("DeleteFont not yet implemented");
}

void TextInterface::commandText(void) {
	string txt = cb->popValue().toString().getRef();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();

	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawTextToWorld());
	cb->getCurrentRenderTarget()->drawText(currentFont, txt, x, y, cb->getDrawColor());
}

void TextInterface::commandCenterText(void) {
	uint8_t style = cb->popValue().toInt();
	ISString str = cb->popValue().toString();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawTextToWorld());
	switch (style) {
		case 0:
			cb->getCurrentRenderTarget()->drawText(
				currentFont, str.getRef(), x, y, cb->getDrawColor(), RenderTarget::HCenter
			);
		break;
		case 1:
			cb->getCurrentRenderTarget()->drawText(
				currentFont, str.getRef(), x, y, cb->getDrawColor(), RenderTarget::VCenter
			);
		break;
		case 2:
			cb->getCurrentRenderTarget()->drawText(
				currentFont, str.getRef(), x, y, cb->getDrawColor(), RenderTarget::Center
			);
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
	locationY += al_get_font_line_height(currentFont);
}

void TextInterface::commandClearText(void) {
	locationX = 0;
	locationY = 0;
	vector<AddText*>::iterator i;
	for (i = texts.begin(); i != texts.end(); i++) {
		delete (*i);
	}
	texts.clear();
}

void TextInterface::functionLoadFont(void) {
	//TODO find fonts on windows and do underlining with line primitives
	bool underLine = cb->popValue().toBool();
	bool italic = cb->popValue().toBool();
	bool bold = cb->popValue().toBool();
	int size = cb->popValue().toInt();
	string fontname = cb->popValue().toString().getRef();

	ALLEGRO_PATH *path;
	if (fontname.find_first_of('.') == fontname.npos) {
		// If the fontname doesn't imply that it is a path to a font file, try to find it
		// from systems own font dir
		path = al_create_path(findfont(fontname.c_str(), bold, italic));
	}
	else {
		// Font name had a dot in it, so we assume the programmer wanted to load a font
		// relative to the program.
		path = al_create_path(fontname.c_str());
	}

	ALLEGRO_FONT *font = al_load_font(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), size, ALLEGRO_TTF_MONOCHROME);
	int32_t keyId = nextfontid();
	fontMap[keyId] = font;

	cb->pushValue(keyId);
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
	cb->pushValue(al_get_text_width(currentFont, cb->popValue().getString().getRef().c_str()));
}

void TextInterface::functionTextHeight(void) {
	int dummy;
	int height;
	al_get_text_dimensions(currentFont, cb->popValue().getString().getRef().c_str(), &dummy, &dummy, &dummy, &height);
	cb->pushValue(height);
}

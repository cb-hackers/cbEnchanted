#include "precomp.h"
#include "cbenchanted.h"
#include "textinterface.h"
#include "util.h"
#include "errorsystem.h"
#include <string>
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
		cerr << "Failed to initialize fontconfig" << endl;
		return false;
	}
	FcInitLoadConfigAndFonts();
#endif
	currentFont = al_load_font(findfont("Courier New").c_str(), 12, ALLEGRO_TTF_MONOCHROME);
	if (currentFont == 0) {
		cerr << " -> Failed to load the default font Courier New!" << endl;
		cerr << " -> If you are using text in your program without first setting" << endl;
		cerr << "    a font with LoadFont and SetFont, your program might crash!" << endl;
		return true;
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
		if (cb->errors->createError("SetFont() failed!", "Failed to load a font with id " + boost::lexical_cast<string>(id) + ". You probably have deleted it with DeleteFont() and then tried to use it.")) {
			// Try to continue
			currentFont = fontMap[0];
		}
	}
}

void TextInterface::commandDeleteFont(void) {
	int32_t id = cb->popValue().toInt();
	if (fontMap.count(id) == 0) {
		// Font that we tried to delete didn't exist
		cb->errors->createError("DeleteFont() failed!", "Font with id " + boost::lexical_cast<string>(id) + " is not loaded!");
		return;
	}

	ALLEGRO_FONT *fontToDelete = fontMap[id];
	al_destroy_font(fontToDelete);
	fontMap.erase(id);
}

void TextInterface::commandText(void) {
	const ISString &txt = cb->popValue().toString();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();

	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawTextToWorld() && !cb->drawingOnImage());
	cb->getCurrentRenderTarget()->drawText(currentFont, txt, x, y, cb->getDrawColor());
}

void TextInterface::commandCenterText(void) {
	uint8_t style = cb->popValue().toInt();
	const ISString &str = cb->popValue().toString();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawTextToWorld() && !cb->drawingOnImage());
	switch (style) {
		case 0:
			cb->getCurrentRenderTarget()->drawText(currentFont, str, x, y, cb->getDrawColor(), RenderTarget::HCenter);
			break;
		case 1:
			cb->getCurrentRenderTarget()->drawText(currentFont, str, x, y, cb->getDrawColor(), RenderTarget::VCenter);
			break;
		case 2:
			cb->getCurrentRenderTarget()->drawText(currentFont, str, x, y, cb->getDrawColor(), RenderTarget::Center);
			break;
	}
}

void TextInterface::commandVerticalText(void) {
	const ISString &str = cb->popValue().toString();
	int32_t x = cb->popValue().toInt();
	int32_t y = cb->popValue().toInt();
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawTextToWorld() && !cb->drawingOnImage());
	uint16_t textLen = str.length();
	uint16_t textHeight = al_get_font_line_height(currentFont);
	string Str = str.getStdString();

	for(uint16_t i = 0; i < textLen; i++) {
		string charac;
		charac = Str[i];
		cb->getCurrentRenderTarget()->drawText(currentFont, ISString(charac), x, y, cb->getDrawColor());
		y += textHeight;
	}

}

void TextInterface::commandPrint(void) {
	#ifdef _WIN32
		std::cout << utf8toCP1252(cb->popValue().toString().getUtf8Encoded()) << std::endl;
	#else
		std::cout << cb->popValue().toString().getUtf8Encoded() << std::endl;
	#endif
}

void TextInterface::commandWrite(void) {
	#ifdef _WIN32
		std::cout << utf8toCP1252(cb->popValue().toString().getUtf8Encoded());
	#else
		std::cout << cb->popValue().toString().getUtf8Encoded();
	#endif
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
	//TODO do underlining with line primitives
	bool underLine = cb->popValue().toBool();
	bool italic = cb->popValue().toBool();
	bool bold = cb->popValue().toBool();
	int size = cb->popValue().toInt();
	string fontname = cb->popValue().toString().getRef();

	ALLEGRO_PATH *path;
	if (fontname.find_first_of('.') == fontname.npos) {
		// If the fontname doesn't imply that it is a path to a font file, try to find it
		// from systems own font dir
		path = al_create_path(findfont(fontname.c_str(), bold, italic).c_str());
	}
	else {
		// Font name had a dot in it, so we assume the programmer wanted to load a font
		// relative to the program.
		path = al_create_path(fontname.c_str());
	}

	ALLEGRO_FONT *font;
	if (cb->isSmooth2D()) {
		// If Smooth2D is set, use font antialiasing
		font = al_load_font(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), size, 0);
	} else {
		font = al_load_font(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), size, ALLEGRO_TTF_MONOCHROME);
	}
	if (font != NULL) {
		int32_t keyId = nextfontid();
		fontMap[keyId] = font;
		cb->pushValue(keyId);
	}
	else {
		// Error, but what kind?
		string strpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
		if (strpath != "") {
			cb->errors->createError("LoadFont() failed!", "Failed to load font " + fontname + "\nTried to load it from the following path:\n" + strpath);
		}
		else {
			cb->errors->createError("LoadFont() failed!", "Failed to load font \"" + fontname + "\".\nThe font file could not be found.");
		}
		cb->pushValue(0);
	}
}

void TextInterface::renderAddTexts(RenderTarget &r){
	if (texts.empty()) return;
	r.setAsCurrent();
	//al_hold_bitmap_drawing(true);

	vector<AddText*>::iterator i;
	r.useWorldCoords(false);
	for(i = texts.begin(); i != texts.end(); i++){
		r.drawText((*i)->font, (*i)->txt, (*i)->txtX,(*i)->txtY, (*i)->col);
	}
	//al_hold_bitmap_drawing(false);
}

void TextInterface::functionTextWidth(void) {
	cb->pushValue(al_get_text_width(currentFont, cb->popValue().toString().getRef().c_str()));
}

void TextInterface::functionTextHeight(void) {
	int dummy;
	int height;
	al_get_text_dimensions(currentFont, cb->popValue().toString().getRef().c_str(), &dummy, &dummy, &dummy, &height);
	cb->pushValue(height);
}

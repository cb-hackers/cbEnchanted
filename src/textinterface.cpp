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
		
void TextInterface::commandSetFont(void) {
	
}

void TextInterface::commandDeleteFont(void) {
	
}

void TextInterface::commandText(void) {
    glEnable(GL_TEXTURE_2D);
    string txt = cast_to_string(cb->popValue());
    float y = cast<float>(cb->popValue());
    float x = cast<float>(cb->popValue());

    sf::Text text(txt,currentFont->font,currentFont->fontSize);
    text.SetStyle(currentFont->style);
    text.SetColor(cb->getDrawColor());
    text.SetPosition(x,y);

    cb->getWindow()->Draw(text);
    glDisable(GL_TEXTURE_2D);
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

void TextInterface::functionLoadFont(void) {
	
}

void TextInterface::functionTextWidth(void) {
	
}

void TextInterface::functionTextHeight(void) {
	
}

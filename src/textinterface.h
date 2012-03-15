#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEnchanted;
#include "precomp.h"
#include <SFML/Graphics/Font.hpp>

struct CBFont
{
	sf::Font font;
	sf::Text::Style style;
	uint32_t fontSize;
};

struct addTexts{
	string txt;
	CBFont *font;
	int32_t txtX;
	int32_t txtY;
	sf::Color col;
};

class TextInterface {
	public:
		TextInterface();
		~TextInterface();

		void commandSetFont(void);
		void commandDeleteFont(void);
		void commandText(void);
		void commandCenterText(void);
		void commandVerticalText(void);
		void commandPrint(void);
		void commandWrite(void);
		void commandLocate(void);
		void commandAddText(void);
		void commandClearText(void);
		
		void functionLoadFont(void);
		void functionTextWidth(void);
		void functionTextHeight(void);
		void renderAddTexts();
		CBFont *GetCurrentFont() { return currentFont; }
	private:
		CBEnchanted *cb;
		map<int32_t, CBFont*> fontMap;
		CBFont *currentFont;
		int32_t locationX;
		int32_t locationY;
		std::vector<addTexts*> texts;
};		inline int32_t nextfontid(){static int32_t fontID = 0; return ++fontID;}

#endif

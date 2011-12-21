#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEnchanted;
#include <SFML/Graphics/Font.hpp>

struct CBFont
{
	sf::Font font;
	sf::Text::Style style;
	uint32_t fontSize;
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
		CBFont *GetCurrentFont() { return currentFont; }
	private:
		CBEnchanted *cb;
		CBFont *currentFont;
};

#endif

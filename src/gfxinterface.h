#ifndef GFXINTERFACE_H
#define GFXINTERFACE_H

#include <SFML/Graphics.hpp>

class CBEmu;

class GfxInterface {
	public:
		GfxInterface();
		~GfxInterface();
		
		void functionScreen(void);
		void commandScreen(void);
		void commandClsColor(void);
		void commandColor(void);
		void commandCircle(void);
		void commandDrawScreen(void);
		void commandLine(void);
		
		sf::RenderWindow *getWindow(void) { return &this->window; }
	private:
		CBEmu *cb;
		
		string windowTitle;
		sf::RenderWindow window;
		sf::Color clearColor;
		sf::Color drawColor;
};

#endif

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
		void commandLock(void);
		void commandUnlock(void);
		void commandPutPixel(void);
		void commandPutPixel2(void);
		void commandCopyBox(void);
		void commandColor(void);
		void commandClsColor(void);
		void commandCls(void);
		void commandDot(void);
		void commandLine(void);
		void commandBox(void);
		void commandCircle(void);
		void commandEllipse(void);
		void commandPickColor(void);
		void commandScreenGamma(void);
		void commandDrawToImage(void);
		void commandDrawToScreen(void);
		void commandDrawToWorld(void);
		void commandSmooth2D(void);
		void commandScreenShot(void);
		void commandUpdateGame(void);
		void commandDrawGame(void);
		void commandDrawScreen(void);
		
		sf::RenderWindow *getWindow(void) { return &this->window; }
	private:
		CBEmu *cb;
		
		string windowTitle;
		sf::RenderWindow window;
		sf::Color clearColor;
		sf::Color drawColor;
};

#endif

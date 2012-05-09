#ifndef GFXINTERFACE_H
#define GFXINTERFACE_H

#include "rendertarget.h"
#include "cbmap.h"

class CBEnchanted;
class CBImage;


class GfxInterface {
	public:
		GfxInterface();
		~GfxInterface();

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

		void functionSCREEN(void);
		void functionImage(void);
		void functionGetPixel(void);
		void functionGetPixel2(void);
		void functionGetRGB(void);
		void functionScreenWidth(void);
		void functionScreenHeight(void);
		void functionScreenDepth(void);
		void functionGFXModeExists(void);

		int32_t getFPS()const{return currentFPS;}
		void sleep(int64_t time);

		ALLEGRO_DISPLAY *getWindow(void) { return window; }

		const ALLEGRO_COLOR &getDrawColor() const { return drawColor; }
		void setDrawColor(const ALLEGRO_COLOR &c) {drawColor = c;}
		const ALLEGRO_COLOR &getClearColor() const {return clearColor; }
		void setClearColor(const ALLEGRO_COLOR &c) {clearColor = c;}

		inline bool getDrawDrawCommandToWorld()const{return drawDrawCommandToWorld;}
		inline bool getDrawImageToWorld()const{return drawImageToWorld;}
		inline bool getDrawTextToWorld()const{return drawTextToWorld;}

		inline RenderTarget *getCurrentRenderTarget(){return currentRenderTarget;}
		void setCurrentRenderTarget(RenderTarget *t);
		bool drawingOnImage() const { return imageToDrawTo != 0; }
		bool initializeGfx();
		RenderTarget *getBuffer(int32_t id) {return bufferMap[id];}


	private:
		void registerWindow();
		void unregisterWindow();
		void resizeTempBitmap(int32_t w, int32_t h);

		CBEnchanted *cb;
		float windowScaleX,windowScaleY;
		string windowTitle;
		ALLEGRO_DISPLAY *window;
		ALLEGRO_COLOR clearColor;
		ALLEGRO_COLOR drawColor;
		int32_t fpsCounter;
		int32_t currentFPS;
		clock_t lastSecTimer;
		int64_t lastFrameTime;
		RenderTarget *currentRenderTarget;

		RenderTarget *windowRenderTarget;
		ALLEGRO_BITMAP *drawscreenTempBitmap;
		float windowGammaR;
		float windowGammaG;
		float windowGammaB;
		bool resizableWindow; //cbSizable enabled
		bool drawDrawCommandToWorld;
		bool drawImageToWorld;
		bool drawTextToWorld;
		bool gameDrawn; //Game has already been drawn in DrawGame
		bool gameUpdated; //Game has already been updated in UpdateGame

		/** Buffer's returned by SCREEN() and Image() */
		std::map<int32_t,RenderTarget*> bufferMap;

		/** Holds the image DrawToImage() was called with. */
		CBImage* imageToDrawTo;
};

#endif

#ifndef GFXINTERFACE_H
#define GFXINTERFACE_H

#include "rendertarget.h"
#include "cbmap.h"

class CBEnchanted;
class CBImage;

typedef void (*VoidFuncPtrType) (void);

class GfxInterface {
	public:
#ifndef CBE_LIB
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
#endif
		int32_t getFPS() const { return currentFPS; }

		ALLEGRO_DISPLAY *getWindow(void) { return window; }

		const ALLEGRO_COLOR &getDrawColor() const { return drawColor; }
		void setDrawColor(const ALLEGRO_COLOR &c) { drawColor = c; }
		const ALLEGRO_COLOR &getClearColor() const { return clearColor; }
		void setClearColor(const ALLEGRO_COLOR &c) { clearColor = c; }

		inline bool getDrawDrawCommandToWorld() const { return drawDrawCommandToWorld; }
		inline bool getDrawImageToWorld() const { return drawImageToWorld; }
		inline bool getDrawTextToWorld() const { return drawTextToWorld; }

		inline int32_t screenWidth() { return windowRenderTarget->width(); }
		inline int32_t screenHeight() { return windowRenderTarget->height(); }

		inline int32_t getDefaultWidth() { return defaultWidth; }
		inline int32_t getDefaultHeight() { return defaultHeight; }
		inline RenderTarget *getCurrentRenderTarget() { return currentRenderTarget; }
		void setCurrentRenderTarget(RenderTarget *t);
		bool drawingOnImage() const { return imageToDrawTo != 0; }
		bool initializeGfx();
		RenderTarget *getBuffer(int32_t id) { return bufferMap[id]; }
		float getLineWidth() const { return lineWidth; }
		void setLineWidth(float w) { lineWidth = w; }

		/** Adds function pointer which will be called after DrawScreen. */
		void addDrawScreenCallback(int32_t group, VoidFuncPtrType ptr) { drawScreenCallbacks[group] = ptr; }
		/** Adds function pointer which will be called after DrawGame. */
		void addDrawGameCallback(int32_t group, VoidFuncPtrType ptr) { drawGameCallbacks[group] = ptr; }
		/** Adds function pointer which will be called after UpdateGame. */
		void addUpdateGameCallback(int32_t group, VoidFuncPtrType ptr) { updateGameCallbacks[group] = ptr; }
	private:
		enum WindowMode {
			FullScreen,
			Windowed,
			Resizeable,
			LockedAspect
		};
		
		void registerWindow();
		void unregisterWindow();
		void resizeTempBitmap(int32_t w, int32_t h);

		CBEnchanted *cb;
		float windowScaleX, windowScaleY;
		string windowTitle;
		ALLEGRO_DISPLAY *window;
		ALLEGRO_COLOR clearColor;
		ALLEGRO_COLOR drawColor;
		int32_t fpsCounter;
		int32_t currentFPS;
		clock_t lastSecTimer;
		
		WindowMode windowMode;
		
		int64_t lastFrameTime;
		RenderTarget *currentRenderTarget;

		int32_t defaultWidth;
		int32_t defaultHeight;
		int32_t newWidth;
		int32_t newHeight;

		RenderTarget *windowRenderTarget;
		ALLEGRO_BITMAP *drawscreenTempBitmap;

		float defaultAspectRatio;

		float windowGammaR;
		float windowGammaG;
		float windowGammaB;
		bool resizableWindow; //cbSizable enabled
		bool drawDrawCommandToWorld;
		bool drawImageToWorld;
		bool drawTextToWorld;
		bool gameDrawn; //Game has already been drawn in DrawGame
		bool gameUpdated; //Game has already been updated in UpdateGame
		float lineWidth;

		/** Buffer's returned by SCREEN() and Image() */
		std::map<int32_t,RenderTarget*> bufferMap;

		/** Callbacks*/
		map<int32_t, VoidFuncPtrType> drawScreenCallbacks;
		map<int32_t, VoidFuncPtrType> updateGameCallbacks;
		map<int32_t, VoidFuncPtrType> drawGameCallbacks;

		void callDrawScreenCallbacks();
		void callDrawGameCallbacks();
		void callUpdateGameCallbacks();

		/** Holds the image DrawToImage() was called with. */
		CBImage* imageToDrawTo;
};

#endif

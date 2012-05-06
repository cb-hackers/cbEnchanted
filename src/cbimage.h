#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertarget.h"
class CBParticleEmitter;
class CBImage
{
	public:
		friend class RenderTarget;
		CBImage();
		~CBImage();
		bool load(const string &path);
		bool save(const string &path);
		void lock();
		void unlock();
		int32_t width()const{return renderTarget.width();}
		int32_t height()const{return renderTarget.height();}
		void makeImage(int32_t w, int32_t h);
		void setHotspot(float x,float y) {hotspotX = x;hotspotY = y;}
		RenderTarget *getRenderTarget(){return &renderTarget;}
		void draw(RenderTarget &r,float x,float y,bool useMask);
		void draw(RenderTarget &r,float x,float y,int frame, bool useMask);
		void drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, bool useMask);
		void drawBox(RenderTarget &r,float sx,float sy, float sw, float sh, float tx, float ty, int frame, bool useMask);
		void maskImage(const ALLEGRO_COLOR &color);
		ALLEGRO_COLOR getMaskColor()const{return maskColor;}
		void setAnimParams(int32_t frameW,int32_t frameH,int32_t begining,int32_t animL){frameWidth = frameW;frameHeight = frameH;animBegin = begining;animLength = animL;}
		void resize(int32_t w, int32_t h);
		CBImage *clone();
		static void initMaskShader();
		/** Set this CBImage ready for drawing operations or set it back for drawing. */
		void switchMaskBitmaps(bool switchToUnmasked);
	private:
		float hotspotX,hotspotY;
		RenderTarget renderTarget;
		ALLEGRO_COLOR maskColor;
		int32_t frameWidth;
		int32_t frameHeight;
		int32_t animBegin;
		int32_t animLength;

		/** The unmasked version of this image */
		ALLEGRO_BITMAP* unmaskedBitmap;
		/** The masked version of this image - is NULL before setupForDrawOperations() is called. */
		ALLEGRO_BITMAP* maskedBitmap;
};

#endif // CBIMAGE_H

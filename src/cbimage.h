#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertarget.h"
class CBParticleEmitter;
class CBImage
{
	public:
		friend class RenderTarget;
		friend class GfxInterface;
		CBImage();
		~CBImage();
		bool load(const string &path);
		bool save(const string &path);
		void lock();
		void unlock();
		int32_t width() const { return renderTarget.width(); }
		int32_t height() const { return renderTarget.height(); }
		void makeImage(int32_t w, int32_t h);
		/** Sets image hotspot to the given coordinates or to the center, if either one is < 0 */
		void setHotspot(int32_t x, int32_t y);
		RenderTarget *getRenderTarget() { return &renderTarget; }
		void draw(RenderTarget &r ,float x, float y, bool useMask);
		void draw(RenderTarget &r, float x, float y, int frame, bool useMask);
		void drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, bool useMask);
		void drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, int frame, bool useMask);
		/** Draws this image with the given level (0.0f to 1.0f) of alpha blending. */
		void drawAlphaBlended(RenderTarget &r, float x, float y, float alpha);
		void drawAlphaBlended(RenderTarget &r, float x, float y, int frame, float alpha);
		void maskImage(const ALLEGRO_COLOR &color);
		ALLEGRO_COLOR getMaskColor() const { return maskColor; }
		void setAnimParams(int32_t frameW, int32_t frameH, int32_t begin, int32_t animL);
		void resize(int32_t w, int32_t h);
		CBImage *clone();
		ALLEGRO_BITMAP *getMaskedBitmap() const { return maskedBitmap; }
		ALLEGRO_BITMAP *getUnmaskedBitmap() const { return unmaskedBitmap; }
		/** Set this CBImage ready for drawing operations or set it back for drawing. */
		void switchMaskBitmaps(bool switchToUnmasked);
		/** Rotates an image with the given angle (in degrees) clockwise. */
		void rotate(float angle);
		/** Checks if an image overlaps another image on their bounding boxes. */
		bool overlaps(CBImage *img, float x1, float y1, float x2, float y2);
		/** Checks if an image collides with another image on a pixel precise level. */
		bool collides(CBImage *img, float x1, float y1, float x2, float y2);
		int32_t getHotSpotX() const { return hotspotX; }
		int32_t getHotSpotY() const { return hotspotY; }
		int32_t getFrameWidth() const { return frameWidth; }
		int32_t getFrameHeight() const { return frameHeight; }

		bool masked() { return isMasked; }
		void cleanDirtyMask();
	private:

		int32_t hotspotX, hotspotY;
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
		/** Is this image masked or not */
		bool isMasked;
		bool maskIsDirty;
		bool *maskData;
};

#endif // CBIMAGE_H

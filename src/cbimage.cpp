#include "cbimage.h"
#include "cbenchanted.h"
#include "collisioncheck.h"

CBImage::CBImage() :
	hotspotX(0),
	hotspotY(0),
	renderTarget(),
	frameWidth(0),
	frameHeight(0),
	animBegin(0),
	animLength(0),
	isMasked(false),
	maskedBitmap(NULL),
	unmaskedBitmap(NULL)
{
}

CBImage::~CBImage() {
	renderTarget.swapBitmap(NULL);
	if (unmaskedBitmap != NULL) {
		al_destroy_bitmap(unmaskedBitmap);
	}
	al_destroy_bitmap(maskedBitmap);
}

bool CBImage::load(const string &path) {
	if (!renderTarget.load(path)) {
		return false;
	}
	maskedBitmap = renderTarget.getBitmap();
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
	return true;
}

bool CBImage::save(const string &path) {
	return al_save_bitmap(path.c_str(),unmaskedBitmap);
}

void CBImage::draw(RenderTarget &r,float x, float y, bool useMask) {
	if (useMask) {
		r.drawBitmap(maskedBitmap, x - hotspotX, y - hotspotY);
	}
	else {
		r.drawBitmap(unmaskedBitmap, x - hotspotX, y - hotspotY);
	}
}

void CBImage::draw(RenderTarget &r, float x, float y, int frame, bool useMask)
{
	if (animLength == 0) { //Not anim image
		draw(r, x, y, useMask);
		return;
	}
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth);
	float frameAreaTop = (copyY * frameWidth);
	float frameAreaHeight = frameHeight;
	float frameAreaWidth = frameWidth;
	if (useMask) {
		r.drawBitmapRegion(
			maskedBitmap,
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			x - hotspotX,
			y - hotspotY
		);
	}
	else {
		r.drawBitmapRegion(
			unmaskedBitmap,
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			x - hotspotX,
			y - hotspotY
		);
	}
}

void CBImage::drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, bool useMask) {
	if (useMask) {
		r.drawBitmapRegion(maskedBitmap, sx, sy, sw, sh, tx, ty);
	}
	else {
		r.drawBitmapRegion(unmaskedBitmap, sx, sy, sw, sh, tx, ty);
	}
}

void CBImage::drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, int frame, bool useMask) {

	if (animLength == 0) { //Not anim image
		drawBox(r, sx, sy, sw, sh, tx, ty, useMask);
		return;
	}
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth) + sx;
	float frameAreaTop = (copyY * frameWidth) + sy;
	drawBox(r, frameAreaLeft, frameAreaTop, sw, sh, tx, ty, useMask);
}

/** Draws this image with the given level (0.0f to 1.0f) of alpha blending. */
void CBImage::drawAlphaBlended(RenderTarget &r, float x, float y, float alpha) {
	r.drawBitmap(unmaskedBitmap, x, y, al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha));
}

void CBImage::drawAlphaBlended(RenderTarget &r, float x, float y, int frame, float alpha) {
	if (animLength == 0) {
		drawAlphaBlended(r, x, y, alpha);
		return;
	}
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth);
	float frameAreaTop = (copyY * frameWidth);
	float frameAreaHeight = frameHeight;
	float frameAreaWidth = frameWidth;
	r.drawBitmapRegion(
		unmaskedBitmap,
		frameAreaLeft,
		frameAreaTop,
		frameAreaWidth,
		frameAreaHeight,
		al_map_rgba_f(1,1,1,alpha),
		x - hotspotX,
		y - hotspotY,
		0
	);
}

/** Turns the current image bitmap to an alpha masked version and saves the unmasked version. */
void CBImage::maskImage(const ALLEGRO_COLOR &color) {
	isMasked = true;
	maskColor = color;
	maskedBitmap = al_clone_bitmap(unmaskedBitmap);
	al_destroy_bitmap(unmaskedBitmap);
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
	al_convert_mask_to_alpha(maskedBitmap, maskColor);
	renderTarget.changeBitmap(maskedBitmap);
}

void CBImage::setAnimParams(int32_t frameW, int32_t frameH, int32_t begin, int32_t animL) {
	frameWidth = frameW;
	frameHeight = frameH;
	animBegin = begin;
	animLength = animL;
}

void CBImage::resize(int32_t w, int32_t h) {
	this->switchMaskBitmaps(true);
	renderTarget.resize(w, h);
	unmaskedBitmap = renderTarget.getBitmap();
	this->switchMaskBitmaps(false);
	hotspotX = 0;
	hotspotY = 0;
}

CBImage *CBImage::clone() {
	CBImage *newImg = new CBImage;
	newImg->renderTarget.copy(&this->renderTarget);
	newImg->maskColor = this->maskColor;
	newImg->hotspotX = this->hotspotX;
	newImg->hotspotY = this->hotspotY;
	newImg->frameWidth = this->frameWidth;
	newImg->frameHeight = this->frameHeight;
	newImg->animBegin = this->animBegin;
	newImg->animLength = this->animLength;
	newImg->maskedBitmap = newImg->renderTarget.getBitmap();
	newImg->unmaskedBitmap = al_clone_bitmap(this->unmaskedBitmap);
	newImg->isMasked = this->isMasked;
	return newImg;
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(al_map_rgb(0, 0, 0));
	maskedBitmap = renderTarget.getBitmap();
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
}

/** Sets image hotspot to the given coordinates or to the center, if either one is < 0 */
void CBImage::setHotspot(int32_t x, int32_t y) {
	if (x < 0 || y < 0) {
		// Set hotspot to center
		if (frameWidth && frameHeight) {
			// Center of a single frame
			hotspotX = frameWidth / 2.0f;
			hotspotY = frameHeight / 2.0f;
		}
		else {
			hotspotX = this->width() / 2.0f;
			hotspotY = this->height() / 2.0f;
		}
	}
	else {
		hotspotX = x;
		hotspotY = y;
	}
}

/** Set this CBImage ready for drawing operations or set it back for drawing. */
void CBImage::switchMaskBitmaps(bool switchToUnmasked) {
	// If we don't have masking on, we don't need to do anything.
	if (!isMasked) {
		return;
	}
	if (switchToUnmasked) {
		renderTarget.changeBitmap(unmaskedBitmap);
		maskedBitmap = NULL;
	}
	else {
		maskedBitmap = al_clone_bitmap(unmaskedBitmap);
		al_convert_mask_to_alpha(maskedBitmap, maskColor);
		renderTarget.swapBitmap(maskedBitmap);
	}
}

/** Rotates an image with the given angle (in degrees) clockwise. */
void CBImage::rotate(float angle) {
	float radAngle = (angle / 180.0f) * M_PI;
	float oldWidth = al_get_bitmap_width(maskedBitmap);
	float oldHeight = al_get_bitmap_height(maskedBitmap);

	float mSin = fabs(sin(radAngle));
	float mCos = fabs(cos(radAngle));

	float x1 = -0.5f * (oldWidth * mCos + oldHeight * mSin);
	float x2 =  0.5f * (oldWidth * mCos + oldHeight * mSin);

	float y1 = -0.5f * (oldWidth * mSin + oldHeight * mCos);
	float y2 =  0.5f * (oldWidth * mSin + oldHeight * mCos);

	float newHeight, newWidth;
	newWidth = (x1 > x2) ? x1 - x2 : x2 - x1;
	newHeight = (y1 > y2) ? y1 - y2 : y2 - y1;

	hotspotX = ceil(newWidth * 0.5f);
	hotspotY = ceil(newHeight * 0.5f);

	ALLEGRO_BITMAP* newUnmaskedBitmap = al_create_bitmap(int(ceil(newWidth) + 1e-5f), int(ceil(newHeight) + 1e-5f));
	ALLEGRO_BITMAP* newMaskedBitmap;

	renderTarget.changeBitmap(newUnmaskedBitmap);
	renderTarget.drawBitmap(unmaskedBitmap, hotspotX, hotspotY, radAngle);

	if (isMasked) {
		newMaskedBitmap = al_clone_bitmap(newUnmaskedBitmap);
		al_convert_mask_to_alpha(newMaskedBitmap, maskColor);
		al_destroy_bitmap(unmaskedBitmap);
	}
	else {
		newMaskedBitmap = newUnmaskedBitmap;
	}

	renderTarget.swapBitmap(newMaskedBitmap);
	maskedBitmap = newMaskedBitmap;
	unmaskedBitmap = newUnmaskedBitmap;
}

/** Checks if an image overlaps another image on their bounding boxes.
 *
 * @param img Image to check overlapping against
 * @param x1,y1 Top left coordinates of this image
 * @param x2,y2 Top left coordinates of the given image
 */
bool CBImage::overlaps(CBImage *img, float x1, float y1, float x2, float y2) {
	float w1 = al_get_bitmap_width(this->maskedBitmap);
	float h1 = al_get_bitmap_height(this->maskedBitmap);
	float w2 = al_get_bitmap_width(img->maskedBitmap);
	float h2 = al_get_bitmap_height(img->maskedBitmap);

	// Flip y-coordinates because RectRectTest uses world coordinates
	return CollisionCheck::RectRectTest(x1, -y1, w1, h1, x2, -y2, w2, h2);
}

/** Checks if an image collides with another image on a pixel precise level.
 *
 * @param img Image to check overlapping against
 * @param x1,y1 Top left coordinates of this image
 * @param x2,y2 Top left coordinates of the given image
 */
bool CBImage::collides(CBImage *img, float x1, float y1, float x2, float y2) {
	float w1 = al_get_bitmap_width(this->maskedBitmap);
	float h1 = al_get_bitmap_height(this->maskedBitmap);
	float w2 = al_get_bitmap_width(img->maskedBitmap);
	float h2 = al_get_bitmap_height(img->maskedBitmap);

	// First check for a simple rectangle collision. If there's no rectangle overlapping,
	// there can't be any pixel-precise overlapping either.
	// Flip y-coordinates because RectRectTest uses world coordinates
	if (!CollisionCheck::RectRectTest(x1, -y1, w1, h1, x2, -y2, w2, h2)) {
		return false;
	}

	ALLEGRO_BITMAP *img1, *img2;
	int mask1, mask2;
	int format1, format2;

	// Image1
	if (this->isMasked) {
		img1 = this->maskedBitmap;
		format1 = ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA;
	}
	else {
		img1 = this->unmaskedBitmap;
		format1 = ALLEGRO_PIXEL_FORMAT_ANY_NO_ALPHA;
	}
	mask1 = int(this->maskColor.a) << 24 + int(this->maskColor.r) << 16 + int(this->maskColor.g) << 8 + int(this->maskColor.b);

	// Image2
	if (img->isMasked) {
		img2 = img->maskedBitmap;
		format2 = ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA;
	}
	else {
		img2 = img->unmaskedBitmap;
		format2 = ALLEGRO_PIXEL_FORMAT_ANY_NO_ALPHA;
	}
	mask2 = int(img->maskColor.a) << 24 + int(img->maskColor.r) << 16 + int(img->maskColor.g) << 8 + int(img->maskColor.b);

	int xmax1 = x1 + w1, ymax1 = y1 + h1;
	int xmax2 = x2 + w2, ymax2 = y2 + h2;

	int xmin = max(x1, x2);
	int ymin = max(y1, y2);

	int xmax = min(xmax1, xmax2);
	int ymax = min(ymax1, ymax2);



	// Lock images for speed
	ALLEGRO_LOCKED_REGION *lock1 = al_lock_bitmap(img1, format1, ALLEGRO_LOCK_READONLY);
	ALLEGRO_LOCKED_REGION *lock2 = al_lock_bitmap(img2, format2, ALLEGRO_LOCK_READONLY);


	// Collide detection
	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			int cx1 = x - x1, cy1 = y - y1;
			int cx2 = x - x2, cy2 = y - y2;

			ALLEGRO_COLOR tcolor1 = al_get_pixel(img1, cx1, cy1);
			ALLEGRO_COLOR tcolor2 = al_get_pixel(img2, cx2, cy2);
			int a,r,g,b;

			a = int(tcolor1.a);
			r = int(tcolor1.r);
			g = int(tcolor1.g);
			b = int(tcolor1.b);
			int color1 = (a << 24) + (r << 16) + (g << 8) + b;

			a = int(tcolor2.a);
			r = int(tcolor2.r);
			g = int(tcolor2.g);
			b = int(tcolor2.b);
			int color2 = (a << 24) + (r << 16) + (g << 8) + b;

			if (color1 != mask1 && color2 != mask2) {
				return true;
			}

		}
	}



	return false;
}

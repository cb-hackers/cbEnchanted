#include "cbimage.h"
#include "cbenchanted.h"

CBImage::CBImage() :
	hotspotX(0),
	hotspotY(0),
	renderTarget(),
	frameWidth(0),
	frameHeight(0),
	animBegin(0),
	animLength(0)
{
	maskColor = al_map_rgb(0, 0, 0);
	maskedBitmap = NULL;
	unmaskedBitmap = NULL;
}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
	if (!renderTarget.load(path)) {
		return false;
	}
	maskedBitmap = renderTarget.getBitmap();
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
	return true;
}

void CBImage::draw(RenderTarget &r,float x, float y, bool useMask) {
	r.drawBitmap(renderTarget.getBitmap(), x - hotspotX, y - hotspotY);
}

void CBImage::draw(RenderTarget &r, float x, float y, int frame, bool useMask)
{
	if (animLength == 0) { //Not anim image
		draw(r,x,y,useMask);
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
		//TODO MASKING
		r.drawBitmapRegion(
			renderTarget.getBitmap(),
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
			renderTarget.getBitmap(),
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
	r.drawBitmapRegion(renderTarget.getBitmap(),sx,sy,sw,sh,tx,ty);
}

void CBImage::drawBox(RenderTarget &r, float sx, float sy, float sw, float sh, float tx, float ty, int frame, bool useMask) {

	if (animLength == 0) { //Not anim image
		drawBox(r,sx,sy,sw,sh,tx,ty,useMask);
		return;
	}
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame - copyX) / framesY;

	float frameAreaLeft = (copyX * frameWidth)+sx;
	float frameAreaTop = (copyY * frameWidth)+sy;
	drawBox(r,frameAreaLeft,frameAreaTop,sw,sh,tx,ty,useMask);
}

/** Turns the current image bitmap to an alpha masked version and saves the unmasked version. */
void CBImage::maskImage(const ALLEGRO_COLOR &color) {
	maskColor = color;
	maskedBitmap = al_clone_bitmap(unmaskedBitmap);
	al_destroy_bitmap(unmaskedBitmap);
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
	al_convert_mask_to_alpha(maskedBitmap, maskColor);
	ALLEGRO_BITMAP* toBeDeleted = renderTarget.swapBitmap(maskedBitmap);
	al_destroy_bitmap(toBeDeleted);
}

void CBImage::resize(int32_t w, int32_t h) {
	this->switchMaskBitmaps(true);
	renderTarget.resize(w, h);
	al_destroy_bitmap(maskedBitmap);
	this->switchMaskBitmaps(false);
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
	return newImg;
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(al_map_rgb(0, 0, 0));
	maskedBitmap = renderTarget.getBitmap();
	unmaskedBitmap = al_clone_bitmap(maskedBitmap);
}

/** Set this CBImage ready for drawing operations or set it back for drawing. */
void CBImage::switchMaskBitmaps(bool switchToUnmasked) {
	if (switchToUnmasked) {
		maskedBitmap = renderTarget.swapBitmap(unmaskedBitmap);
	}
	else {
		maskedBitmap = al_clone_bitmap(renderTarget.getBitmap());
		al_convert_mask_to_alpha(maskedBitmap, maskColor);
		unmaskedBitmap = renderTarget.swapBitmap(maskedBitmap);
	}
}

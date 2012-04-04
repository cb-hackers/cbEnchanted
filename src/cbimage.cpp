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
	maskColor = al_map_rgb_f(0, 0, 0);
}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
	return renderTarget.load(path);
}

void CBImage::draw(RenderTarget &r,float x, float y, bool useMask) {
	if (useMask) {
		//TODO MASKING
		r.drawBitmap(renderTarget.getBitmap(), x - hotspotX, y - hotspotY);
	}
	else {
		r.drawBitmap(renderTarget.getBitmap(), x - hotspotX, y - hotspotY);
	}
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

void CBImage::drawBox(RenderTarget &r, float rx, float ry, float rw, float rh, float x, float y, int frame, bool useMask)
{
	/*
	if (animLength == 0) { //Not anim image
		r.drawBitmapRegion(renderTarget.getBitmap(),rx,ry,rw,rh,x,y);
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
	}*/
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(al_map_rgb_f(0, 0, 0));
}

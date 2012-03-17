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
	maskColor = al_map_rgb(0,0,0);
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
	if (animLength == 0) {draw(r,x,y,useMask);return;} //Not anim image
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame-copyX) / framesY;

	float frameAreaLeft = (copyX*frameWidth);
	float frameAreaTop = (copyY*frameWidth);
	float frameAreaHeight = frameHeight;
	float frameAreaWidth = frameWidth;

	float drawAreaLeft = x - hotspotX;
	float drawAreaTop = y - hotspotY;
	float drawAreaWidth = frameWidth;
	float drawAreaHeight = frameHeight;
	if (useMask) {
		//TODO MASKING
		r.drawBitmapRegion(renderTarget.getBitmap(),frameAreaLeft,frameAreaTop,frameAreaWidth,frameAreaWidth,x,y);
	}
	else {
		r.drawBitmapRegion(renderTarget.getBitmap(),frameAreaLeft,frameAreaTop,frameAreaWidth,frameAreaWidth,x,y);
	}
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(al_map_rgb_f(1.0f,1.0f,1.0f));
}

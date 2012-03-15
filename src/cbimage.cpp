#include "cbimage.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

CBImage::CBImage() :
	hotspotX(0),
	hotspotY(0),
	maskColor(0,0,0),
	renderTarget(),
	frameWidth(0),
	frameHeight(0),
	animBegin(0),
	animLength(0)
{

}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
	sf::Texture texture;
	if (!texture.loadFromFile(path)) return false;
	renderTarget.create(texture);
	return true;
}

void CBImage::draw(float x, float y, bool useMask) {
	if (useMask) {
		CBEnchanted::instance()->getCurrentRenderTarget()->drawRenderTarget(renderTarget, x - hotspotX, y - hotspotY,maskColor);
	}
	else {
		CBEnchanted::instance()->getCurrentRenderTarget()->drawRenderTarget(renderTarget, x - hotspotX, y - hotspotY);
	}
}

void CBImage::draw(float x, float y, int frame, bool useMask)
{
	if (animLength == 0) {draw(x,y,useMask);return;} //Not anim image
	sf::FloatRect frameArea;
	//INFO("%i", frame)
	int32_t framesX = renderTarget.width() / frameWidth;
	int32_t framesY = renderTarget.height() / frameHeight;
	int32_t copyX = frame % framesX;
	int32_t copyY = (frame-copyX) / framesY;

	frameArea.left = (copyX*frameWidth)/(float)renderTarget.width();
	frameArea.top = (copyY*frameWidth)/(float)renderTarget.height();
	frameArea.height = frameHeight/(float)renderTarget.height();
	frameArea.width = frameWidth/(float)renderTarget.width();

	sf::FloatRect drawArea;
	drawArea.left = x - hotspotX;
	drawArea.top = y - hotspotY;
	drawArea.width = frameWidth;
	drawArea.height = frameHeight;
	if (useMask) {
		CBEnchanted::instance()->getCurrentRenderTarget()->drawRenderTarget(renderTarget,drawArea,frameArea,maskColor);
	}
	else {
		CBEnchanted::instance()->getCurrentRenderTarget()->drawRenderTarget(renderTarget, drawArea,frameArea);
	}
}

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(sf::Color(0, 0, 0));
}

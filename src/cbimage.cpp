#include "cbimage.h"
#include "cbenchanted.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

CBImage::CBImage() : hotspotX(0), hotspotY(0),maskColor(0,0,0), renderTarget() {

}

CBImage::~CBImage() {

}

bool CBImage::load(const string &path) {
	sf::Texture texture;
	if (!texture.LoadFromFile(path)) return false;
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

void CBImage::makeImage(int32_t w, int32_t h) {
	renderTarget.create(w, h);
	renderTarget.clear(sf::Color(0, 0, 0));
}

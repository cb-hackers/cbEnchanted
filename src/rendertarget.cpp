#include "rendertarget.h"
#include "cbenchanted.h"
#include "cbimage.h"
int32_t renderTargetIdCounter = 0;
RenderTarget *bindRenderTarget = 0;

void RenderTarget::useWorldCoords(bool t, bool force) {
	if (t == worldMatrixEnabled && !force) return;
	setAsCurrent();
	worldMatrixEnabled = t;
	ALLEGRO_TRANSFORM trans;
	if (t) al_build_transform(&trans,al_get_display_width(CBEnchanted::instance()->getWindow())*0.5f-CBEnchanted::instance()->getCameraX(),al_get_display_height(CBEnchanted::instance()->getWindow())*0.5f+CBEnchanted::instance()->getCameraY(),1.0f,-1.0f,0.0f);
	else al_identity_transform(&trans);
	//al_translate_transform(&trans,&trans,-al_get_display_width(CBEnchanted::instance()->getWindow())*0.5f,al_get_display_height(CBEnchanted::instance()->getWindow())*0.5f);
	al_use_transform(&trans);
}

void RenderTarget::drawParticles(CBImage *tex, const vector<Particle> &particles, int32_t particleLifeTime,int32_t animLength) {
	if (animLength == 0 || tex->animLength == 0) {
		float w = tex->getRenderTarget()->width();
		float h = tex->getRenderTarget()->height();
		for (vector<Particle>::const_iterator i = particles.begin();i != particles.end();i++) {
			this->drawBitmap(tex->getRenderTarget()->getBitmap(),i->x-w*0.5f-tex->hotspotX,i->y+h*0.5f-tex->hotspotY);
		}
	}
	else {
		float pLifeTime = static_cast<float>(particleLifeTime);
		float drawAreaWidth = tex->frameWidth;
		float drawAreaHeight = tex->frameHeight;
		float frameAreaHeight = tex->frameHeight;
		float frameAreaWidth = tex->frameWidth;
		for (vector<Particle>::const_iterator i = particles.begin();i != particles.end();i++) {
			int32_t frame = ((float)i->lifeTime / pLifeTime) * animLength;
			if (frame >= animLength) frame = animLength - 1;
			frame += tex->animBegin;
			int32_t framesX = tex->renderTarget.width() / tex->frameWidth;
			int32_t framesY = tex->renderTarget.height() / tex->frameHeight;
			int32_t copyX = frame % framesX;
			int32_t copyY = (frame-copyX) / framesY;

			float frameAreaLeft = ((framesX-copyX-1)*tex->frameWidth);
			float frameAreaTop = (copyY*tex->frameWidth);
			float drawAreaLeft = i->x - tex->hotspotX;
			float drawAreaTop = i->y - tex->hotspotY;
			this->drawBitmapRegion(tex->getRenderTarget()->getBitmap(),frameAreaLeft,frameAreaTop,frameAreaWidth,frameAreaHeight,drawAreaLeft,drawAreaTop,drawAreaWidth,drawAreaHeight);
		}
	}
}

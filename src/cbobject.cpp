
#include "precomp.h"
#include "debug.h"
#include "cbobject.h"
#include <math.h>
#include "cbenchanted.h"

static bool defaultVisible = true;

CBObject::CBObject(bool floor):
	visible(defaultVisible),
	posX(0),
	posY(0),
	angle(0),
	alphaBlend(255),
	startFrame(0),
	maxFrames(0),
	frameWidth(0),
	frameHeight(0),
	currentFrame(0),
	painted(false),
	floor(floor),
	texture(0),
	copied(false),
	sizeX(0),
	renderTarget(0),
	sizeY(0),
	usingLife(false),
	life(0),
	lastObj(0),
	nextObj(0),
	playing(false)
{
	maskColor = al_map_rgba_f(0,0,0,1);
}

CBObject::~CBObject() {
	if (!copied)
	{
		if (renderTarget) delete renderTarget;
		if (texture) al_destroy_bitmap(texture);
	}
}

bool CBObject::load(const string &file) {
	return load(file,maskColor);
}

bool CBObject::load(const string &file, const ALLEGRO_COLOR &mask) {
	if (renderTarget) delete renderTarget;
	renderTarget = new RenderTarget;
	if (!renderTarget->load(file)) return false;

	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture,mask);
	maskColor = mask;
	sizeX = renderTarget->width();
	sizeY = renderTarget->height();
	frameWidth = 0;
	frameHeight = 0;
	startFrame = 0;
	maxFrames = 0;
	alphaBlend = 255;
	angle = 0;
	currentFrame = 0;
	painted = true;
	return true;
}


bool CBObject::loadAnimObject(const string &file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount){
	if (!load(file)) return false;

	if((int)fw > renderTarget->width()){
		INFO("Frame width %u is more than texture width: %u", fw, renderTarget->width());
		return false;
	}
	if((int)fh > renderTarget->height()){
		INFO("Frame height %u is more than texture height: %u", fh, renderTarget->height());
		return false;
	}
	if(framecount > (renderTarget->width()/fw)*(renderTarget->height()/fh)){
		FIXME("Too much frames!")
		return false;
	}
	frameWidth = fw;
	frameHeight = fh;
	sizeX = frameWidth;
	sizeY = frameHeight;
	startFrame = startf;
	maxFrames = framecount;
	alphaBlend = 255;
	angle = 0;
	painted = true;
	currentFrame = 0;
	playing = false;
	return true;
}

void CBObject::positionObject(float x, float y){
	posX = x;
	posY = y;
}

void CBObject::paintObject(const RenderTarget &tex){
	if (!renderTarget) renderTarget = new RenderTarget;
	if (texture) al_destroy_bitmap(texture);
	renderTarget->copy(&tex);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture,maskColor);
	painted = true;
}

void CBObject::paintObject(const CBObject &obj){
	if (!renderTarget) renderTarget = new RenderTarget;
	if (texture) al_destroy_bitmap(texture);
	renderTarget->copy(obj.renderTarget);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture,maskColor);
	painted = true;
}

void CBObject::setFrame(uint16_t frame){
	currentFrame = frame;
}

void CBObject::startPlaying(uint16_t startf, uint16_t endf, float spd, bool continuous){
	if ((!continuous && !playing) || (continuous &&(currentFrame < startf || currentFrame > endf))) currentFrame = startf;
	animStartFrame = startf;
	animEndingFrame = endf;
	animSpeed = spd;
	playing = true;
}

bool CBObject::updateObject(float timestep) {
	if (playing) {
		if(animStartFrame < animEndingFrame){
			currentFrame = currentFrame + animSpeed;
			if ((int)currentFrame > animEndingFrame) {
				currentFrame = animStartFrame;
				if (!animLooping) playing = false;
			}
		}else{
			currentFrame = currentFrame - animSpeed;
			if ((int)currentFrame < animStartFrame) {
				currentFrame = animEndingFrame;
				if (!animLooping) playing = false;
			}
		}
	}
	if (usingLife) {
		--life;
		if (life <= 0) {
			life = 0;
			return true; //Delete object
		}
	}
	return false;
}

void CBObject::ghostObject(uint8_t ab){
	alphaBlend = ab;
}

void CBObject::maskObject(uint8_t r, uint8_t g, uint8_t b){
	if (!painted) {
		FIXME("Masking object without texture");
		return;
	}
	al_destroy_bitmap(texture);
	texture = al_clone_bitmap(renderTarget->getBitmap());
	al_convert_mask_to_alpha(texture,al_map_rgb(r,g,b));
}

void CBObject::moveObject(float fwrd, float sdwrd){
	posX+=cos(angle / 180.0 * M_PI) * fwrd + cos((angle-90.0) / 180.0 * M_PI)*sdwrd;
	posY+=sin(angle / 180.0 * M_PI) * fwrd + sin((angle-90.0) / 180.0 * M_PI)*sdwrd;
}

void CBObject::translateObject(float hor, float ver, float depth){
	posX+=hor;
	posY+=ver;
}

void CBObject::turnObject(float speed){
	angle+=speed;
	if(angle < 0){
		while(angle < 0){
			angle+=360;
		}
	}else if(angle > 360){
		while(angle > 360){
			angle-=360;
		}
	}

}

 void CBObject::render(RenderTarget &target){
	float camX = CBEnchanted::instance()->getCameraX();
	float camY = CBEnchanted::instance()->getCameraY();
	if (visible && painted) {
		if (floor) {
				//Drawing floor objects

			float snappedx=floorf(camX/renderTarget->width());
			float snappedy=floorf(camY/renderTarget->height());

			float ekax= camX+snappedx*renderTarget->width();
			float ekay=-camY-snappedy*renderTarget->height();

			float xx=ekax;
			while (xx<target.width()) {
			float yy=ekay;
			while (yy<target.height()) {
				target.drawBitmap(texture,xx,yy);
				yy=yy+renderTarget->height();
			}
			xx=xx+renderTarget->width();
			}
		} else {
			if(maxFrames!=0){
				int32_t frame = (int)currentFrame;
				//INFO("%i", frame)
				int32_t framesX = renderTarget->width() / frameWidth;
				int32_t framesY = renderTarget->height()/ frameHeight;
				int32_t copyX = frame % framesX;
				int32_t copyY = (frame-copyX) / framesY;
				//INFO("X, Y: %i,%i", copyX, copyY)
				target.drawBitmapRegion(texture,copyX*frameWidth, copyY*frameHeight, frameWidth, frameHeight,al_map_rgba(255,255,255,alphaBlend),posX,posY,angle*M_PI/180.0f);
				return;
			}
			if(alphaBlend!=255){
				target.drawBitmap(texture,posX,posY,angle+180,al_map_rgba(255,255,255,alphaBlend));
				return;
			}

			target.drawBitmap(texture,posX,posY,angle+180);
		}
	}
}

void CBObject::setPosition(float x, float y){
	posX = x;
	posY = y;
}

float CBObject::getX(){
	return posX;
}

float CBObject::getY(){
	return posY;
}

float CBObject::getAngle(){
	return angle;
}

void CBObject::setDefaultVisible(bool t) {
	defaultVisible = t;
}

CBObject *CBObject::copyObject() const {
	CBObject *obj = new CBObject(floor);
	obj->texture = texture;
	obj->renderTarget = renderTarget;
	obj->copied = true;
	obj->frameHeight = frameHeight;
	obj->frameWidth = frameWidth;
	obj->visible = true;
	return obj;
}


void CBObject::setLife(uint32_t energy){
	usingLife = true;
	life = energy;
}

uint32_t CBObject::getLife(){
	return life;
}

bool CBObject::isLife(){
	return usingLife;
}



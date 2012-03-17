
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
	imgtex(0),
	texture(0),
	copied(false),
	sizeX(0),
	sizeY(0),
	usingLife(false),
	life(0),
	lastObj(0),
	nextObj(0),
	playing(false)
{
}

CBObject::~CBObject() {
	if (!copied)
	{
		if (imgtex) delete imgtex;
		if (texture) delete texture;
	}
}

bool CBObject::load(const string &file) {
	return load(file,sf::Color(0,0,0));
}

bool CBObject::load(const string &file, const sf::Color &mask) {
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->loadFromFile(file)) return false;

	imgtex->createMaskFromColor(mask);

	if (texture) delete texture;
	texture = new sf::Texture;
	texture->loadFromImage(*imgtex);
	sprite.setTexture(*texture);
	sizeX = texture->getWidth();
	sizeY = texture->getHeight();
	frameWidth = 0;
	frameHeight = 0;
	startFrame = 0;
	maxFrames = 0;
	alphaBlend = 255;
	angle = 0;
	currentFrame = 0;
	painted = true;
	sprite.setScale(1.0,-1.0);
	return true;
}


bool CBObject::loadAnimObject(const string &file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount){
	INFO("%s, %u, %u, %u, %u", file.c_str(), fw, fh, startf, framecount)
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->loadFromFile(file)) return false;

	imgtex->createMaskFromColor(sf::Color(0, 0, 0));

	if (texture) delete texture;
	texture = new sf::Texture;
	bool success = texture->loadFromImage(*imgtex);
	INFO("Image copying: %u", success)


	INFO("Texture exists")
	INFO("Texture sizes(w, h): %u, %u", texture->getWidth(), texture->getHeight())


	if((int)fw > texture->getWidth()){
		INFO("Frame width %u is more than texture width: %u", fw, texture->getWidth());
		return false;
	}
	if((int)fh > texture->getHeight()){
		INFO("Frame height %u is more than texture height: %u", fh, texture->getHeight());
		return false;
	}
	if(framecount > (texture->getWidth()/fw)*(texture->getHeight()/fh)){
		FIXME("Too much frames!")
		return false;
	}
	sprite.setTexture(*texture);
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
	INFO("Everything ok!")
	sprite.setScale(1.0,-1.0);
	return true;
}

void CBObject::positionObject(float x, float y){
	posX = x;
	posY = y;
}

void CBObject::paintObject(const sf::Texture &txt){
    if (!imgtex) imgtex = new sf::Image;
    if (!texture) texture = new sf::Texture;
	*imgtex = txt.copyToImage();
	imgtex->createMaskFromColor(sf::Color(0, 0, 0));
	texture->loadFromImage(*imgtex);
	sprite.setTexture(*texture);
	painted = true;
}

void CBObject::paintObject(const CBObject &obj){
    if (!imgtex) imgtex = new sf::Image;
    if (!texture) texture = new sf::Texture;
	imgtex = obj.imgtex;
	texture = obj.texture;
	sprite.setTexture(*texture);
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

bool CBObject::updateObject(float timestep)
{
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
	imgtex->createMaskFromColor(sf::Color(r, g, b));
	texture->loadFromImage(*imgtex);
	sprite.setTexture(*texture);
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
}

 void CBObject::render(RenderTarget &target){
	float camX = CBEnchanted::instance()->getCameraX();
	float camY = CBEnchanted::instance()->getCameraY();
	if (visible && painted) {
		if (floor) {
		    sprite.setOrigin(0,0);
				//Drawing floor objects

			float snappedx=floorf(camX/texture->getWidth());
			float snappedy=floorf(camY/texture->getHeight());

			float ekax= camX+snappedx*texture->getWidth();
			float ekay=-camY-snappedy*texture->getHeight();

			float xx=ekax;
			while (xx<target.width()) {
			float yy=ekay;
			while (yy<target.height()) {
				sprite.setPosition(xx,yy);
				target.draw(sprite);
				yy=yy+texture->getHeight();
			}
			xx=xx+texture->getWidth();
			}
		} else {
			if(maxFrames!=0){
				int32_t frame = (int)currentFrame;
				//INFO("%i", frame)
				int32_t framesX = texture->getWidth() / frameWidth;
				int32_t framesY = texture->getHeight() / frameHeight;
				int32_t copyX = frame % framesX;
				int32_t copyY = (frame-copyX) / framesY;
				//INFO("X, Y: %i,%i", copyX, copyY)
				sprite.setTextureRect(sf::IntRect(((angle == 0) ? 1.0f:0.0f)+copyX*frameWidth, copyY*frameHeight, frameWidth, frameHeight));
				sprite.setOrigin((float)frameWidth*0.5f,(float)frameHeight*0.5f);
			}
			else {
				sprite.setOrigin(texture->getWidth()*0.5,texture->getHeight()*0.5);
			}
			sprite.setPosition(posX, posY);
			sprite.setRotation(angle);
			if(alphaBlend!=255){
				sprite.setColor(sf::Color(255, 255, 255, alphaBlend));

			}
			target.draw(sprite);
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
	obj->imgtex = imgtex;
	obj->copied = true;
	obj->sprite = sprite;
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



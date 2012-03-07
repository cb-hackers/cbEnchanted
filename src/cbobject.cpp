
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
	alphablend(255),
	startframe(0),
	maxframes(0),
	frameWidth(0),
	frameHeight(0),
	currentframe(0),
	painted(false),
	floor(floor),
	imgtex(0),
	texture(0),
	copied(false),
	sizeX(0),
	sizeY(0),
	usinglife(0),
	life(0)
{
}

CBObject::~CBObject() {
	if (!copied)
	{
		if (imgtex) delete imgtex;
		if (texture) delete texture;
	}
}

bool CBObject::load(string file)
{
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->LoadFromFile(file)) return false;

	imgtex->CreateMaskFromColor(sf::Color(0, 0, 0));

	if (texture) delete texture;
	texture = new sf::Texture;
	texture->LoadFromImage(*imgtex);
	sprite.SetTexture(*texture);
	sizeX = texture->GetWidth();
	sizeY = texture->GetHeight();
	frameWidth = 0;
	frameHeight = 0;
	startframe = 0;
	maxframes = 0;
	alphablend = 255;
	angle = 0;
	painted = true;
	return true;
}

bool CBObject::load(string file, const sf::Color &mask)
{
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->LoadFromFile(file)) return false;

	imgtex->CreateMaskFromColor(mask);

	if (texture) delete texture;
	texture = new sf::Texture;
	texture->LoadFromImage(*imgtex);
	sprite.SetTexture(*texture);
	sizeX = texture->GetWidth();
	sizeY = texture->GetHeight();
	frameWidth = 0;
	frameHeight = 0;
	startframe = 0;
	maxframes = 0;
	alphablend = 255;
	angle = 0;
	painted = true;
	return true;
}


bool CBObject::loadAnimObject(string file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount){
	INFO("%s, %u, %u, %u, %u", file.c_str(), fw, fh, startf, framecount)
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->LoadFromFile(file)) return false;

	imgtex->CreateMaskFromColor(sf::Color(0, 0, 0));

	if (texture) delete texture;
	texture = new sf::Texture;
	bool success = texture->LoadFromImage(*imgtex);
	INFO("Image copying: %u", success)


	INFO("Texture exists")
	INFO("Texture sizes(w, h): %u, %u", texture->GetWidth(), texture->GetHeight())


	if((int)fw > texture->GetWidth()){
		INFO("Frame width %u is more than texture width: %u", fw, texture->GetWidth());
		return false;
	}
	if((int)fh > texture->GetHeight()){
		INFO("Frame height %u is more than texture height: %u", fh, texture->GetHeight());
		return false;
	}
	if(framecount > (texture->GetWidth()/fw)*(texture->GetHeight()/fh)){
		FIXME("Too much frames!")
		return false;
	}

	frameWidth = fw;
	frameHeight = fh;
	sizeX = frameWidth;
	sizeY = frameHeight;
	startframe = startf;
	maxframes = framecount;
	alphablend = 255;
	angle = 0;
	painted = true;
	INFO("Everything ok!")
	return true;
}

void CBObject::positionObject(float x, float y){
	posX = x;
	posY = y;
}

void CBObject::paintObject(const sf::Texture &txt){
    if (!imgtex) imgtex = new sf::Image;
    if (!texture) texture = new sf::Texture;
	*imgtex = txt.CopyToImage();
	imgtex->CreateMaskFromColor(sf::Color(0, 0, 0));
	texture->LoadFromImage(*imgtex);
	sprite.SetTexture(*texture);
	painted = true;
}

void CBObject::paintObject(const CBObject &obj){
    if (!imgtex) imgtex = new sf::Image;
    if (!texture) texture = new sf::Texture;
	imgtex = obj.imgtex;
	texture = obj.texture;
	sprite.SetTexture(*texture);
	painted = true;
}

void CBObject::setFrame(uint16_t frame){
	currentframe = frame;
}

void CBObject::setFrames(uint16_t startf, uint16_t endf, float spd, uint8_t looping){
	if(currentframe < startf && currentframe > endf) currentframe = startf;
	animStartFrame = startf;
	animEndingFrame = endf;
	animSpeed = spd;
	animLooping = looping;
}

void CBObject::playObject(){
	if((int)currentframe > animEndingFrame && animLooping == 0) currentframe = animStartFrame;
	if((int)currentframe < animStartFrame && animLooping == 0) currentframe = animEndingFrame;
	if(animStartFrame < animEndingFrame){
		currentframe = currentframe + animSpeed;
	}else{
		currentframe = currentframe - animSpeed;
	}
}

void CBObject::ghostObject(uint8_t ab){
	alphablend = ab;
}

void CBObject::maskObject(uint8_t r, uint8_t g, uint8_t b){
	if (!painted) {
		FIXME("Masking object without texture");
		return;
	}
	imgtex->CreateMaskFromColor(sf::Color(r, g, b));
	texture->LoadFromImage(*imgtex);
	sprite.SetTexture(*texture);
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
		    sprite.SetOrigin(0,0);
				//Drawing floor objects

		    float snappedx=floorf(camX/texture->GetWidth());
		    float snappedy=floorf(camY/texture->GetHeight());

		    float ekax= camX+snappedx*texture->GetWidth();
		    float ekay=-camY-snappedy*texture->GetHeight();

		    float xx=ekax;
		    while (xx<target.width()) {
			float yy=ekay;
			while (yy<target.height()) {
			    sprite.SetPosition(xx,yy);
			    target.draw(sprite);
			    yy=yy+texture->GetHeight();
			}
			xx=xx+texture->GetWidth();
		    }
		} else {
			sprite.SetPosition(posX, posY);
			if(maxframes!=0){
				int32_t frame = (int)ceil(currentframe);
				INFO("%i", frame)
				int32_t framesX = texture->GetWidth() / frameWidth;
				int32_t framesY = texture->GetHeight() / frameHeight;
				int32_t copyX = frame % framesX;
				int32_t copyY = frame / framesY;
				INFO("X, Y: %i,%i", copyX, copyY)
				sprite.SetTextureRect(sf::IntRect(copyX*frameWidth, copyY*frameHeight, frameWidth, frameHeight));
				sprite.SetOrigin(frameWidth*0.5,frameHeight*0.5);
			}
			else {
				sprite.SetOrigin(texture->GetWidth()*0.5,texture->GetHeight()*0.5);
			}

			sprite.SetRotation(angle);
			if(alphablend!=255){
				sprite.SetColor(sf::Color(255, 255, 255, alphablend));

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
	usinglife = 1;
	life = energy;
}

uint32_t CBObject::getLife(){
	return life;
}

uint8_t CBObject::isLife(){
	return usinglife;
}



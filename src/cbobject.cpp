
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
	startFrame = 0;
	maxFrames = 0;
	alphaBlend = 255;
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
	startFrame = 0;
	maxFrames = 0;
	alphaBlend = 255;
	angle = 0;
	currentFrame = 0;
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
	sprite.SetTexture(*texture);
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
	currentFrame = frame;
}

void CBObject::startPlaying(uint16_t startf, uint16_t endf, float spd, bool continuous){
	if (!continuous || (continuous &&(currentFrame < startf || currentFrame > endf))) currentFrame = startf;
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
			if(maxFrames!=0){
				int32_t frame = (int)currentFrame;
				//INFO("%i", frame)
				int32_t framesX = texture->GetWidth() / frameWidth;
				int32_t framesY = texture->GetHeight() / frameHeight;
				int32_t copyX = frame % framesX;
				int32_t copyY = (frame-copyX) / framesY;
				//INFO("X, Y: %i,%i", copyX, copyY)
				sprite.SetTextureRect(sf::IntRect(((angle == 0) ? 1.0f:0.0f)+copyX*frameWidth, copyY*frameHeight, frameWidth, frameHeight));
				sprite.SetOrigin((float)frameWidth*0.5f,(float)frameHeight*0.5f);
			}
			else {
				sprite.SetOrigin(texture->GetWidth()*0.5,texture->GetHeight()*0.5);
			}
			sprite.SetPosition(posX, posY);
			sprite.SetRotation(angle);
			if(alphaBlend!=255){
				sprite.SetColor(sf::Color(255, 255, 255, alphaBlend));

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




#include "precomp.h"
#include "debug.h"
#include "cbobject.h"
#include <math.h>
#include "cbenchanted.h"

static bool defaultVisible = true;

CBObject::CBObject(bool floor):visible(defaultVisible),posX(0),posY(0),angle(0),alpha(255),startframe(0),maxframes(0),frameWidth(0),frameHeight(0),currentframe(0),painted(false),floor(floor),imgtex(0),texture(0),copied(false),sizeX(0),sizeY(0){
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
	alpha = 255;
    angle = 0;
	painted = true;
	return true;
}


bool CBObject::loadAnimObject(string file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount){
	if (imgtex) delete imgtex;
	imgtex = new sf::Image;
	if (!imgtex->LoadFromFile(file)) return false;

	imgtex->CreateMaskFromColor(sf::Color(0, 0, 0));

	if (texture) delete texture;
	texture = new sf::Texture;
	texture->LoadFromImage(*imgtex);

	if(fw > texture->GetWidth()) return false;
	if(fh > texture->GetHeight()) return false;
	if(framecount > (texture->GetWidth()/frameWidth)*(texture->GetHeight()/frameHeight)) return false;

	frameWidth = fw;
	frameHeight = fh;
	sizeX = frameWidth;
	sizeY = frameHeight;
	startframe = startf;
	maxframes = framecount;
	alpha = 255;
    angle = 0;
	painted = true;
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


void CBObject::ghostObject(uint8_t ab){
	alpha = ab;
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
    posY-=sin(angle / 180.0 * M_PI) * fwrd + sin((angle-90.0) / 180.0 * M_PI)*sdwrd;
}

void CBObject::translateObject(float hor, float ver, float depth){
	posX+=hor;
	posY+=ver;
}

void CBObject::turnObject(float speed){
	angle+=speed;
}

void CBObject::render(RenderTarget &target){
	if (visible && painted) {
		if (floor) {
            sprite.SetOrigin(0,0);
			//Drawing floor objects

            float snappedx=floorf(CBEnchanted::instance()->getCameraX()/texture->GetWidth());
            float snappedy=floorf(CBEnchanted::instance()->getCameraY()/texture->GetHeight());

            float ekax=-CBEnchanted::instance()->getCameraX()+snappedx*texture->GetWidth();
            float ekay=-CBEnchanted::instance()->getCameraY()+snappedy*texture->GetHeight();

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
		}
		else {
			sprite.SetPosition(posX, posY);
			if(maxframes!=0){
				int16_t copyY = (currentframe % texture->GetWidth());
				int16_t copyX = (currentframe / texture->GetHeight());
				sprite.SetTextureRect(sf::IntRect(copyX*frameWidth, copyY*frameHeight, frameWidth, frameHeight));
				sprite.SetOrigin(frameWidth*0.5,frameHeight*0.5);
			}
			else {
				sprite.SetOrigin(texture->GetWidth()*0.5,texture->GetHeight()*0.5);
			}

			sprite.SetRotation(-angle);
			if(alpha!=255)
				sprite.SetColor(sf::Color(255, 255, 255, alpha));
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

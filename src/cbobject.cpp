
#include "cbobject.h"

CBObject::CBObject(){
}

bool CBObject::load(string file)
{
	if (!imgtex.LoadFromFile(file)) return false;

	imgtex.CreateMaskFromColor(sf::Color(0, 0, 0));
	texture.LoadFromImage(imgtex);
	sprite.SetTexture(texture);
	frameWidth = 0;
	frameHeight = 0;
	startframe = 0;
	maxframes = 0;
	alpha = 100;
	angle = 0;
	return true;
}

<<<<<<< HEAD
bool Object::loadAnimObject(string file, uint16_t fw, uint16_t fh, uint16_t startf, uint16_t framecount){
	if (!imgtex.LoadFromFile(file)) return false;

	imgtex.CreateMaskFromColor(sf::Color(0, 0, 0));
	texture.LoadFromImage(imgtex);
	sprite.SetTexture(texture);

	if(fw > texture.GetWidth()) return false;
	if(fh > texture.GetHeight()) return false;
	if(framecount > (texture.GetWidth()/frameWidth)*(texture.GetHeight()/frameHeight)) return false;

	frameWidth = fw;
	frameHeight = fh;
	startframe = startf;
	maxframes = framecount;
	alpha = 100;
	angle = 0;
	return true;
}

void Object::paintObject(sf::Texture txt){
=======
void CBObject::paintObject(sf::Texture txt){
>>>>>>> 4b770b5b5a61a0537859e35dfd938b471d750ba8
	texture = txt;
	sprite.SetTexture(texture);
}

void CBObject::paintObject(CBObject &obj){
	texture = obj.texture;
	sprite.SetTexture(texture);
}


void CBObject::ghostObject(uint8_t ab){
	alpha = ab;
}

void CBObject::maskObject(uint8_t r, uint8_t g, uint8_t b){
	imgtex.CreateMaskFromColor(sf::Color(r, g, b));
	texture.LoadFromImage(imgtex);
	sprite.SetTexture(texture);
}

void CBObject::moveObject(float fwrd, float sdwrd){
	posX+=cos(angle / 180.0 * M_PI) * fwrd + cos((angle-90) / 180.0 * M_PI)*sdwrd;
	posX-=sin(angle / 180.0 * M_PI) * fwrd + sin((angle-90) / 180.0 * M_PI)*sdwrd;
}

void CBObject::translateObject(float hor, float ver, float depth){
	posX+=hor;
	posY+=ver;
}

void CBObject::turnObject(float speed){
	angle+=speed;
}

void CBObject::render(sf::RenderTarget &target){
	sprite.SetPosition(posX, posY);
	if(maxframes!=0){

		int16_t copyY = (currentframe % texture.GetWidth());
		int16_t copyX = (currentframe / texture.GetHeight());
	}
	sprite.Rotate(angle);
	if(alpha!=100)
		sprite.SetColor(sf::Color(255, 255, 255, alpha));
	target.Draw(sprite);
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

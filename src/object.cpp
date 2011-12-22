
#include "object.h"

Object::Object(string file){
        imgtex.LoadFromFile(file);
        imgtex.CreateMaskFromColor(sf::Color(0, 0, 0));
        texture.LoadFromImage(imgtex);
        sprite.SetTexture(texture);
        frameWidth = 0;
        frameHeight = 0;
        startframe = 0;
        maxframes = 0;
        alpha = 100;
        angle = 0;

}

void Object::paintObject(sf::Texture txt){
        texture = txt;
        sprite.SetTexture(texture);
}

void Object::paintObject(Object &obj){
        texture = obj.texture;
        sprite.SetTexture(texture);
}


void Object::ghostObject(uint8_t ab){
        alpha = ab;
}

void Object::maskObject(uint8_t r, uint8_t g, uint8_t b){
        imgtex.CreateMaskFromColor(sf::Color(r, g, b));
        texture.LoadFromImage(imgtex);
        sprite.SetTexture(texture);
}

void Object::moveObject(float fwrd, float sdwrd){
        posX+=cos(angle / 180.0 * 3.14159265358979323);
        posX-=sin(angle / 180.0 * 3.14159265358979323);
}

void Object::translateObject(float hor, float ver, float depth){
        posX+=hor;
        posY+=ver;
}

void Object::turnObject(float speed){
        angle+=speed;
}

void Object::render(sf::RenderTarget &target){
        sprite.SetPosition(posX, posY);
        sprite.Rotate(angle);
        if(alpha!=100)
                sprite.SetColor(sf::Color(255, 255, 255, alpha));
        target.Draw(sprite);
}

void Object::setPosition(float x, float y){
        posX = x;
        posY = y;
}

float Object::getX(){
        return posX;
}

float Object::getY(){
        return posY;
}

float Object::getAngle(){
        return angle;
}

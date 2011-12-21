#ifndef OBJECT_H
#define OBJECT_H
#include "precomp.h"
class Object{
public:
    Object();
    void paintObject(sf::Texture txt);
    void moveObject(float fwrd, float sdwrd);
    void translateObject(float hor, float ver, float depth);
    void turnObject(float speed);
    void render(sf::RenderTarget &target);
    void setPosition(float x, float y);
    float getX();
    float getY();
    float getAngle();
private:
    float x, y;
    float angle;
    sf::Sprite sprite;
};

#endif // OBJECT_H

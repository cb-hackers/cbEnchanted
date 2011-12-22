#ifndef RENDERTARGETPOINTER_H
#define RENDERTARGETPOINTER_H
#include "precomp.h"
class RenderTargetPointer
{
public:
    RenderTargetPointer(sf::RenderWindow *target);
    RenderTargetPointer(sf::RenderTexture *target);
    sf::RenderTarget *getTarget(){return target;}
    void setViewTo(bool drawtoworld);
    void draw(const sf::Drawable &d);
    void clear(const sf::Color &c);
    void display();
private:
    enum TargetType {
        RenderWindow,
        RenderTexture
    };

    sf::RenderTarget *target;
    TargetType targetType;
    bool drawToWorldViewOn;
};

#endif // RENDERTARGETPOINTER_H

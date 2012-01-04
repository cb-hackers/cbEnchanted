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
    void draw(const sf::Vertex* vertices, unsigned int vertexCount,sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
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

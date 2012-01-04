#include "rendertargetpointer.h"
#include "cbenchanted.h"

RenderTargetPointer::RenderTargetPointer(sf::RenderWindow *target): target(target),drawToWorldViewOn(false),targetType(RenderWindow) {
}
RenderTargetPointer::RenderTargetPointer(sf::RenderTexture *target): target(target),drawToWorldViewOn(false),targetType(RenderTexture) {
}


void RenderTargetPointer::setViewTo(bool drawtoworld) {
    if (drawToWorldViewOn == drawtoworld) return; //Useless to update view
    if (drawtoworld) {
        sf::View view;
        view.SetSize(target->GetWidth(),target->GetHeight());
        view.SetCenter(CBEnchanted::instance()->getCameraX(),CBEnchanted::instance()->getCameraY());
        view.SetRotation(180); //Flip y
        target->SetView(view);
        drawToWorldViewOn = true;
    }
    else {
        target->SetView(target->GetDefaultView());
        drawToWorldViewOn = false;
    }
}

void RenderTargetPointer::draw(const sf::Drawable &d) {
    switch(targetType) {
    case RenderWindow: static_cast<sf::RenderWindow*>(target)->Draw(d);break;
    case RenderTexture: static_cast<sf::RenderTexture*>(target)->Draw(d);break;
    }
}

void RenderTargetPointer::draw(const sf::Vertex *vertices, unsigned int vertexCount, sf::PrimitiveType type, const sf::RenderStates &states)
{
    switch(targetType) {
    case RenderWindow: static_cast<sf::RenderWindow*>(target)->Draw(vertices,vertexCount,type,states);break;
    case RenderTexture: static_cast<sf::RenderTexture*>(target)->Draw(vertices,vertexCount,type,states);break;
    }
}

void RenderTargetPointer::clear(const sf::Color &c)
{
    switch(targetType) {
    case RenderWindow: static_cast<sf::RenderWindow*>(target)->Clear(c);break;
    case RenderTexture: static_cast<sf::RenderTexture*>(target)->Clear(c);break;
    }
}

void RenderTargetPointer::display()
{
    switch(targetType) {
    case RenderWindow: static_cast<sf::RenderWindow*>(target)->Display();break;
    case RenderTexture: static_cast<sf::RenderTexture*>(target)->Display();break;
    }
}

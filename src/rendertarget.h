#ifndef RENDERTARGETPOINTER_H
#define RENDERTARGETPOINTER_H
#include "precomp.h"
class RenderTarget : public sf::NonCopyable
{
public:
    RenderTarget();
    sf::RenderTexture *getSurface(){return &target;}
    const sf::RenderTexture *getSurface()const{return &target;}
    void create(int w,int h);
    void create(const sf::Texture &texture);
    void setViewTo(bool drawtoworld);
    void draw(const sf::Drawable &d);
    void draw(const sf::Vertex* vertices, unsigned int vertexCount,sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
    void drawLine(float x1,float y1,float x2,float y2);
    void drawCircle(float cx,float cy,float r,bool fill);
    void drawBox(float x,float y,float w,float h,bool fill);
    void drawEllipse(float x,float y,float w,float h,bool fill);
    void drawRenderTarget(const RenderTarget &rt,float x,float y);
    void drawTexture(const sf::Texture &tex,float x, float y);
    void drawDot(float cx,float cy);
    void setColor(const sf::Color &c) {glColor4ub(c.r,c.g,c.b,c.a);}

    void clear(const sf::Color &c);
    void display() const;
    int width()const{return target.GetWidth();}
    int height()const{return target.GetHeight();}
    void setup();
    void enableOpenGLDrawMode();
    void enableSFMLDrawMode();
private:

    sf::RenderTexture target;
    sf::Image *lockBuffer;
    bool drawToWorldViewOn;
    bool openGLDrawMode;
    sf::RenderStates defaultRenderState;
    bool changedSinceDisplay;
};

#endif // RENDERTARGETPOINTER_H

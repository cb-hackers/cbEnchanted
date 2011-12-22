#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertargetpointer.h"
class CBImage : public sf::Drawable
{
public:
    CBImage();
    bool load(const string &path);
    void lock();
    void unlock();
    int32_t width()const{return renderTexture.GetWidth();}
    int32_t height()const{return renderTexture.GetHeight();}
    void setHotspot(float x,float y) {hotspotX = x;hotspotY = y;}
    RenderTargetPointer *getRenderTargetPointer(){return &renderTargetPointer;}
    void Render(sf::RenderTarget &target, sf::Renderer &renderer) const;
private:
    float hotspotX,hotspotY;
    sf::RenderTexture renderTexture;
    sf::Image *image;
    RenderTargetPointer renderTargetPointer;
};

#endif // CBIMAGE_H

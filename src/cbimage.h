#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertarget.h"
class CBImage : public sf::Drawable
{
public:
    CBImage();
    ~CBImage();
    bool load(const string &path);
    void lock();
    void unlock();
    int32_t width()const{return renderTexture.GetWidth();}
    int32_t height()const{return renderTexture.GetHeight();}
    void makeImage(int32_t w, int32_t h);
    void setHotspot(float x,float y) {hotspotX = x;hotspotY = y;}
    void setDrawPos(const sf::Vector2f &pos){drawingPos = pos;}
    RenderTarget *getRenderTarget(){return &renderTarget;}
    void Draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    float hotspotX,hotspotY;
    sf::RenderTexture renderTexture;
    sf::Image *image;
    RenderTarget renderTarget;
    sf::Vector2f drawingPos;
};

#endif // CBIMAGE_H

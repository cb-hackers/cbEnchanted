#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertarget.h"
class CBParticleEmitter;
class CBImage
{
public:
	CBImage();
	~CBImage();
	bool load(const string &path);
	void lock();
	void unlock();
	int32_t width()const{return renderTarget.width();}
	int32_t height()const{return renderTarget.height();}
	void makeImage(int32_t w, int32_t h);
	void setHotspot(float x,float y) {hotspotX = x;hotspotY = y;}
	RenderTarget *getRenderTarget(){return &renderTarget;}
	void draw(float x,float y,bool useMask);
	void maskImage(const sf::Color &c){maskColor = c;}
private:
	float hotspotX,hotspotY;
	RenderTarget renderTarget;
	sf::Color maskColor;
};

#endif // CBIMAGE_H

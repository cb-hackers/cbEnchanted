#ifndef CBIMAGE_H
#define CBIMAGE_H
#include "precomp.h"
#include "rendertarget.h"
class CBParticleEmitter;
class CBImage
{
public:
	friend class RenderTarget;
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
	void draw(float x,float y,int frame, bool useMask);
	void maskImage(const sf::Color &c){maskColor = c;}
	sf::Color getMaskColor()const{return maskColor;}
	void setAnimParams(int32_t frameW,int32_t frameH,int32_t begining,int32_t animL){frameWidth = frameW;frameHeight = frameH;animBegin = begining;animLength = animL;}
private:
	float hotspotX,hotspotY;
	RenderTarget renderTarget;
	sf::Color maskColor;
	int32_t frameWidth;
	int32_t frameHeight;
	int32_t animBegin;
	int32_t animLength;
};

#endif // CBIMAGE_H

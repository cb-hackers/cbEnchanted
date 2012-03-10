#ifndef RENDERTARGETPOINTER_H
#define RENDERTARGETPOINTER_H
#include "precomp.h"
#include "particle.h"
class CBImage;
class RenderTarget : public sf::NonCopyable
{
	public:
		enum DrawingMode {
			DM_Textures, //Vertex array, texcoord array, color array, textures
			DM_NoTextures
		};
		RenderTarget();
		~RenderTarget();
		sf::RenderTexture *getSurface(){return &target;}
		const sf::RenderTexture *getSurface()const{return &target;}
		void create(int w,int h);
		void create(const sf::Texture &texture);
		void setViewTo(bool drawtoworld,bool force = false);
		void draw(const sf::Drawable &d);
		void draw(const sf::Vertex* vertices, unsigned int vertexCount,sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
		void drawLine(float x1,float y1,float x2,float y2);
		void drawCircle(float cx,float cy,float r,bool fill);
		void drawBox(float x,float y,float w,float h,bool fill);
		void drawEllipse(float x,float y,float w,float h,bool fill);
		void drawRenderTarget(const RenderTarget &rt,float x,float y);
		void drawRenderTarget(const RenderTarget &rt,float x,float y,const sf::Color &mask);
		void drawRenderTarget(const RenderTarget &rt,sf::FloatRect pos,sf::FloatRect area,const sf::Color &mask);
		void drawRenderTarget(const RenderTarget &rt,sf::FloatRect pos,sf::FloatRect area);
		void drawTexture(const sf::Texture &tex,float x, float y);
		void drawDot(float cx,float cy);
		void setColor(const sf::Color &c) {target.SetActive();glColor4ub(c.r,c.g,c.b,c.a);}
		void drawParticles(CBImage *tex,const vector<Particle> &particles, int32_t particleLifeTime, int32_t animLength);

		int32_t getPixel(int x,int y);
		int32_t getPixel2(int x,int y);
		void putPixel(int x,int y,int pixel);
		void putPixel2(int x,int y,int pixel);
		void lock();
		void unlock();

		bool isDrawToWorldViewOn()const{return drawToWorldViewOn;}

		void clear(const sf::Color &c);
		static void init();
		void display() const;
		int width()const{return target.GetWidth();}
		int height()const{return target.GetHeight();}
		void setup();
		void setDrawingMode(DrawingMode mode);
		int32_t getId()const{return id;}
	private:

		sf::RenderTexture target;
		sf::Image *lockBuffer;
		bool drawToWorldViewOn;
		DrawingMode drawingMode;
		sf::RenderStates defaultRenderState;
		bool changedSinceDisplay;
		bool pointSmoothing;

		bool lineSmoothing;
		int32_t id;
};

#endif // RENDERTARGETPOINTER_H

#ifndef RENDERTARGET_H
#define RENDERTARGET_H
#include "precomp.h"
#include "particle.h"
class CBImage;
class RenderTarget
{
	public:
		enum Align {
			Left = 1 <<0,
			Right = 1 << 1,
			HCenter = Left | Right,
			Top = 1 << 2,
			Bottom = 1 << 3,
			VCenter = Top | Bottom,
			Center = HCenter | VCenter
		};

		RenderTarget();
		~RenderTarget();
		void create(int32_t w, int32_t h);
		bool load(const string &path);
		RenderTarget *clone();
		void copy(const RenderTarget * r);
		void setAsCurrent(bool force = 0);
		void useWorldCoords(bool t,bool force = false);
		void drawBox(float x,float y,float w,float h,bool fill,const ALLEGRO_COLOR &color);
		void drawEllipse(float x,float y,float w,float h,bool fill,const ALLEGRO_COLOR &color);
		void drawCircle(float cx,float cy,float r,bool fill,const ALLEGRO_COLOR &color);
		void drawDot(float x,float y,const ALLEGRO_COLOR &color);
		void drawLine(float x1,float y1,float x2,float y2,const ALLEGRO_COLOR &color);
		void drawBitmap(ALLEGRO_BITMAP *r,float x,float y);
		void drawBitmap(ALLEGRO_BITMAP *r,float x,float y,float w,float h);
		void drawBitmapRegion(ALLEGRO_BITMAP *r,float sx,float sy,float sw,float sh,float tx,float ty,float tw,float th);
		void drawBitmap(ALLEGRO_BITMAP *r,float x,float y,float rot);
		void drawBitmapRegion(ALLEGRO_BITMAP *r,float rx,float ry,float rw,float rh,float x,float y);

		void drawBitmapRegion(ALLEGRO_BITMAP *r,float sx,float sy,float sw,float sh,const ALLEGRO_COLOR &tint,float x,float y,float rot);
		void drawBitmap(ALLEGRO_BITMAP *r,float x,float y,float rot,const ALLEGRO_COLOR &tint);
		void drawParticles(CBImage *tex, const vector<Particle> &particles, int32_t particleLifeTime,int32_t animLength);
		void drawText(const ALLEGRO_FONT *font,const string&text,float x,float y,const ALLEGRO_COLOR &color,int flags = Left|Top);
		void putPixel(int32_t x,int32_t y, const ALLEGRO_COLOR &color);
		bool isDrawToWorldViewOn()const{return worldMatrixEnabled;}
		ALLEGRO_COLOR getPixel(int32_t x,int32_t y);
		void clear(const ALLEGRO_COLOR &c);
		void lock(){al_lock_bitmap(bitmap,ALLEGRO_LOCK_READWRITE,ALLEGRO_PIXEL_FORMAT_ANY);}
		void unlock(){al_unlock_bitmap(bitmap);}
		int32_t width()const{return al_get_bitmap_width(bitmap);}
		int32_t height()const{return al_get_bitmap_height(bitmap);}
		ALLEGRO_BITMAP *getBitmap(){return bitmap;}
		int32_t getId()const{return id;}
	private:
		bool worldMatrixEnabled;
		ALLEGRO_BITMAP *bitmap;
		int32_t id;
};


#endif // RENDERTARGET_H

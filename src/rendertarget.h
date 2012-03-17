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
		void setAsCurrent();
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
extern int32_t renderTargetIdCounter;
extern RenderTarget *bindRenderTarget;

inline RenderTarget::RenderTarget():
	bitmap(0),
	worldMatrixEnabled(false)
{
	id = ++renderTargetIdCounter;
}

inline RenderTarget::~RenderTarget() {
	if (bitmap) al_destroy_bitmap(bitmap);
}
inline void RenderTarget::create(int32_t w, int32_t h) {
	if (bitmap) al_destroy_bitmap(bitmap);
	bitmap = al_create_bitmap(w,h);
}

inline bool RenderTarget::load(const string &path) {
	if (bitmap) al_destroy_bitmap(bitmap);
	bitmap = al_load_bitmap(path.c_str());
	if (bitmap) return true;
	return false;
}

inline void RenderTarget::setAsCurrent() {
	if (bindRenderTarget != this) {al_set_target_bitmap(bitmap);bindRenderTarget = this;}
}

inline void RenderTarget::drawBox(float x, float y, float w, float h, bool fill,const ALLEGRO_COLOR &color) {
	setAsCurrent();
	if (fill) {
		al_draw_filled_rectangle(x,y,x+w,y+h,color);
	}
	else {
		al_draw_rectangle(x,y,x+w,y+h,color,1.0f);
	}
}

inline void RenderTarget::drawCircle(float cx, float cy, float r, bool fill, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	if (fill) {
		al_draw_filled_circle(cx,cy,r,color);
	}
	else {
		al_draw_circle(cx,cy,r,color,1.0f);
	}
}

inline void RenderTarget::drawEllipse(float x, float y, float w, float h, bool fill,const ALLEGRO_COLOR &color) {
	setAsCurrent();
	if (fill) {
		al_draw_filled_ellipse(x,y,w*0.5f,h*0.5f,color);
	}
	else {
		al_draw_ellipse(x,y,w*0.5f,h*0.5f,color,1.0f);
	}
}

inline void RenderTarget::drawDot(float x, float y, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	al_draw_pixel(x,y,color);
}

inline void RenderTarget::drawLine(float x1, float y1, float x2, float y2, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	al_draw_line(x1,y1,x2,y2,color,1.0f);
}

inline void RenderTarget::putPixel(int32_t x, int32_t y, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	al_put_pixel(x,y,color);
}
inline ALLEGRO_COLOR RenderTarget::getPixel(int32_t x, int32_t y) {
	return al_get_pixel(bitmap,x,y);
}

inline void RenderTarget::clear(const ALLEGRO_COLOR &c) {
	setAsCurrent();
	al_clear_to_color(c);
}

inline void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y) {
	setAsCurrent();
	al_draw_bitmap(r,x,y,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}

inline void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float w, float h) {
	drawBitmapRegion(r,0,0,al_get_bitmap_width(r),al_get_bitmap_height(r),x,y,w,h);
}

inline void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP*r, float sx, float sy, float sw, float sh, float tx, float ty, float tw, float th) {
	setAsCurrent();
	al_draw_scaled_bitmap(r,sx,sy,sw,sh,tx,ty,tw,th,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}

inline void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float rot) {
	setAsCurrent();
	al_draw_rotated_bitmap(r,(float)al_get_bitmap_width(r)*0.5f,(float)al_get_bitmap_height(r)*0.5f,x,y,rot,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}

inline void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float rx, float ry, float rw, float rh, float x, float y)
{
	al_draw_bitmap_region(r,rx,ry,rw,rh,x,y,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}

inline void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float sx, float sy, float sw, float sh, const ALLEGRO_COLOR &tint, float x, float y, float rot)
{
	setAsCurrent();
	al_draw_tinted_scaled_rotated_bitmap_region(r,sx,sy,sw,sh,tint,sw*0.5f,sh*0.5f,x,y,1.0f,1.0f,rot,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}

inline void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float rot, const ALLEGRO_COLOR &tint)
{
	setAsCurrent();
	al_draw_tinted_rotated_bitmap(r,tint,(float)al_get_bitmap_width(r)*0.5f,(float)al_get_bitmap_height(r)*0.5f,x,y,rot,worldMatrixEnabled ? ALLEGRO_FLIP_HORIZONTAL:0);
}


inline RenderTarget *RenderTarget::clone() {
	RenderTarget *ret = new RenderTarget;
	ret->bitmap = al_clone_bitmap(this->bitmap);
	return ret;
}

inline void RenderTarget::copy(const RenderTarget *r) {
	if (bitmap) al_destroy_bitmap(bitmap);
	bitmap=al_clone_bitmap(r->bitmap);
}

inline void RenderTarget::drawText(const ALLEGRO_FONT *font, const string &text, float x, float y, const ALLEGRO_COLOR &color,int flags) {
	setAsCurrent();
	if ((flags & Center) == Center) {
		al_draw_text(font,color,x,y,ALLEGRO_ALIGN_CENTRE,text.c_str());
		return;
	}
	if ((flags & VCenter)==VCenter) {
		y -= (float)al_get_font_ascent(font)*0.5f;
	}
	else if (flags & Bottom) {
		y -= al_get_font_ascent(font);
	}
	if ((flags & HCenter) == HCenter) {
		al_get_text_width(font,text.c_str());
		al_draw_text(font,color,x-al_get_text_width(font,text.c_str())*0.5f,y,0,text.c_str());
		return;
	}
	if (flags & Right) {
		al_draw_text(font,color,x,y,ALLEGRO_ALIGN_RIGHT,text.c_str());
		return;
	}
	al_draw_text(font,color,x,y,0,text.c_str());
}



#endif // RENDERTARGET_H

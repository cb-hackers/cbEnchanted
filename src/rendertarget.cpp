#include "gfxinterface.h"
#include "rendertarget.h"
#include "cbenchanted.h"
#include "cbimage.h"
#include "camerainterface.h"

int32_t renderTargetIdCounter = 0;
RenderTarget *bindRenderTarget = 0;

void RenderTarget::drawParticles(CBImage *tex, const vector<Particle> &particles, int32_t particleLifeTime, int32_t animLength) {
	ALLEGRO_BITMAP *bm = tex->getMaskedBitmap();
	if (animLength == 0 || tex->animLength == 0) {
		float w = tex->width();
		float h = tex->height();
		for (vector<Particle>::const_iterator i = particles.begin(); i != particles.end(); i++) {
			this->drawBitmap(
				bm,
				i->x - w * 0.5f - tex->hotspotX,
				i->y + h * 0.5f - tex->hotspotY
			);
		}
	}
	else {
		float pLifeTime = static_cast<float>(particleLifeTime);
		float drawAreaWidth = tex->frameWidth;
		float drawAreaHeight = tex->frameHeight;
		float frameAreaHeight = tex->frameHeight;
		float frameAreaWidth = tex->frameWidth;
		for (vector<Particle>::const_iterator i = particles.begin(); i != particles.end(); i++) {
			int32_t frame = ((float)i->lifeTime / pLifeTime) * animLength;
			if (frame >= animLength) {
				frame = animLength - 1;
			}
			frame += tex->animBegin;
			int32_t framesX = tex->width() / tex->frameWidth;
			int32_t framesY = tex->height() / tex->frameHeight;
			int32_t copyX = frame % framesX;
			int32_t copyY = (frame - copyX) / framesY;

			float frameAreaLeft = ((framesX-copyX-1)*tex->frameWidth);
			float frameAreaTop = (copyY * tex->frameWidth);
			float drawAreaLeft = i->x - tex->hotspotX;
			float drawAreaTop = i->y - tex->hotspotY;
			this->drawBitmapRegion(
				bm,
				frameAreaLeft,
				frameAreaTop,
				frameAreaWidth,
				frameAreaHeight,
				drawAreaLeft,
				drawAreaTop,
				drawAreaWidth,
				drawAreaHeight
			);
		}
	}
}

RenderTarget::RenderTarget():
	bitmap(0),
	worldCoordsEnabled(false),
	cbImg(NULL)
{
	id = ++renderTargetIdCounter;
}

RenderTarget::~RenderTarget() {
	if (bitmap) {
		al_destroy_bitmap(bitmap);
	}
}
void RenderTarget::create(int32_t w, int32_t h) {
	if (bindRenderTarget == this) {
		bindRenderTarget = 0;
	}
	if (bitmap) {
		al_destroy_bitmap(bitmap);
	}
	bitmap = al_create_bitmap(w,h);
}

void RenderTarget::create(ALLEGRO_BITMAP *bm) {
	if (bindRenderTarget == this) {
		bindRenderTarget = 0;
	}
	if (bitmap) {
		al_destroy_bitmap(bitmap);
	}
	bitmap = bm;
}

bool RenderTarget::load(const string &path) {
	if (bitmap) {
		al_destroy_bitmap(bitmap);
	}
	bitmap = al_load_bitmap(path.c_str());
	if (bitmap) {
		return true;
	}
	return false;
}

void RenderTarget::setAsCurrent(bool force) {
	if (bindRenderTarget != 0 && !force) {
		if (bindRenderTarget->bitmap == this->bitmap) {
			return;
		}
	}
	al_set_target_bitmap(this->bitmap);
	bindRenderTarget = this;
}

void RenderTarget::useWorldCoords(bool t) {
	if (t != worldCoordsEnabled || pixelPreciseWorldCoordsEnabled) {
		setAsCurrent();
		if (t) {
			al_use_transform(CBEnchanted::instance()->cameraInterface->getWorldTransform());
		}
		else {
			ALLEGRO_TRANSFORM t;
			al_identity_transform(&t);
			al_use_transform(&t);
		}
		worldCoordsEnabled = t;
		pixelPreciseWorldCoordsEnabled = false;
	}
}

void RenderTarget::usePixelPreciseWorldCoords() {
	if (!pixelPreciseWorldCoordsEnabled) {
		setAsCurrent();
		al_use_transform(CBEnchanted::instance()->cameraInterface->getPixelPreciseWorldTransform());
		pixelPreciseWorldCoordsEnabled = true;
	}
}

void RenderTarget::drawBox(float x, float y, float w, float h, bool fill,const ALLEGRO_COLOR &color) {
	setAsCurrent();
	convertCoords(x, y);
	if (fill) {
		al_draw_filled_rectangle(x, y, x+w, y+h, color);
	}
	else {
		al_draw_rectangle(x, y, x+w, y+h, color, CBEnchanted::instance()->gfxInterface->getLineWidth());
	}
}

void RenderTarget::drawCircle(float cx, float cy, float r, bool fill, const ALLEGRO_COLOR &color) {
	convertCoords(cx, cy);
	if (r <= 0) {
		return;
	}
	setAsCurrent();
	if (fill) {
		al_draw_filled_circle(cx, cy, r, color);
	}
	else {
		al_draw_circle(cx, cy, r, color, CBEnchanted::instance()->gfxInterface->getLineWidth());
	}
}

void RenderTarget::drawEllipse(float x, float y, float w, float h, bool fill,const ALLEGRO_COLOR &color) {
	convertCoords(x,y);
	if (w < 0 || h < 0) {
		return;
	}
	setAsCurrent();
	if (fill) {
		al_draw_filled_ellipse(x, y, w*0.5f, h*0.5f, color);
	}
	else {
		al_draw_ellipse(x, y, w*0.5f, h*0.5f, color, CBEnchanted::instance()->gfxInterface->getLineWidth());
	}
}

void RenderTarget::drawDot(float x, float y, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	convertCoords(x, y);
	al_draw_pixel(x, y, color);
}

void RenderTarget::drawLine(float x1, float y1, float x2, float y2, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	convertCoords(x1, y1);
	convertCoords(x2, y2);
	al_draw_line(x1, y1, x2, y2, color, CBEnchanted::instance()->gfxInterface->getLineWidth());
}

void RenderTarget::putPixel(int32_t x, int32_t y, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	al_put_pixel(x, y, color);
}

void RenderTarget::copyBox(RenderTarget *src, int32_t sx, int32_t sy, int32_t w, int32_t h, int32_t tx, int32_t ty) {
	setAsCurrent();
	int32_t a, b, c;
	al_get_blender(&a, &b, &c);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);

	if(src->getBitmap() == this->getBitmap()) {
		ALLEGRO_BITMAP *src_clone = al_clone_bitmap(src->getBitmap());
		al_draw_bitmap_region(src_clone, sx, sy, w, h, tx, ty, 0);
		al_destroy_bitmap(src_clone);
	}
	else {
		al_draw_bitmap_region(src->getBitmap(), sx, sy, w, h, tx, ty, 0);
	}

	al_set_blender(a, b, c);
}

void RenderTarget::resize(int32_t w, int32_t h) {
	ALLEGRO_BITMAP *newBitmap = al_create_bitmap(w,h);

	// Check if we have changed Smooth2D in between creating this rendertarget
	// and calling this function. If so, we need to clone this bitmap, because
	// bitmap resize operation flags have changed.
	ALLEGRO_BITMAP *bitmapToDrawResized;
	int bmFlags = al_get_bitmap_flags(this->bitmap);
	int32_t origW = this->width();
	int32_t origH = this->height();
	bool smooth2d = CBEnchanted::instance()->isSmooth2D();
	if ((!smooth2d && (bmFlags & ALLEGRO_MAG_LINEAR) == ALLEGRO_MAG_LINEAR) ||
		(smooth2d && (bmFlags & ALLEGRO_MAG_LINEAR) != ALLEGRO_MAG_LINEAR))
	{
		// We need to clone.
		bitmapToDrawResized = al_clone_bitmap(this->bitmap);
		al_destroy_bitmap(this->bitmap);
	}
	else {
		// Phew, no need to clone.
		bitmapToDrawResized = this->bitmap;
	}
	al_set_target_bitmap(newBitmap);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	int32_t a, b, c;
	al_get_blender(&a, &b, &c);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
	al_draw_scaled_bitmap(bitmapToDrawResized, 0, 0, origW, origH, 0, 0, w, h, 0);
	al_set_blender(a, b, c);
	al_destroy_bitmap(bitmapToDrawResized);
	this->bitmap = newBitmap;
	bindRenderTarget = this;
}

ALLEGRO_BITMAP *RenderTarget::swapBitmap(ALLEGRO_BITMAP *bm) {
	ALLEGRO_BITMAP *old(this->bitmap);
	if (bindRenderTarget == this) {
		bindRenderTarget = 0;
	}
	this->bitmap = bm;
	return old;
}

ALLEGRO_COLOR RenderTarget::getPixel(int32_t x, int32_t y) {
	return al_get_pixel(bitmap, x, y);
}

void RenderTarget::clear(const ALLEGRO_COLOR &c) {
	setAsCurrent();
	al_clear_to_color(c);
}

void RenderTarget::convertCoords(float &x, float &y) {
	if (worldCoordsEnabled) {
		y = -y;
	}
	else if (!CBEnchanted::instance()->isSmooth2D()) {
		x = int(x + 0.5f);
		y = int(y + 0.5f);
	}
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y) {
	setAsCurrent();
	convertCoords(x,y);
	al_draw_bitmap(r, x, y, 0);
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float w, float h) {
	convertCoords(x, y);
	drawBitmapRegion(r, 0, 0, al_get_bitmap_width(r), al_get_bitmap_height(r), x, y, w, h);
}

void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP*r, float sx, float sy, float sw, float sh, float tx, float ty, float tw, float th) {
	setAsCurrent();
	convertCoords(tx, ty);
	al_draw_scaled_bitmap(r, sx, sy, sw, sh, tx, ty, tw, th, 0);
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float rot) {
	setAsCurrent();
	convertCoords(x, y);
	al_draw_rotated_bitmap(
		r, al_get_bitmap_width(r) * 0.5f, al_get_bitmap_height(r) * 0.5f, x, y, rot, 0
	);
}

void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float rx, float ry, float rw, float rh, float x, float y) {
	setAsCurrent();
	convertCoords(x, y);
	al_draw_bitmap_region(r, rx, ry, rw, rh, x, y, 0);
}

void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float sx, float sy, float sw, float sh, const ALLEGRO_COLOR &tint, float x, float y, float rot) {
	setAsCurrent();
	convertCoords(x, y);
	al_draw_tinted_scaled_rotated_bitmap_region(r, sx, sy, sw, sh, tint, sw * 0.5f, sh * 0.5f, x, y, 1.0f, 1.0f, rot, 0);
}

void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float sx, float sy, float sw, float sh, const ALLEGRO_COLOR &tint, float dx, float dy, float xscale, float yscale, float rot) {
	setAsCurrent();
	convertCoords(dx, dy);
	al_draw_tinted_scaled_rotated_bitmap_region(r, sx, sy, sw, sh, tint, sw * 0.5, sh * 0.5, dx, dy, xscale, yscale, rot, 0);
}

void RenderTarget::drawBitmapRegion(ALLEGRO_BITMAP *r, float sx, float sy, float sw, float sh, float dx, float dy, float xscale, float yscale, float rot) {
	setAsCurrent();
	convertCoords(dx, dy);
	al_draw_tinted_scaled_rotated_bitmap_region(r, sx, sy, sw, sh, al_map_rgba_f(1, 1, 1, 1), sw * 0.5, sh * 0.5, dx, dy, xscale, yscale, rot, 0);
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float rot, const ALLEGRO_COLOR &tint) {
	setAsCurrent();
	convertCoords(x,y);
	al_draw_tinted_rotated_bitmap(
		r, tint, al_get_bitmap_width(r) * 0.5f, al_get_bitmap_height(r) * 0.5f, x, y, rot, 0
	);
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, float rot, float xscale, float yscale, const ALLEGRO_COLOR &tint) {
	setAsCurrent();
	convertCoords(x,y);
	al_draw_tinted_scaled_rotated_bitmap(r, tint, al_get_bitmap_width(r) * 0.5f, al_get_bitmap_height(r) * 0.5f, x, y, xscale, yscale, rot, 0);
}

void RenderTarget::drawBitmap(ALLEGRO_BITMAP *r, float x, float y, const ALLEGRO_COLOR &tint) {
	setAsCurrent();
	convertCoords(x, y);
	al_draw_tinted_bitmap(r, tint, x, y, 0);
}

void RenderTarget::drawBitmapMirrored(ALLEGRO_BITMAP *r,int flags) {
	setAsCurrent();
	al_draw_bitmap(r, 0, 0, flags);
}

RenderTarget *RenderTarget::clone() {
	RenderTarget *ret = new RenderTarget;
	ret->bitmap = al_clone_bitmap(this->bitmap);
	return ret;
}

void RenderTarget::copy(const RenderTarget *r) {
	if (bitmap) {
		al_destroy_bitmap(bitmap);
	}
	bitmap = al_clone_bitmap(r->bitmap);
}

void RenderTarget::drawText(const ALLEGRO_FONT *font, const ISString &text, float x, float y, const ALLEGRO_COLOR &color, int flags) {
	setAsCurrent();
	convertCoords(x, y);
	if ((flags & Center) == Center) {
		al_draw_text(font, color, x, y - al_get_font_ascent(font) * 0.5f, ALLEGRO_ALIGN_CENTRE, text.getUtf8Encoded().c_str());
		return;
	}
	if ((flags & VCenter) == VCenter) {
		y -= al_get_font_ascent(font) * 0.5f;
	}
	else if ((flags & Bottom) == Bottom) {
		y -= al_get_font_ascent(font);
	}
	if ((flags & HCenter) == HCenter) {
		al_draw_text(
			font,
			color,
			x,
			y,
			ALLEGRO_ALIGN_CENTRE,
			text.getUtf8Encoded().c_str()
		);
		return;
	}
	if ((flags & Right) == Right) {
		al_draw_text(font, color, x, y, ALLEGRO_ALIGN_RIGHT, text.getUtf8Encoded().c_str());
		return;
	}
	al_draw_text(font, color, x, y, 0, text.getUtf8Encoded().c_str());
}

void RenderTarget::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool fill, const ALLEGRO_COLOR &color) {
	setAsCurrent();
	convertCoords(x1, y1);
	convertCoords(x2, y2);
	convertCoords(x3, y3);
	if (fill) {
		al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, color);
	}
	else {
		al_draw_triangle(x1, y1, x2, y2, x3, y3, color, CBEnchanted::instance()->gfxInterface->getLineWidth());
	}
}

/** Changes to a new bitmap and destroys the current one */
void RenderTarget::changeBitmap(ALLEGRO_BITMAP *bm) {
	al_destroy_bitmap(this->bitmap);
	this->bitmap = bm;
	if (bindRenderTarget == this) {
		bindRenderTarget = 0;
	}
}

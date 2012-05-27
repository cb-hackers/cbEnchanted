#include "customfunctions.h"
#include "cbenchanted.h"
#include "errorsystem.h"
#include "precomp.h"
/** @addtogroup customfunctions
 * @{
 */

/** VesQ's awesome triangle drawing function */
void cbeTriangle(CBEnchanted *cb) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawDrawCommandToWorld());
	bool fill = cb->popValue().toInt();
	float thickness = cb->popValue().toFloat();
	float y3 = cb->popValue().toFloat();
	float x3 = cb->popValue().toFloat();
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	cb->getCurrentRenderTarget()->drawTriangle(
		x1,
		y1,
		x2,
		y2,
		x3,
		y3,
		thickness,
		fill,
		cb->getDrawColor()
	);
	cb->pushValue(0);
}

/** Sets draw color with alpha */
void cbeColor(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b = cb->popValue().toInt();
	int32_t g = cb->popValue().toInt();
	int32_t r = cb->popValue().toInt();

	// Validate values
	if (r < 0) r = 0;
	if (r > 255) r = 255;
	if (g < 0) g = 0;
	if (g > 255) g = 255;
	if (b < 0) b = 0;
	if (b > 255) b = 255;
	if (a < 0) a = 0;
	if (a > 255) a = 255;

	cb->setDrawColor(al_map_rgba(r,g,b,a));
	cb->pushValue(0);
}

/** Sets clear color with alpha */
void cbeClsColor(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b = cb->popValue().toInt();
	int32_t g = cb->popValue().toInt();
	int32_t r = cb->popValue().toInt();

	// Validate values
	if (r < 0) r = 0;
	if (r > 255) r = 255;
	if (g < 0) g = 0;
	if (g > 255) g = 255;
	if (b < 0) b = 0;
	if (b > 255) b = 255;
	if (a < 0) a = 0;
	if (a > 255) a = 255;

	cb->setClearColor(al_map_rgba(r, g, b, a));
	cb->pushValue(0);
}

/** Sets a custom blending mode. */
void cbeSetBlendMode(CBEnchanted *cb) {
	int32_t type = cb->popValue().toInt();

	switch (type) {
		case 1: // Additive
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
			break;
		case 2: // Erase / overwrite
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
			break;
		default:
			// Reset normal, make sure that this is the same is GfxInterface::initializeGfx()
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	}

	cb->pushValue(0);
}

/** Sets a custom blending mode in an advanced way.
 * Makes an error if given unsoppurted parameters. */
void cbeSetBlendModeAdvanced(CBEnchanted *cb) {
	int32_t cbDst = cb->popValue().toInt();
	int32_t cbSrc = cb->popValue().toInt();
	int32_t cbOp = cb->popValue().toInt();

	// Allegro versions
	int op, src, dst;

	switch (cbOp) {
		case 1: op = ALLEGRO_ADD; break;
		case 2: op = ALLEGRO_DEST_MINUS_SRC; break;
		case 3: op = ALLEGRO_SRC_MINUS_DEST; break;
		default:
			cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
				"First parameter was incorrect.");
			cb->pushValue(0);
		return;
	}

	switch (cbSrc) {
		case 1: src = ALLEGRO_ZERO; break;
		case 2: src = ALLEGRO_ONE; break;
		case 3: src = ALLEGRO_ALPHA; break;
		case 4: src = ALLEGRO_INVERSE_ALPHA; break;
		default:
			cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
				"Second parameter was incorrect.");
			cb->pushValue(0);
		return;
	}

	switch (cbDst) {
		case 1: dst = ALLEGRO_ZERO; break;
		case 2: dst = ALLEGRO_ONE; break;
		case 3: dst = ALLEGRO_ALPHA; break;
		case 4: dst = ALLEGRO_INVERSE_ALPHA; break;
		default:
			cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
				"Third parameter was incorrect.");
			cb->pushValue(0);
		return;
	}

	// Ok, every parameter is validated now and set properly. Set the blend mode.
	al_set_blender(op, src, dst);
	cb->pushValue(0);
}

void cbeDrawTintedScaledRotatedImageRegion(CBEnchanted *cb) {


	float angle = cb->popValue().toFloat();
	float scaley = cb->popValue().toFloat();
	float scalex = cb->popValue().toFloat();
	float dy = cb->popValue().toFloat();
	float dx = cb->popValue().toFloat();
	float sh = cb->popValue().toFloat();
	float sw = cb->popValue().toFloat();
	float sy = cb->popValue().toFloat();
	float sx = cb->popValue().toFloat();
	int32_t handle = cb->popValue().toInt();
	if (cb->getImage(handle) == NULL) {
		string err = "Image doesn't exist with id:" + boost::lexical_cast<string>(handle);
		cb->errors->createError("Image not found!", err, "Image not found!");
		return;
	}

	cb->getCurrentRenderTarget()->drawBitmapTintedScaledRegion(cb->getImage(handle)->getMaskedBitmap(), sx, sy, sw, sh, cb->getDrawColor(), dx, dy, scalex, scaley, angle);
	cb->pushValue(0);
}

/** @} */

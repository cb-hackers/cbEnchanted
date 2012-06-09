#include "customfunctions.h"
#include "cbenchanted.h"
#include "errorsystem.h"
#include "precomp.h"
#include "util.h"
#ifdef _WIN32
	#include <Windows.h>
#endif

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

/** Draws image region with scaling, rotating and tinting. */
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


void cbeBAnd(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b = cb->popValue().toInt();
	cb->pushValue(a & b);
}

void cbeBOr(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b = cb->popValue().toInt();
	cb->pushValue(a | b);
}

void cbeBXor(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	int32_t b = cb->popValue().toInt();
	cb->pushValue(a ^ b);
}

void cbeBNot(CBEnchanted *cb) {
	int32_t a = cb->popValue().toInt();
	cb->pushValue(~a);
}

/** Reads an UTF-8 encoded line and converts all possible characters to Windows-1252. */
void cbeReadLineUTF8(CBEnchanted *cb) {
	int32_t fileId = cb->popValue().getInt();
	FILE* file = cb->getFile(fileId);
	if (file == 0) {
		FIXME("Invalid file pointer given to cbeReadLineUTF8()")
		cb->pushValue(ISString(""));
		return;
	}
	string line = "";
	while(1) {
		int c = fgetc(file);
		if (c != '\r' && c != EOF) {
			if (c != '\n') {
				line = line + char(c);
			}
		}
		else {
			break;
		}
	}
	line += '\0';

	line = utf8toCP1252(line);
	cb->pushValue(line);
}

/** Shows or hides the console window. Works only on Windows. */
void cbeShowConsole(CBEnchanted *cb) {
	#ifdef _WIN32
		bool visible = cb->popValue().toBool();
		ShowWindow(GetConsoleWindow(), visible ? SW_RESTORE : SW_HIDE);
		cb->pushValue(0);
	#endif
}

/** Sets cursor to systems mouse cursor */
void cbeSetSystemCursor(CBEnchanted *cb) {
	int32_t t = cb->popValue().toInt();
	bool success = false;
	cb->setCustomCursor(0);
	cb->setCursorVisible(true);
	switch (t) {
		case 2: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW); break;
		case 3: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY); break;
		case 4: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_QUESTION); break;
		case 5: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT); break;
		case 6: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE); break;
		case 7: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N); break;
		case 8: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W); break;
		case 9: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_S); break;
		case 10: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E); break;
		case 11: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NW); break;
		case 12: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SW); break;
		case 13: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE); break;
		case 14: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE); break;
		case 15: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_PROGRESS); break;
		case 16: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION); break;
		case 17: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK); break;
		case 18: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT); break;
		case 19: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE); break;
		default: success = al_set_system_mouse_cursor(cb->getWindow(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT); break;
	}
	cb->pushValue((int32_t)success);
}

/** @} */

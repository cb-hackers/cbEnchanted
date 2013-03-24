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

#ifndef CBE_LIB
/** VesQ's awesome triangle drawing function */
void cbeTriangle(CBEnchanted *cb) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawDrawCommandToWorld() && !cb->drawingOnImage());
	bool fill = cb->popValue().toInt();
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
	bool alphaToo = cb->popValue().toBool();
	int32_t type = cb->popValue().toInt();

	if (alphaToo) {
		switch (type) {
			case 1: // Additive
				al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
				break;
			case 2: // Erase / overwrite
				al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
				break;
			default:
				// Reset normal, make sure that this is the same is GfxInterface::initializeGfx()
				al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
		}
	}
	else {
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
	}

	cb->pushValue(0);
}

/** Sets a custom blending mode in an advanced way.
 * Makes an error if given unsoppurted parameters. */
void cbeSetBlendModeAdvanced(CBEnchanted *cb) {
	int32_t cbADst = cb->popValue().toInt();
	int32_t cbASrc = cb->popValue().toInt();
	int32_t cbAOp = cb->popValue().toInt();
	int32_t cbDst = cb->popValue().toInt();
	int32_t cbSrc = cb->popValue().toInt();
	int32_t cbOp = cb->popValue().toInt();

	// Allegro versions
	int op, src, dst, aOp, aSrc, aDst;

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

	if (cbAOp == 0 && cbASrc == 0 && cbADst == 0) { // Alpha blending = regular blending
		aOp = op;
		aSrc = src;
		aDst = dst;
	}
	else { // Validate alpha parameters, too.
		switch (cbAOp) {
			case 1: aOp = ALLEGRO_ADD; break;
			case 2: aOp = ALLEGRO_DEST_MINUS_SRC; break;
			case 3: aOp = ALLEGRO_SRC_MINUS_DEST; break;
			default:
				cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
					"4th parameter was incorrect.");
				cb->pushValue(0);
			return;
		}

		switch (cbASrc) {
			case 1: aSrc = ALLEGRO_ZERO; break;
			case 2: aSrc = ALLEGRO_ONE; break;
			case 3: aSrc = ALLEGRO_ALPHA; break;
			case 4: aSrc = ALLEGRO_INVERSE_ALPHA; break;
			default:
				cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
					"5th parameter was incorrect.");
				cb->pushValue(0);
			return;
		}

		switch (cbADst) {
			case 1: aDst = ALLEGRO_ZERO; break;
			case 2: aDst = ALLEGRO_ONE; break;
			case 3: aDst = ALLEGRO_ALPHA; break;
			case 4: aDst = ALLEGRO_INVERSE_ALPHA; break;
			default:
				cb->errors->createError("cbeSetAdvancedBlendMode() failed!",
					"6th parameter was incorrect.");
				cb->pushValue(0);
			return;
		}
	}

	// Ok, every parameter is validated now and set properly. Set the blend mode.
	al_set_separate_blender(op, src, dst, aOp, aSrc, aDst);
	cb->pushValue(0);
}

/** Draws image region or frame with scaling, rotating and tinting. */
void cbeDrawTintedImage(CBEnchanted *cb) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld() && !cb->drawingOnImage());

	// Cropping parameters, source X and Y & width and height.
	// If sX is > 0, sY is a frame in an animated image.
	float sH = cb->popValue().toFloat();
	float sW = cb->popValue().toFloat();
	float sY = cb->popValue().toFloat();
	float sX = cb->popValue().toFloat();

	// Scaling factors
	float scaleY = cb->popValue().toFloat();
	float scaleX = cb->popValue().toFloat();

	// Angle
	float angle = cb->popValue().toFloat();
	angle = -(angle / 180.0) * M_PI;

	// Center drawing coordinates
	float centerY = cb->popValue().toFloat();
	float centerX = cb->popValue().toFloat();

	// Image ID
	int32_t handle = cb->popValue().toInt();
	CBImage *img = cb->getImage(handle);
	if (img == NULL) {
		string err = "Image with ID " + boost::lexical_cast<string>(handle) + " doesn't exist.";
		cb->errors->createError("cbeDrawTintedImage() failed!", err);
		cb->pushValue(0);
		return;
	}
	ALLEGRO_BITMAP *bm = 0;
	if (img->masked()) {
		bm = img->getMaskedBitmap();
	}
	else {
		bm = img->getUnmaskedBitmap();
	}

	// If sW and sH are 0, the image shall be drawn without any cropping.
	// But if sX >0, draw a single frame of the given image from sY parameter.
	if (sX != 0) {
		int32_t frameWidth = img->getFrameWidth();
		int32_t frameHeight = img->getFrameHeight();

		if (!frameWidth || !frameHeight) {
			cb->errors->createError("cbeDrawTintedImage() failed!", "Tried to draw a frame out of an image that was not loaded with LoadAnimImage()");
			cb->pushValue(0);
			return;
		}

		int32_t frame = (int)sY;
		int32_t framesX = img->width() / frameWidth;
		int32_t framesY = img->height() / frameHeight;
		int32_t copyX = frame % framesX;
		int32_t copyY = (frame - copyX) / framesY;

		float frameAreaLeft = (copyX * frameWidth);
		float frameAreaTop = (copyY * frameWidth);
		float frameAreaHeight = frameHeight;
		float frameAreaWidth = frameWidth;
		cb->getCurrentRenderTarget()->drawBitmapRegion(
			bm,
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			cb->getDrawColor(),
			centerX,
			centerY,
			scaleX,
			scaleY,
			angle
		);
	}
	else if (sW == 0 && sH == 0) {
		cb->getCurrentRenderTarget()->drawBitmap(bm, centerX, centerY, angle, scaleX, scaleY, cb->getDrawColor());
	}
	else {
		cb->getCurrentRenderTarget()->drawBitmapRegion(bm, sX, sY, sW, sH, cb->getDrawColor(), centerX, centerY, scaleX, scaleY, angle);
	}

	cb->pushValue(0);
}

/** Draws image region or frame with scaling and rotating. */
void cbeDrawImage(CBEnchanted *cb) {
	cb->getCurrentRenderTarget()->useWorldCoords(cb->getDrawImageToWorld() && !cb->drawingOnImage());

	// Cropping parameters, source X and Y & width and height.
	// If sX is > 0, sY is a frame in an animated image.
	float sH = cb->popValue().toFloat();
	float sW = cb->popValue().toFloat();
	float sY = cb->popValue().toFloat();
	float sX = cb->popValue().toFloat();

	// Scaling factors
	float scaleY = cb->popValue().toFloat();
	float scaleX = cb->popValue().toFloat();

	// Angle
	float angle = cb->popValue().toFloat();
	angle = -(angle / 180.0) * M_PI;

	// Center drawing coordinates
	float centerY = cb->popValue().toFloat();
	float centerX = cb->popValue().toFloat();

	// Image ID
	int32_t handle = cb->popValue().toInt();
	CBImage *img = cb->getImage(handle);
	if (img == NULL) {
		string err = "Image with ID " + boost::lexical_cast<string>(handle) + " doesn't exist.";
		cb->errors->createError("cbeDrawImage() failed!", err);
		cb->pushValue(0);
		return;
	}
	ALLEGRO_BITMAP *bm = 0;
	if (img->masked()) {
		bm = img->getMaskedBitmap();
	}
	else {
		bm = img->getUnmaskedBitmap();
	}

	// If sW and sH are 0, the image shall be drawn without any cropping.
	// But if sX >0, draw a single frame of the given image from sY parameter.
	if (sX > 0) {
		int32_t frameWidth = img->getFrameWidth();
		int32_t frameHeight = img->getFrameHeight();

		if (!frameWidth || !frameHeight) {
			cb->errors->createError("cbeDrawImage() failed!", "Tried to draw a frame out of an image that was not loaded with LoadAnimImage()");
			cb->pushValue(0);
			return;
		}

		int32_t frame = (int)sY;
		int32_t framesX = img->width() / frameWidth;
		int32_t framesY = img->height() / frameHeight;
		int32_t copyX = frame % framesX;
		int32_t copyY = (frame - copyX) / framesY;

		float frameAreaLeft = (copyX * frameWidth);
		float frameAreaTop = (copyY * frameWidth);
		float frameAreaHeight = frameHeight;
		float frameAreaWidth = frameWidth;
		cb->getCurrentRenderTarget()->drawBitmapRegion(
			bm,
			frameAreaLeft,
			frameAreaTop,
			frameAreaWidth,
			frameAreaHeight,
			centerX,
			centerY,
			scaleX,
			scaleY,
			angle
		);
	}
	else if (sW == 0 && sH == 0) {
		cb->getCurrentRenderTarget()->drawBitmap(bm, centerX, centerY, angle, scaleX, scaleY, al_map_rgba_f(1, 1, 1, 1));
	}
	else {
		cb->getCurrentRenderTarget()->drawBitmapRegion(bm, sX, sY, sW, sH, centerX, centerY, scaleX, scaleY, angle);
	}

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

/** Applies a scale, rotate and translate transformation to all future drawing operations. */
void cbeTransformBuild(CBEnchanted *cb) {
	float angle = (cb->popValue().toFloat() / 180.0f) * M_PI;
	float scaleY = cb->popValue().toFloat();
	float scaleX = cb->popValue().toFloat();
	float transY = cb->popValue().toFloat();
	float transX = cb->popValue().toFloat();

	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_build_transform(&t, transX, transY, scaleX, scaleY, angle);
	al_use_transform(&t);
	cb->pushValue(0);
}

/** Applies a rotation transformation to the current transformation. */
void cbeTransformRotate(CBEnchanted *cb) {
	float angle = (cb->popValue().toFloat() / 180.0f) * M_PI;

	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_rotate_transform(&t, angle);
	al_use_transform(&t);
	cb->pushValue(0);
}

/** Applies a scale transformation to the current transformation. */
void cbeTransformScale(CBEnchanted *cb) {
	float scaleY = cb->popValue().toFloat();
	float scaleX = cb->popValue().toFloat();

	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_scale_transform(&t, scaleX, scaleY);
	al_use_transform(&t);
	cb->pushValue(0);
}

/** Applies a translate transformation to the current transformation. */
void cbeTransformTranslate(CBEnchanted *cb) {
	float transY = cb->popValue().toFloat();
	float transX = cb->popValue().toFloat();

	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_translate_transform(&t, transX, transY);
	al_use_transform(&t);
	cb->pushValue(0);
}

/** Resets the identity transformation. */
void cbeTransformReset(CBEnchanted *cb) {
	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_use_transform(&t);
	cb->pushValue(0);
}

/** Returns camera's zoom value */
void cbeCameraZoom(CBEnchanted *cb) {
	cb->pushValue(cb->getCameraZoom());
}

/** Returns camera's real angle */
void cbeCameraAngle(CBEnchanted *cb) {
	cb->pushValue(cb->getCameraRealAngle());
}

/** Sets line width for the commandLine and the outlines of the shapes.*/
void cbeSetLineWidth(CBEnchanted *cb) {
	cb->setLineWidth(cb->popValue().toFloat());
	cb->pushValue(0);
}

/** Creates filedialog and returns filepath(s) on succes, empty string on failure.*/
void cbeFileDialog(CBEnchanted * cb) {
	int mode = cb->popValue().getInt();
	string patterns = cb->popValue().toString().getRef();
#ifdef _WIN32
	string title = cb->popValue().toString().getRef();
#else
	string title = cb->popValue().toString().getUtf8Encoded();
#endif
	ALLEGRO_PATH * path = cb->popValue().toString().getPath();

	ALLEGRO_FILECHOOSER * fC = al_create_native_file_dialog(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), title.c_str(), patterns.c_str(), mode);
	al_destroy_path(path);

	if(fC == NULL) {
		cb->errors->createError("Can't create FileDialog!");
		cb->pushValue(0);
		return;
	}
	bool retVal = al_show_native_file_dialog(cb->getWindow(), fC);

	if(retVal) {
		string filePaths = "";
		for(int count = 0; count < al_get_native_file_dialog_count(fC); count++) {
			filePaths += string(al_get_native_file_dialog_path(fC, count)) + string("|");
		}
		cb->pushValue(filePaths.substr(0, filePaths.length()-1));
	} else {
		cb->pushValue(string(""));
	}

	al_destroy_native_file_dialog(fC);

}
/** Converts the world coordinates to the screen coordinates */
void cbeWToSX(CBEnchanted *cb) {
	float x = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	cb->worldCoordToScreen(x, y);
	cb->pushValue(x);
}

/** Converts the world coordinates to the screen coordinates */
void cbeWToSY(CBEnchanted *cb) {
	float x = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	cb->worldCoordToScreen(x, y);
	cb->pushValue(y);
}

/** Converts the screen coordinates to the world coordinates */
void cbeSToWX(CBEnchanted *cb) {
	float x = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	cb->screenCoordToWorld(x, y);
	cb->pushValue(x);
}

/** Converts the screen coordinates to the world coordinates */
void cbeSToWY(CBEnchanted *cb) {
	float x = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	cb->screenCoordToWorld(x, y);
	cb->pushValue(y);
}

/** Returns the object's position in the screen coordinates */
void cbeObjectSX(CBEnchanted *cb) {
	int32_t id = cb->popValue().toInt();
	CBObject *obj = cb->getObject(id);
	float x = obj->getX();
	float y = obj->getY();
	cb->worldCoordToScreen(x, y);
	cb->pushValue(x);
}

/** Returns the object's position in the screen coordinates */
void cbeObjectSY(CBEnchanted *cb) {
	int32_t id = cb->popValue().toInt();
	CBObject *obj = cb->getObject(id);
	float x = obj->getX();
	float y = obj->getY();
	cb->worldCoordToScreen(x, y);
	cb->pushValue(y);
}

void cbeLinkCustomFunctions(CBEnchanted *cb) {
	cb->getCustomFunctionHandler()->link();
	cb->pushValue(0);
}

void cbeLoadLibrary(CBEnchanted *cb) {
	bool t = cb->popValue().toBool();
	typedef void (*cbeInitializeLibraryFuncType)(CBEnchanted *);
	const ISString &path = cb->popValue().toString();
	void * dll = loadDLL(path);
	if (!dll) {
		cb->errors->createError("Cannot open dll file " + path.getStdString());
		cb->pushValue(0);
		return;
	}
	cbeInitializeLibraryFuncType cbeInitializeLibrary = (cbeInitializeLibraryFuncType)getDLLFunction(dll, "cbeInitializeLibrary");
	if (!cbeInitializeLibrary) {
		cb->errors->createError("Cannot find cbeInitilizeLibrary function from " + path.getStdString());
		cb->pushValue(0);
		return;
	}
	cbeInitializeLibrary(cb);
	if (t) cb->getCustomFunctionHandler()->link();
	cb->pushValue(1);
}


/** Get fullscreen display modes count
 * @returns List size
 */
void cbeGetGfxModesCount(CBEnchanted *cb) {
	// Store the old flags for later restoring
	int32_t oldFlags = al_get_display_flags(cb->getWindow());

	// Set the display flags for fullscreen
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);

	// Get display modes count
	int32_t displayModesCount = al_get_num_display_modes();

	// Restore old flags
	al_set_new_display_flags(oldFlags);

	// Return the modes count
	cb->pushValue(displayModesCount);
}

/** Returns the fullscreen display mode in ID as string
 * @param id Id of the display mode. MUST BE in range of 0 to cbeGetGfxModesCount()-1.
 * @returns Display mode as string. I.e. cbeGetGfxMode(0) returns a string "640,480,60,32" (in most cases).
 */
void cbeGetGfxMode(CBEnchanted *cb) {
	// Store the old flags for later restoring
	int32_t oldFlags = al_get_display_flags(cb->getWindow());

	// Set the display flags for fullscreen
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);

	// Get display modes count
	int32_t displayModesCount = al_get_num_display_modes();

	// Pop the ID from input
	int32_t displayId = cb->popValue().toInt();

	// Check if displayId is valid
	if (displayId < 0 || displayId >= displayModesCount) {
		string id = boost::lexical_cast<string>(displayId);
		string count = boost::lexical_cast<string>(displayModesCount);
		bool ignore = cb->errors->createError("cbeGetGfxMode() failed!", "Trying to get gfx mode with ID " + id + " out of " + count + " modes\nWhen ignored, the first display mode available in the list is returned.");

		// If ignored, displayId is 0. Otherwise push empty string and return.
		if (ignore) {
			displayId = 0;
		}
		else {
			cb->pushValue(ISString(""));
			return;
		}
	}

	// Where the display modes data is stored
	ALLEGRO_DISPLAY_MODE *displayData = new ALLEGRO_DISPLAY_MODE;

	// Get the display mode with id
	al_get_display_mode(displayId, displayData);

	// Restore old flags
	al_set_new_display_flags(oldFlags);

	if (displayData != NULL) {
		// Initialize the string to be returned
		string displayModeString;

		// Construct the string
		displayModeString =	boost::lexical_cast<string>(displayData->width);
		displayModeString += "," + boost::lexical_cast<string>(displayData->height);
		displayModeString += "," + boost::lexical_cast<string>(displayData->refresh_rate);
		displayModeString += "," + boost::lexical_cast<string>(al_get_pixel_format_bits(displayData->format));

		// Return the display mode
		cb->pushValue(displayModeString.substr(0 , displayModeString.length() ));
	}
	else {
		INFO("Something funny happened in cbeGetGfxMode(), you got an empty display mode...")
		cb->pushValue(ISString(""));
	}

	// Free memory
	delete displayData;
}

void cbeGetBestGfxMode(CBEnchanted *cb) {

	// Initials
	int32_t bestHertz = 0;
	int32_t bestDisplayId = -1;
	bool no32BitFound = true;

	// Store the old flags for later restoring
	int32_t oldFlags = al_get_display_flags(cb->getWindow());

	// Set the display flags for fullscreen
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);

	// Get display modes count
	int32_t displayModesCount = al_get_num_display_modes();

	// Pop the input
	int32_t displayHeight = cb->popValue().getInt();
	int32_t displayWidth = cb->popValue().getInt();

	// Where the display modes data is stored
	ALLEGRO_DISPLAY_MODE *displayData = new ALLEGRO_DISPLAY_MODE;
	// Initialize the string to be returned
	string displayModeString;

	for (int currentId = 0; currentId < displayModesCount; currentId++) {
		// Get the display mode with id
		al_get_display_mode(currentId, displayData);

		if (displayData != NULL) {
			if (displayData->width == displayWidth && displayData->height == displayHeight && displayData->refresh_rate >= bestHertz) {
				if (al_get_pixel_format_bits(displayData->format) >= 32) {
					no32BitFound = false;
				}
				bestHertz = displayData->refresh_rate;
				bestDisplayId = currentId;
			}
		}
	}

	// If we got something, get that display mode and return it
	if (bestDisplayId > -1) {
		al_get_display_mode(bestDisplayId, displayData);
		// Construct the string
		displayModeString =	boost::lexical_cast<string>(displayData->width);
		displayModeString += "," + boost::lexical_cast<string>(displayData->height);
		displayModeString += "," + boost::lexical_cast<string>(displayData->refresh_rate);
		if (no32BitFound) {
			displayModeString += ",16";
		}
		else {
			displayModeString += ",32";
		}
	}
	else {
		displayModeString = "";
	}

	// Restore old flags
	al_set_new_display_flags(oldFlags);
	// Return the display mode
	cb->pushValue(displayModeString.substr(0 , displayModeString.length() ));

}

void cbeMessageBox(CBEnchanted *cb) {
	int32_t flags = cb->popValue().getInt();
	string buttons = cb->popValue().getString().getRef();
	string txt = cb->popValue().getString().getUtf8Encoded();
	string heading = cb->popValue().getString().getUtf8Encoded();
	string title = cb->popValue().getString().getUtf8Encoded();

	const char * buttons_cstr;

	if (buttons == "") {
		buttons_cstr = NULL;
	} else {
		buttons_cstr = buttons.c_str();
	}

	cb->pushValue(al_show_native_message_box(cb->getWindow(), title.c_str(), heading.c_str(), txt.c_str(), buttons_cstr, flags));
}

void cbePushByte(CBEnchanted *cb) {
	cb->pushValue(0);
}

void cbePushShort(CBEnchanted *cb) {
	cb->pushValue(0);
}


void cbePushInteger(CBEnchanted *cb) {
	cb->pushValue(0);
}

void cbePushFloat(CBEnchanted *cb) {
	cb->pushValue(0);
}

void cbePushString(CBEnchanted *cb) {
	cb->pushValue(0);
}

void cbePopByte(CBEnchanted *cb) {

}

void cbePopShort(CBEnchanted *cb) {

}

void cbePopInteger(CBEnchanted *cb) {

}

void cbePopFloat(CBEnchanted *cb) {

}

void cbePopString(CBEnchanted *cb) {

}

#endif
/** @} */

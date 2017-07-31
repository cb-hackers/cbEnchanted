#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "objectinterface.h"
#include "camerainterface.h"
#include "textinterface.h"
#include "inputinterface.h"
#include "soundinterface.h"
#include "sysinterface.h"
#include "imageinterface.h"
#include "animinterface.h"
#include "sysinterface.h"
#include "cbimage.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include "util.h"
#include "errorsystem.h"
#include <iostream>


const char *screenGammaFragmentShaderCode =
		"uniform sampler2D screenBuf; \n"
		"uniform vec4 windowGamma; \n"
		"void main(void) \n"
		"{ \n"
		"	vec4 color = texture2D(screenBuf, gl_TexCoord[0].xy); \n"
		"	vec4 nyCol; \n"
		"	if (windowGamma.r > 0) nyCol.r = color.r + (1.0 - color.r) * windowGamma.r; else nyCol.r = color.r + color.r * windowGamma.r; \n"
		"	if (windowGamma.g > 0) nyCol.g = color.g + (1.0 - color.g) * windowGamma.g; else nyCol.g = color.g + color.g * windowGamma.g; \n"
		"	if (windowGamma.b > 0) nyCol.b = color.b + (1.0 - color.b) * windowGamma.b; else nyCol.b = color.b + color.b * windowGamma.b; \n"
		"	if (nyCol.r > 1.0)  nyCol.r = 1.0; \n"
		"	if (nyCol.r < 0.0)  nyCol.r = 0.0; \n"
		"	if (nyCol.g > 1.0)  nyCol.g = 1.0; \n"
		"	if (nyCol.g < 0.0)  nyCol.g = 0.0; \n"
		"	if (nyCol.b > 1.0)  nyCol.b = 1.0; \n"
		"	if (nyCol.b < 0.0)  nyCol.b = 0.0; \n"
		"	nyCol.a = 1.0; \n"
		"	gl_FragColor = nyCol; \n"
		"} \n";

#ifndef CBE_LIB
GfxInterface::GfxInterface() :
	windowTitle(""),
	window(0),
	drawDrawCommandToWorld(false),
	drawImageToWorld(false),
	drawTextToWorld(false),
	gameDrawn(false),
	gameUpdated(false),
	lineWidth(1.0f),
	imageToDrawTo(NULL)
{
	cb = CBEnchanted::instance();
	fpsCounter = 0;
	currentFPS = 0;
	lastSecTimer = clock();
	lastFrameTime = mtimer();
}

GfxInterface::~GfxInterface() {
}

bool GfxInterface::initializeGfx() {
	drawColor = al_map_rgb(255, 255, 255);
	clearColor = al_map_rgb(0, 0, 0);

	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 0, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 1, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_COMPATIBLE_DISPLAY, 1, ALLEGRO_REQUIRE);
	window = al_create_display(400, 300);
	windowMode = WindowMode::Windowed;
	defaultWidth = 400;
	defaultHeight = 300;
	newWidth = defaultWidth;
	newHeight = defaultHeight;
	defaultAspectRatio = (float)defaultWidth / (float)defaultHeight;

	if (!window) {
		cb->errors->createFatalError("Can't create window","Can't create default window.");
		return false;
	}

	//Register event source
	registerWindow();
	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);

	//Screen is not resizable
	resizableWindow = false;

	al_set_new_bitmap_flags(ALLEGRO_NO_PREMULTIPLIED_ALPHA);
	// If you change the blenders below, make sure to change them from the default case
	// in cbeSetBlendMode() in customfunctions.cpp, too.
	al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);


	windowRenderTarget = new RenderTarget;
	windowRenderTarget->create(al_get_backbuffer(window));
	windowRenderTarget->clear(clearColor);
	bufferMap[windowRenderTarget->getId()] = windowRenderTarget;

	drawscreenTempBitmap = al_create_bitmap(400, 300);

	al_init_image_addon();
	al_init_primitives_addon();

	currentRenderTarget = windowRenderTarget;
	windowGammaR = 0;
	windowGammaG = 0;
	windowGammaB = 0;

	return true;
}

void GfxInterface::commandScreen(void) {

	//TODO use al_resize_window
	int mode = cb->popValue().toInt();
	uint32_t depth = cb->popValue().toInt();
	uint32_t height = cb->popValue().toInt();
	uint32_t width = cb->popValue().toInt();

	defaultWidth = width;
	defaultHeight = height;
	uint32_t flags;
	
	if (depth == 0) {
		depth = 32;
	}
	
	switch (mode) {
		case 0: windowMode = WindowMode::FullScreen; break;
		case 1: windowMode = WindowMode::Windowed; break;
		case 2: windowMode = WindowMode::Resizeable; break;
		case 3: windowMode = WindowMode::LockedAspect; break;
	}
	
	switch (windowMode) {
		case WindowMode::FullScreen: //cbFullscreen
			flags = ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN;
			break;
		case WindowMode::Windowed: // default
			flags = ALLEGRO_OPENGL | ALLEGRO_WINDOWED;
			break;
		case WindowMode::Resizeable: //cbSizable
			flags = ALLEGRO_OPENGL | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE;
			break;
		case WindowMode::LockedAspect: //cbeSizable, aspect ratio will be always the same.
			flags = ALLEGRO_OPENGL | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE;
			newWidth = defaultWidth;
			newHeight = defaultHeight;
			defaultAspectRatio = (float)width / (float)height;
			break;
	}
	
	if ((al_get_display_flags(window) & flags) == flags) {

		al_resize_display(window, width, height);


		resizeTempBitmap(width, height);
	}
	else {
		al_set_new_display_flags(flags);
		unregisterWindow();
		int windowW = al_get_display_width(window);
		int windowH = al_get_display_height(window);
		al_destroy_display(window);

		if (windowMode == WindowMode::Resizeable) {
			window = al_create_display(windowW, windowH);
		}
		else {
			window = al_create_display(width, height);
		}
		
		if (window == 0) {
			if (cb->isSmooth2D()) {
				cb->errors->createError("Can't create window", "Creating window failed in command Screen.\nIf you try to continue, Smooth2D will be toggled off.");
				cb->setSmooth2D(false);
				if (windowMode == WindowMode::Resizeable) {
					window = al_create_display(al_get_display_width(window), al_get_display_height(window));
				}
				else {
					window = al_create_display(width, height);
				}
				if (window == 0) {
					cb->errors->createFatalError("Can't create window", "Creating window failed in command Screen, even when Smooth2D was unset.");
					return;
				}
			}
			else {
				cb->errors->createFatalError("Can't create window", "Creating window failed in command Screen.");
				return;
			}
			return;
		}
		
		resizeTempBitmap(width, height);
		if (windowMode == WindowMode::Resizeable || windowMode == WindowMode::LockedAspect) {
			windowRenderTarget->swapBitmap(drawscreenTempBitmap);
			resizableWindow = true;
		}
		else {
			windowRenderTarget->swapBitmap(al_get_backbuffer(window));
			resizableWindow = false;
		}

		registerWindow();
	}

	if (windowMode != WindowMode::FullScreen) {
		// Center the window to the first display, if not fullscreen
		ALLEGRO_MONITOR_INFO displayInfo;
		if (!al_get_monitor_info(0, &displayInfo)) {
			// Couln't get display info, just output error to cerr and continue as usual.
			cerr << "ERROR: Can't center the window - could not get monitor info." << endl;
		}
		else {
			int topleftX = ((displayInfo.x2 - displayInfo.x1) - al_get_display_width(window)) / 2;
			int topleftY = ((displayInfo.y2 - displayInfo.y1) - al_get_display_height(window)) / 2;
			al_set_window_position(window, topleftX, topleftY);
		}
	}
	windowRenderTarget->clear(al_map_rgb(0, 0, 0));
	al_flip_display();
}

void GfxInterface::commandClsColor(void) {
	int b = cb->popValue().toInt();
	int g = cb->popValue().toInt();
	int r = cb->popValue().toInt();
	clearColor = al_map_rgb(r, g, b);
}

void GfxInterface::commandColor(void) {
	float b = cb->popValue().toInt();
	float g = cb->popValue().toInt();
	float r = cb->popValue().toInt();
	drawColor = al_map_rgb(r, g, b);
}

void GfxInterface::commandCircle(void) {
	bool dw = drawDrawCommandToWorld && !drawingOnImage();
	currentRenderTarget->useWorldCoords(dw);
	bool fill = cb->popValue().toInt();
	float r = cb->popValue().toFloat() * 0.5;
	float cy = cb->popValue().toFloat() + (dw ? -r : r);
	float cx = cb->popValue().toFloat() + r;
	currentRenderTarget->drawCircle(cx, cy, r, fill, drawColor);
}

void GfxInterface::commandLine(void){
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();
	if (cb->isSmooth2D() || currentRenderTarget->isDrawToWorldViewOn()) {
		x1 += 0.5f;
		y1 += 0.5f;
		x2 += 0.5f;
		y2 += 0.5f;
	}
	else {
		if (x1 == x2) {
			x1 += 0.5f;
			x2 += 0.5f;
			if (y1 < y2) {
				y2 += 1.0f;
			}
			else {
				y1 += 1.0f;
			}
		}
		else if (y1 == y2) {
			y1 += 0.5;
			y2 += 0.5;
			if (x1 < x2) {
				x2 += 1.0f;
			}
			else {
				x1 += 1.0f;
			}
		}
		else {
			if (y1 < y2) {
				y2 += 1.0f;
			}
			else {
				y1 += 1.0f;
			}
			if (x1 < x2) {
				x2 += 1.0f;
			}
			else {
				x1 += 1.0f;
			}
		}
	}

	currentRenderTarget->drawLine(x1, y1, x2, y2, drawColor);
}

void GfxInterface::commandDrawScreen(void) {
	bool vSync = cb->popValue().toBool();
	bool cls = cb->popValue().toInt();

	if ((mtimer() - lastFrameTime) <= 1000.0 / cb->getFrameLimit()) {
		al_rest(((1000.0 / cb->getFrameLimit()) - (mtimer() - lastFrameTime)) / 1000.0);
	}
	lastFrameTime = mtimer();

	if (!gameUpdated) {
		callUpdateGameCallbacks();
		cb->objectInterface->updateObjects();
	}
	if (cb->cameraInterface->isCamFollowing()) {
		cb->cameraInterface->updateCamFollow();
	}
	if (!gameDrawn) {
		callDrawGameCallbacks();
		cb->objectInterface->drawObjects(*windowRenderTarget);
	}

	gameUpdated = false;
	gameDrawn = false;

	cb->inputInterface->preEventLoopUpdate();
	ALLEGRO_EVENT e;
	bool windowResized = false;
	while (al_get_next_event(cb->getEventQueue(), &e)) {
		if (cb->inputInterface->handleKeyboardEvent(&e)) {
			return;
		}
		cb->inputInterface->handleMouseEvent(&e);
		switch (e.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->sysInterface->askForExit()) {
					cb->stop();
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE: {
				windowResized = true;
			}
			break;
			case ALLEGRO_EVENT_VIDEO_FRAME_SHOW:
				cb->animInterface->acquireNewFrame();
			break;
			case ALLEGRO_EVENT_VIDEO_FINISHED:
				cb->animInterface->commandStopAnimation();
			break;
		}
	}
	if (windowResized) al_acknowledge_resize(window);
	cb->inputInterface->postEventLoopUpdate();

	int dispWidth, dispHeight;
	dispWidth = al_get_display_width(window);
	dispHeight = al_get_display_height(window);

	if (windowMode == WindowMode::LockedAspect && windowResized == true) {
		float newRatio = (float)dispWidth / (float)dispHeight;

		float editedRatio;
		INFO("display's new ratio is: %f", newRatio)
		if (newRatio < defaultAspectRatio) {
			newWidth = dispWidth;
			newHeight = (int)((1.0f / defaultAspectRatio) * (float)dispWidth);

			editedRatio = newWidth / newHeight;
			INFO("Width: %i ,\n Height: %i \n", newWidth, newHeight)
			INFO("New Ratio: %f ", editedRatio)

		}
		else {
			newHeight = dispHeight;
			newWidth = (int)(defaultAspectRatio * (float)dispHeight);

			editedRatio = newWidth / newHeight;
			INFO("Width: %i, \n Height: %i \n", newWidth, newHeight)
			INFO("New Ratio: %f ", editedRatio)
		}

	}

	if (vSync) {
		al_wait_for_vsync();
	}

	fpsCounter++;
	if ((clock() - lastSecTimer) >= CLOCKS_PER_SEC) {
		currentFPS = fpsCounter;
		fpsCounter = 0;
		lastSecTimer = clock();
	}

	callDrawScreenCallbacks();

	cb->textInterface->renderAddTexts(*windowRenderTarget);
	cb->inputInterface->renderInput(*windowRenderTarget);
	cb->inputInterface->renderCursor(*windowRenderTarget);
	cb->soundInterface->updateAudio();

	if (windowMode == WindowMode::Resizeable) {
		al_set_target_backbuffer(window);
		al_draw_scaled_bitmap(windowRenderTarget->getBitmap(),
							  0, 0, windowRenderTarget->width(), windowRenderTarget->height(),
							  0, 0, al_get_display_width(window), al_get_display_height(window),
							  0);
		al_flip_display();
		currentRenderTarget->setAsCurrent(true);
		if (cls) {
			windowRenderTarget->clear(al_map_rgba_f(0, 0, 0, 1.0f));
		}
	}
	else if (windowMode == WindowMode::LockedAspect) {
		al_set_target_backbuffer(window);
		al_draw_scaled_bitmap(windowRenderTarget->getBitmap(),
							0,
							0,
							defaultWidth,
							defaultHeight,
							al_get_display_width(window) / 2 - newWidth / 2,
							al_get_display_height(window) / 2 - newHeight / 2,
							newWidth,
							newHeight,
							0
				      );
		al_flip_display();
		currentRenderTarget->setAsCurrent(true);
		if (cls) {
			windowRenderTarget->clear(al_map_rgba_f(0, 0, 0, 1.0f));
		}
	}
	else {
		if (cls) {
			al_flip_display();
			windowRenderTarget->clear(clearColor);
		}
		else {
			//Setting target to temporary bitmap
			al_set_target_bitmap(drawscreenTempBitmap);
			//Saving blender state
			int32_t a, b, c, aa, bb, cc;
			al_get_separate_blender(&a, &b, &c, &aa, &bb, &cc);
			//Setting blender state to replace
			al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
			//Drawing backbuffer to temporary bitmap
			al_draw_bitmap(windowRenderTarget->getBitmap(), 0, 0, 0);
			//Swaping window buffers
			al_flip_display();
			windowRenderTarget->setAsCurrent(true);
			windowRenderTarget->useWorldCoords(false);
			//Drawing temporary bitmap on backbuffer.
			al_draw_bitmap(drawscreenTempBitmap, 0, 0, 0);
			//Restoring blender state
			al_set_separate_blender(a, b, c, aa, bb, cc);
		}
	}
}

void GfxInterface::commandLock(void) {
	int32_t id = cb->popValue().getInt();
	if (id == 0) {
		currentRenderTarget->lock();
	}
	else {
		bufferMap[id]->lock();
	}
}

void GfxInterface::commandUnlock(void) {
	int32_t id = cb->popValue().getInt();
	if (id == 0) {
		currentRenderTarget->unlock();
	}
	else {
		bufferMap[id]->unlock();
	}
}

void GfxInterface::commandPutPixel(void) {
	int32_t id = cb->popValue().getInt();
	int32_t pixel = cb->popValue().getInt();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	if (id == 0) {
		currentRenderTarget->putPixel(
			x, y, al_map_rgb((pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF)
		);
	}
	else {
		bufferMap[id]->putPixel(
			x, y, al_map_rgb((pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF)
		);
	}
}

void GfxInterface::commandPutPixel2(void) {
	commandPutPixel();
}

void GfxInterface::commandCopyBox(void) {
	int32_t destId = cb->popValue().toInt();
	int32_t srcId = cb->popValue().toInt();
	int32_t destY = cb->popValue().toInt();
	int32_t destX = cb->popValue().toInt();
	int32_t height = cb->popValue().toInt();
	int32_t width = cb->popValue().toInt();
	int32_t sourceY = cb->popValue().toInt();
	int32_t sourceX = cb->popValue().toInt();
	RenderTarget *dest;
	RenderTarget *source;
	if (destId == 0) {
		dest = currentRenderTarget;
	}
	else {
		dest = getBuffer(destId);
	}
	if (srcId == 0) {
		source = currentRenderTarget;
	}
	else {
		source = getBuffer(srcId);
	}
	if (dest->cbImg != NULL) {
		dest->cbImg->switchMaskBitmaps(true);
		dest->copyBox(source, sourceX, sourceY, width, height, destX, destY);
		dest->cbImg->unmaskedBitmap = dest->getBitmap();
		dest->cbImg->switchMaskBitmaps(false);
	}
	else {
		dest->copyBox(source, sourceX, sourceY, width, height, destX, destY);
	}
}

void GfxInterface::commandCls(void) {
	currentRenderTarget->clear(clearColor);
}

void GfxInterface::commandDot(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	float y = cb->popValue().toFloat() + 0.5f;
	float x = cb->popValue().toFloat() + 0.5f;
	currentRenderTarget->drawDot(x, y, drawColor);
}

void GfxInterface::commandBox(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	bool fill = cb->popValue().toBool();
	float h = cb->popValue().toFloat() - 1.0f * (!fill);
	float w = cb->popValue().toFloat() - 1.0f * (!fill);
	float y = cb->popValue().toFloat() + 0.5f * (!fill);
	float x = cb->popValue().toFloat() + 0.5f * (!fill);
	currentRenderTarget->drawBox(x, y, w, h, fill, drawColor);
}

void GfxInterface::commandEllipse(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	bool fill = cb->popValue().toBool();
	float h = cb->popValue().toFloat();
	float w = cb->popValue().toFloat();
	float y = cb->popValue().toFloat() + 0.5f;
	float x = cb->popValue().toFloat() + 0.5f;
	currentRenderTarget->drawEllipse(x, y, w, h, fill, drawColor);
}

void GfxInterface::commandPickColor(void) {
	int y = cb->popValue().toInt();
	int x = cb->popValue().toInt();
	cb->gfxInterface->setDrawColor(windowRenderTarget->getPixel(x, y));
}

void GfxInterface::commandScreenGamma(void) {
	float blue = cb->popValue().toInt();
	float green = cb->popValue().toInt();
	float red = cb->popValue().toInt();
	windowGammaR = red / 255;
	windowGammaG = green / 255;
	windowGammaB = blue / 255;
}

void GfxInterface::commandDrawToImage(void) {
	int32_t id = cb->popValue().getInt();
	if (imageToDrawTo != NULL) {
		imageToDrawTo->switchMaskBitmaps(false);
	}
	imageToDrawTo = cb->imageInterface->getImage(id);
	imageToDrawTo->switchMaskBitmaps(true);
	setCurrentRenderTarget(imageToDrawTo->getRenderTarget());
}

void GfxInterface::commandDrawToScreen(void) {
	if (imageToDrawTo != NULL) {
		imageToDrawTo->switchMaskBitmaps(false);
		imageToDrawTo = NULL;
	}
	setCurrentRenderTarget(windowRenderTarget);
}

void GfxInterface::commandDrawToWorld(void) {
	drawTextToWorld = (bool)cb->popValue().toInt();
	drawImageToWorld = (bool)cb->popValue().toInt();
	drawDrawCommandToWorld = (bool)cb->popValue().toInt();
}

void GfxInterface::commandSmooth2D(void) {
	bool toggled = cb->popValue().toBool();
	cb->setSmooth2D(toggled);
}

void GfxInterface::commandScreenShot(void) {
	const string &str = cb->popValue().toString().getRef();
	al_save_bitmap(str.c_str(), windowRenderTarget->getBitmap());
}

void GfxInterface::commandUpdateGame(void) {
	callUpdateGameCallbacks();
	cb->objectInterface->updateObjects();
	gameUpdated = true;
}

void GfxInterface::commandDrawGame(void) {
	if (!gameUpdated) {
		callUpdateGameCallbacks();
		cb->objectInterface->updateObjects();
	}
	callDrawGameCallbacks();
	cb->objectInterface->drawObjects(*currentRenderTarget);
	gameDrawn = true;
	gameUpdated = true;
}

void GfxInterface::functionSCREEN(void) {
	cb->pushValue(windowRenderTarget->getId());
}

void GfxInterface::functionImage(void) {
	cb->popValue(); // Mystery integer, apparently does nothing
	int32_t id = cb->popValue().getInt();
	CBImage *img = cb->imageInterface->getImage(id);
	RenderTarget *rt = img->getRenderTarget();

	bufferMap[rt->getId()] = rt;
	cb->pushValue(rt->getId());
}

void GfxInterface::functionGetPixel(void) {
	int32_t id = cb->popValue().getInt();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	ALLEGRO_COLOR color;
	if (id == 0) {
		color = currentRenderTarget->getPixel(x, y);
	}
	else {
		color = bufferMap[id]->getPixel(x, y);
	}

	int32_t pixel;
	al_unmap_rgba(color, ((unsigned char*)&pixel) + 2, ((unsigned char*)&pixel) + 1, ((unsigned char*)&pixel), ((unsigned char*)&pixel) + 3);
	cb->pushValue(pixel);
}

void GfxInterface::functionGetPixel2(void) {
	functionGetPixel();
}

void GfxInterface::functionGetRGB(void) {
	uint8_t r, g, b, a;
	al_unmap_rgba(drawColor, &r, &g, &b, &a);
	switch (cb->popValue().getInt()) {
		case 1: cb->pushValue((int32_t)r); break;
		case 2: cb->pushValue((int32_t)g); break;
		case 3: cb->pushValue((int32_t)b); break;
		case 4: cb->pushValue((int32_t)a); break;
		default: cb->pushValue(0);
	}
}

void GfxInterface::functionScreenWidth(void) {
	cb->pushValue(screenWidth());
}

void GfxInterface::functionScreenHeight(void) {
	cb->pushValue(screenHeight());
}

void GfxInterface::functionScreenDepth(void) {
	cb->pushValue(al_get_pixel_format_bits(al_get_display_format(cb->gfxInterface->getWindow())));
}

/** Returns true if Gfx mode exists
 * @param width Gfx mode width
 * @param height Gfx mode height
 * @param depth Gfx mode color depth
 * @returns True if Gfx mode exists, otherwise false'
 */
void GfxInterface::functionGFXModeExists(void) {
	// Function variables
	bool gfxModeExists = false;

	// Pop input values
	int32_t depth = cb->popValue().getInt();
	int32_t height = cb->popValue().getInt();
	int32_t width = cb->popValue().getInt();

	// Get old display flags for later restoring
	int32_t oldFlags = al_get_display_flags(cb->gfxInterface->getWindow());

	// Set new display flags for fullscreen
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);

	// Create pointer for display modes
	ALLEGRO_DISPLAY_MODE *displayMode = new ALLEGRO_DISPLAY_MODE;

	for (int32_t i = 0; i < al_get_num_display_modes(); i++) {
		// Get display mode from id
		al_get_display_mode(i, displayMode);

		// See if this is what we're looking for
		if (displayMode->width == width && displayMode->height == height && al_get_pixel_format_bits(displayMode->format) == depth) {
			gfxModeExists = true;
			break;
		}
	}

	// Restore old display flags and return gfxModeExists value
	al_set_new_display_flags(oldFlags);
	cb->pushValue(gfxModeExists);
}

void GfxInterface::cbeGetPixel() {
	int32_t id = cb->popValue().getInt();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();

	ALLEGRO_COLOR color;
	if (id == 0) {
		color = currentRenderTarget->getPixel(x, y);
	}
	else {
		color = bufferMap[id]->getPixel(x, y);
	}

	int32_t pixel;
							//	R				G				B			A
	al_unmap_rgba(color, ((unsigned char*)&pixel) + 3, ((unsigned char*)&pixel) + 2, ((unsigned char*)&pixel+1), ((unsigned char*)&pixel));
	cb->pushValue(pixel);
}

void GfxInterface::cbePutPixel() {
	int32_t id = cb->popValue().getInt();
	int32_t pixel = cb->popValue().getInt();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	if (id == 0) {
		currentRenderTarget->putPixel(
			x, y, al_map_rgba((pixel >> 24) & 0xFF, (pixel >> 16) & 0xFF, (pixel >> 8) & 0xFF, pixel & 0xFF)
		);
	}
	else {
		bufferMap[id]->putPixel(
			x, y, al_map_rgba((pixel >> 24) & 0xFF, (pixel >> 16) & 0xFF, (pixel >> 16) & 0xFF, pixel & 0xFF)
		);
	}
	cb->pushValue(0);
}
#endif

void GfxInterface::setCurrentRenderTarget(RenderTarget *t) {
	currentRenderTarget = t;
}

void GfxInterface::registerWindow() {
	al_register_event_source(cb->getEventQueue(), al_get_display_event_source(window));
}

void GfxInterface::unregisterWindow() {
	al_unregister_event_source(cb->getEventQueue(), al_get_display_event_source(window));
}

void GfxInterface::resizeTempBitmap(int32_t w, int32_t h) {
	al_destroy_bitmap(drawscreenTempBitmap);
	drawscreenTempBitmap = al_create_bitmap(w, h);
}

void GfxInterface::callDrawScreenCallbacks() {
	for (map<int32_t, VoidFuncPtrType>::iterator i = drawScreenCallbacks.begin(); i != drawScreenCallbacks.end(); i++) {
		(i->second)();
	}
}

void GfxInterface::callDrawGameCallbacks() {
	for (map<int32_t, VoidFuncPtrType>::iterator i = drawGameCallbacks.begin(); i != drawGameCallbacks.end(); i++) {
		(i->second)();
	}
}

void GfxInterface::callUpdateGameCallbacks() {
	for (map<int32_t, VoidFuncPtrType>::iterator i = updateGameCallbacks.begin(); i != updateGameCallbacks.end(); i++) {
		(i->second)();
	}
}


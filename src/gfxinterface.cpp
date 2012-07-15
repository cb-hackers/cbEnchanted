#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "objectinterface.h"
#include "cbimage.h"
#include <allegro5/allegro_image.h>
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
		"	if (windowGamma.r > 0) nyCol.r = color.r + (1.0-color.r)*windowGamma.r; else nyCol.r = color.r + color.r*windowGamma.r; \n"
		"	if (windowGamma.g > 0) nyCol.g = color.g + (1.0-color.g)*windowGamma.g; else nyCol.g = color.g + color.g*windowGamma.g; \n"
		"	if (windowGamma.b > 0) nyCol.b = color.b + (1.0-color.b)*windowGamma.b; else nyCol.b = color.b + color.b*windowGamma.b; \n"
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
	cb = static_cast <CBEnchanted *> (this);
	fpsCounter = 0;
	currentFPS = 0;
	lastSecTimer = clock();
	lastFrameTime = mtimer();
}

GfxInterface::~GfxInterface() {
}

bool GfxInterface::initializeGfx() {
	// Initialize colors
	drawColor = al_map_rgb(255, 255, 255);
	clearColor = al_map_rgb(0, 0, 0);

	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE,0,ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP,1,ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP,1,ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_COMPATIBLE_DISPLAY,1,ALLEGRO_REQUIRE);
	window = al_create_display(400,300);
	if (!window) {
		cb->errors->createFatalError("Can't create window","Can't create default window.");
		return false;
	}

	//Register event source
	registerWindow();
	al_set_window_title(window,"");

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

	drawscreenTempBitmap = al_create_bitmap(400,300);

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
	uint32_t state = cb->popValue().toInt();
	uint32_t depth = cb->popValue().toInt();
	uint32_t height = cb->popValue().toInt();
	uint32_t width = cb->popValue().toInt();
	uint32_t flags;
	if (depth == 0) depth = 32;
	switch (state) {
		case 0: //cbFullscreen
			flags = ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN;
		break;
		case 1: // default
			flags = ALLEGRO_OPENGL | ALLEGRO_WINDOWED;
		break;
		case 2: //cbSizable
			flags = ALLEGRO_OPENGL | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE;
		break;
	}
	if ((al_get_display_flags(window) & flags) == flags) {
		if (state != 2) {
			al_resize_display(window, width, height);
			resizeTempBitmap(width, height);
		}
	}
	else {
		al_set_new_display_flags(flags);
		unregisterWindow();
		if (state != 2) {
			al_destroy_display(window);
			window = al_create_display(width, height);
			if (window == 0) {
				if (cb->isSmooth2D()) {
					cb->errors->createError("Can't create window","Creating window failed in command Screen.\nIf you try to continue, Smooth2D will be toggled off.");
					cb->setSmooth2D(false);
					window = al_create_display(width, height);
					if (window == 0) {
						cb->errors->createFatalError("Can't create window","Creating window failed in command Screen, even when Smooth2D was unset.");
						return;
					}
				}
				else {
					cb->errors->createFatalError("Can't create window","Creating window failed in command Screen.");
					return;
				}
				return;
			}
			resizeTempBitmap(width, height);
			windowRenderTarget->swapBitmap( al_get_backbuffer(window) );
			resizableWindow = false;
		}
		else { //cbSizable
			int32_t w = al_get_display_width(window);
			int32_t h = al_get_display_height(window);
			al_resize_display(window, w, h);
			if (cb->isSmooth2D()) {
				al_destroy_bitmap(drawscreenTempBitmap);
				drawscreenTempBitmap = al_create_bitmap(width, height);
			}
			else {
				resizeTempBitmap(width, height);
			}
			windowRenderTarget->swapBitmap(drawscreenTempBitmap);
			resizableWindow = true;
		}
		registerWindow();
	}

	if (state != 0) {
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
	float r = cb->popValue().toFloat()*0.5;
	float cy = cb->popValue().toFloat() + (dw ? -r :r);
	float cx = cb->popValue().toFloat() + r;
	currentRenderTarget->drawCircle(cx,cy,r,fill,drawColor);
}

void GfxInterface::commandLine(void){
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	float y2 = cb->popValue().toFloat()+0.5f;
	float x2 = cb->popValue().toFloat()+0.5f;
	float y1 = cb->popValue().toFloat()+0.5f;
	float x1 = cb->popValue().toFloat()+0.5f;

	currentRenderTarget->drawLine(x1,y1,x2,y2,drawColor);
}

void GfxInterface::commandDrawScreen(void) {
	bool vSync = cb->popValue().toBool();
	bool cls = cb->popValue().toInt();

	if ((mtimer() - lastFrameTime) <= 1000.0 / cb->getFrameLimit()) {
		al_rest(((1000.0 / cb->getFrameLimit()) - (mtimer() - lastFrameTime)) / 1000.0);
	}
	lastFrameTime = mtimer();

	if (!gameUpdated) cb->updateObjects();
	if (cb->isCamFollowing()) cb->updateCamFollow();
	if (!gameDrawn) cb->drawObjects(*windowRenderTarget);

	gameUpdated = false;
	gameDrawn = false;

	cb->preEventLoopUpdate();
	ALLEGRO_EVENT e;
	bool windowResized = false;
	while (al_get_next_event(cb->getEventQueue(), &e)) {
		if (cb->handleKeyboardEvent(&e)) {
			return;
		}
		cb->handleMouseEvent(&e);
		switch (e.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				if (cb->askForExit()) {
					cb->stop();
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				windowResized = true;
				break;
		}
	}
	if (windowResized) al_acknowledge_resize(window);
	cb->postEventLoopUpdate();

	if (vSync) {
		al_wait_for_vsync();
	}

	fpsCounter++;
	if ((clock()-lastSecTimer) >= CLOCKS_PER_SEC)
	{
		currentFPS = fpsCounter;
		fpsCounter = 0;
		lastSecTimer = clock();
	}
	cb->renderAddTexts(*windowRenderTarget);
	cb->renderInput(*windowRenderTarget);
	cb->renderCursor(*windowRenderTarget);
	cb->updateAudio();
	if (resizableWindow) {
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
	else {
		if (cls) {
			al_flip_display();
			windowRenderTarget->clear(clearColor);
		}
		else {
			//Setting target to temporary bitmap
			al_set_target_bitmap(drawscreenTempBitmap);
			//Saving blender state
			int32_t a,b,c, aa, bb, cc;
			al_get_separate_blender(&a,&b,&c, &aa, &bb, &cc);
			//Setting blender state to replace
			al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
			//Drawing backbuffer to temporary bitmap
			al_draw_bitmap(windowRenderTarget->getBitmap(),0,0,0);
			//Swaping window buffers
			al_flip_display();
			windowRenderTarget->setAsCurrent(true);
			windowRenderTarget->useWorldCoords(false);
			//Drawing temporary bitmap on backbuffer.
			al_draw_bitmap(drawscreenTempBitmap,0,0,0);
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
	RenderTarget *dest = getBuffer(destId);
	RenderTarget *source = getBuffer(srcId);
	dest->copyBox(source,sourceX,sourceY,width,height,destX,destY);
}

void GfxInterface::commandCls(void) {
	currentRenderTarget->clear(clearColor);
}

void GfxInterface::commandDot(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	float y = cb->popValue().toFloat()+0.5f;
	float x = cb->popValue().toFloat()+0.5f;
	currentRenderTarget->drawDot(x,y,drawColor);
}

void GfxInterface::commandBox(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	bool fill = cb->popValue().toBool();
	float h = cb->popValue().toFloat() - 1.0f * (!fill);
	float w = cb->popValue().toFloat() - 1.0f * (!fill);
	float y = cb->popValue().toFloat() + 0.5f * (!fill);
	float x = cb->popValue().toFloat() + 0.5f * (!fill);
	currentRenderTarget->drawBox(x,y,w,h,fill,drawColor);
}

void GfxInterface::commandEllipse(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld && !drawingOnImage());
	bool fill = cb->popValue().toBool();
	float h = cb->popValue().toFloat();
	float w = cb->popValue().toFloat();
	float y = cb->popValue().toFloat()+0.5f;
	float x = cb->popValue().toFloat()+0.5f;
	currentRenderTarget->drawEllipse(x,y,w,h,fill,drawColor);
}

void GfxInterface::commandPickColor(void) {
	int y = cb->popValue().toInt();
	int x = cb->popValue().toInt();
	cb->setDrawColor(windowRenderTarget->getPixel(x, y));
}

void GfxInterface::commandScreenGamma(void) {
	float blue = cb->popValue().toInt();
	float green = cb->popValue().toInt();
	float red = cb->popValue().toInt();
	windowGammaR = red/255;
	windowGammaG = green/255;
	windowGammaB = blue/255;
}

void GfxInterface::commandDrawToImage(void) {
	int32_t id = cb->popValue().getInt();
	if (imageToDrawTo != NULL) {
		imageToDrawTo->switchMaskBitmaps(false);
	}
	imageToDrawTo = cb->getImage(id);
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
	cb->updateObjects();
	gameUpdated = true;
}

void GfxInterface::commandDrawGame(void) {
	if (!gameUpdated) cb->updateObjects();
	cb->drawObjects(*currentRenderTarget);
	gameDrawn = true;
	gameUpdated = true;
}

void GfxInterface::functionSCREEN(void) {
	cb->pushValue(windowRenderTarget->getId());
}

void GfxInterface::functionImage(void) {
	cb->popValue(); //???
	int32_t id = cb->popValue().getInt();
	CBImage *img = cb->getImage(id);
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
		color = currentRenderTarget->getPixel(x,y);
	}
	else {
		color = bufferMap[id]->getPixel(x,y);
	}

	int32_t pixel;
	al_unmap_rgba(color,((unsigned char*)&pixel)+2,((unsigned char*)&pixel)+1,((unsigned char*)&pixel),((unsigned char*)&pixel)+3);
	cb->pushValue(pixel);
}

void GfxInterface::functionGetPixel2(void) {
	functionGetPixel();
}

void GfxInterface::functionGetRGB(void) {
	uint8_t r,g,b,a;
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
	STUB;
}

void GfxInterface::functionGFXModeExists(void) {
	STUB;
}
#endif

void GfxInterface::setCurrentRenderTarget(RenderTarget *t) {
	currentRenderTarget = t;
}

void GfxInterface::registerWindow() {
	al_register_event_source(cb->getEventQueue(),al_get_display_event_source(window));
}

void GfxInterface::unregisterWindow() {
	al_unregister_event_source(cb->getEventQueue(),al_get_display_event_source(window));
}

void GfxInterface::resizeTempBitmap(int32_t w, int32_t h) {
	al_destroy_bitmap(drawscreenTempBitmap);
	drawscreenTempBitmap = al_create_bitmap(w, h);
}


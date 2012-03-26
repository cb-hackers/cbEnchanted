#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "objectinterface.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "cbimage.h"
#include <allegro5/allegro_image.h>
#include "util.h"

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


GfxInterface::GfxInterface() :
	cb(static_cast <CBEnchanted *> (this)),
	windowTitle(""),
	window(),
	drawDrawCommandToWorld(false),
	drawImageToWorld(false),
	drawTextToWorld(false),
	gameDrawn(false),
	gameUpdated(false)
{
	ALLEGRO_COLOR  test = al_map_rgba_f(1,0,1,1);

	drawColor = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
	clearColor = al_map_rgba_f(0,0,0,1.0f);
	fpsCounter = 0;
	currentFPS = 0;
	lastSecTimer = clock();
	lastFrameTime = mtimer();
}

GfxInterface::~GfxInterface() {
}

void GfxInterface::initializeGfx()
{
	al_init_image_addon();
	al_init_primitives_addon();
	al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE,0,ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SUPPORT_NPOT_BITMAP,1,ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP,1,ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_COMPATIBLE_DISPLAY,1,ALLEGRO_REQUIRE);
	window = al_create_display(400,300);
	registerWindow();
	al_set_window_title(window,"");
	windowRenderTarget = new RenderTarget;
	windowRenderTarget->create(400,300);
	windowRenderTarget->clear(clearColor);

	currentRenderTarget = windowRenderTarget;
	windowGammaR = 0;
	windowGammaG = 0;
	windowGammaB = 0;
	cb->updateInputs();
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
			al_resize_display(window,width,height);
		}
	}
	else {
		al_set_new_display_flags(flags);
		unregisterWindow();
		if (state != 2) {
			al_destroy_display(window);
			window = al_create_display(width,height);
		}
		else {
			int32_t w = al_get_display_width(window);
			int32_t h = al_get_display_height(window);
			al_destroy_display(window);
			window = al_create_display(w,h);
		}
		registerWindow();
	}

	windowRenderTarget->create(width,height);

}

void GfxInterface::commandClsColor(void) {
	float b = cb->popValue().toFloat()/255.0f;
	float g = cb->popValue().toFloat()/255.0f;
	float r = cb->popValue().toFloat()/255.0f;
	clearColor.r = r;
	clearColor.g = g;
	clearColor.b = b;
}

void GfxInterface::commandColor(void) {
	float b = cb->popValue().toFloat()/255.0f;
	float g = cb->popValue().toFloat()/255.0f;
	float r = cb->popValue().toFloat()/255.0f;
	drawColor.r = r;
	drawColor.g = g;
	drawColor.b = b;
}

void GfxInterface::commandCircle(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld);
	bool fill = cb->popValue().toInt();
	float r = cb->popValue().toFloat()*0.5;
	float cy = cb->popValue().toFloat() + r;
	float cx = cb->popValue().toFloat() + r;
	currentRenderTarget->drawCircle(cx,cy,r,fill,drawColor);
}

void GfxInterface::commandLine(void){
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld);
	float y2 = cb->popValue().toFloat();
	float x2 = cb->popValue().toFloat();
	float y1 = cb->popValue().toFloat();
	float x1 = cb->popValue().toFloat();

	currentRenderTarget->drawLine(x1,y1,x2,y2,drawColor);
}

void GfxInterface::commandDrawScreen(void) {
	bool vSync = cb->popValue().toInt();
	bool cls = cb->popValue().toInt();
	if ((mtimer()-lastFrameTime)<=cb->getFrameLimit()){
		sleep(cb->getFrameLimit()-(mtimer()-lastFrameTime));
	}
	lastFrameTime = mtimer();
	if (!gameUpdated) cb->updateObjects();
	if (!gameDrawn) cb->drawObjects(*windowRenderTarget);

	gameUpdated = false;
	gameDrawn = false;
	ALLEGRO_EVENT e;
	while (al_get_next_event(cb->getEventQueue(),&e)) {
		switch (e.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				cb->stop();
				break;
			default:
				break;
		}
	}
	cb->updateInputs();

	fpsCounter++;
	if ((clock()-lastSecTimer) >= CLOCKS_PER_SEC)
	{
		currentFPS = fpsCounter;
		fpsCounter = 0;
		lastSecTimer = clock();
	}
	cb->renderAddTexts(*windowRenderTarget);
	al_set_target_backbuffer(window);
	al_draw_scaled_bitmap(windowRenderTarget->getBitmap(),0,0,al_get_bitmap_width(windowRenderTarget->getBitmap()),al_get_bitmap_height(windowRenderTarget->getBitmap()),0,0,al_get_display_width(window),al_get_display_height(window),0);

	al_flip_display();
	bindRenderTarget = 0;
	if (cls) {
		windowRenderTarget->clear(clearColor);
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
	STUB;
}

void GfxInterface::commandPutPixel2(void) {
	int32_t id = cb->popValue().getInt();
	int32_t pixel = cb->popValue().getInt();
	int32_t y = cb->popValue().toInt();
	int32_t x = cb->popValue().toInt();
	if (id == 0) {
		currentRenderTarget->putPixel(x,y,al_map_rgb((pixel&&0xFF0000)>>16,(pixel&&0xFF00)>>8,pixel&&0xFF));
	}
	else {
		bufferMap[id]->putPixel(x,y,al_map_rgb((pixel&&0xFF0000)>>16,(pixel&&0xFF00)>>8,pixel&&0xFF));
	}
}

void GfxInterface::commandCopyBox(void) {
	STUB;
}

void GfxInterface::commandCls(void) {
	currentRenderTarget->clear(clearColor);
}

void GfxInterface::commandDot(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld);
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	currentRenderTarget->drawDot(x,y,drawColor);
}

void GfxInterface::commandBox(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld);
	bool fill = cb->popValue().toInt();
	float h = cb->popValue().toFloat();
	float w = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	currentRenderTarget->drawBox(x,y,w,h,fill,drawColor);
}

void GfxInterface::commandEllipse(void) {
	currentRenderTarget->useWorldCoords(drawDrawCommandToWorld);
	bool fill = cb->popValue().toInt();
	float h = cb->popValue().toFloat();
	float w = cb->popValue().toFloat();
	float y = cb->popValue().toFloat();
	float x = cb->popValue().toFloat();
	currentRenderTarget->drawEllipse(x,y,w,h,fill,drawColor);
}

void GfxInterface::commandPickColor(void) {
	STUB;
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
	setCurrentRenderTarget(cb->getImage(id)->getRenderTarget());
}

void GfxInterface::commandDrawToScreen(void) {
	setCurrentRenderTarget(windowRenderTarget);
}

void GfxInterface::commandDrawToWorld(void) {
	drawTextToWorld = (bool)cb->popValue().toInt();
	drawImageToWorld = (bool)cb->popValue().toInt();
	drawDrawCommandToWorld = (bool)cb->popValue().toInt();
}

void GfxInterface::commandSmooth2D(void) {
	STUB;
}

void GfxInterface::commandScreenShot(void) {
	STUB;
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
	int32_t id = cb->popValue().getInt();
	RenderTarget *rt = cb->getImage(id)->getRenderTarget();

	bufferMap[rt->getId()] = rt;
	cb->pushValue(rt->getId());
}

void GfxInterface::functionGetPixel(void) {
	STUB;
}

void GfxInterface::functionGetPixel2(void) {
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

void GfxInterface::functionGetRGB(void) {
	switch (cb->popValue().toInt()) {
		case 0: cb->pushValue((int32_t)(drawColor.r*255.0)); break;
		case 1: cb->pushValue((int32_t)(drawColor.g*255.0)); break;
		case 2: cb->pushValue((int32_t)(drawColor.b*255.0)); break;
		case 3: cb->pushValue((int32_t)(drawColor.a*255.0)); break;
		default: cb->pushValue(0); break;
	}
}

void GfxInterface::functionScreenWidth(void) {
	cb->pushValue(al_get_display_width(window));
}

void GfxInterface::functionScreenHeight(void) {
	cb->pushValue(al_get_display_height(window));
}

void GfxInterface::functionScreenDepth(void) {
	STUB;
}

void GfxInterface::functionGFXModeExists(void) {
	STUB;
}

void GfxInterface::setCurrentRenderTarget(RenderTarget *t) {
	currentRenderTarget = t;
}

void GfxInterface::registerWindow() {
	al_register_event_source(cb->getEventQueue(),al_get_display_event_source(window));
}

void GfxInterface::unregisterWindow() {
	al_unregister_event_source(cb->getEventQueue(),al_get_display_event_source(window));
}

void GfxInterface::sleep(int64_t time){
	int64_t start = mtimer();
	while(!((start+time)<=mtimer())){

	}
	return;
}

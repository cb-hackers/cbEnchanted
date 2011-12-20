#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "drawingprimitives.h"
#include <SFML/Graphics/Shape.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
GfxInterface::GfxInterface() : cb(static_cast <CBEnchanted *> (this)), windowTitle(""), clearColor(0, 0, 0, 255), drawColor(255, 255, 255, 255), window(sf::VideoMode(400, 300, 32), "", sf::Style::Default) {
	window.SetActive(true);
    fpsCounter = 0;
    currentFPS = 0;
    lastSecTimer = clock();
}

GfxInterface::~GfxInterface() {
}

void GfxInterface::commandScreen(void) {
    uint32_t state = cb->popValue().toInt();
    uint32_t depth = cb->popValue().toInt();
    uint32_t height = cb->popValue().toInt();
    uint32_t width = cb->popValue().toInt();
	uint32_t style;
	switch (state) {
		case 0: //cbFullscreen
			style = sf::Style::Fullscreen;
			break;
		case 1: // default
			style = sf::Style::Close;
			break;
		case 2: //cbSizable
			style = sf::Style::Close | sf::Style::Resize;
			break;
	}
	
	window.Create(sf::VideoMode(width, height, depth), windowTitle, style);
}

void GfxInterface::commandClsColor(void) {
    uint8_t b = cb->popValue().toByte();
    uint8_t g = cb->popValue().toByte();
    uint8_t r = cb->popValue().toByte();
    clearColor.r = r;
    clearColor.g = g;
    clearColor.b = b;
}

void GfxInterface::commandColor(void) {
    uint8_t b = cb->popValue().toByte();
    uint8_t g = cb->popValue().toByte();
    uint8_t r = cb->popValue().toByte();
    drawColor.r = r;
    drawColor.g = g;
    drawColor.b = b;
}


void GfxInterface::commandCircle(void) {
    bool fill = cb->popValue().toInt();
    float rad = cb->popValue().toFloat();
    float cy = cb->popValue().toFloat() + rad * 0.5;
    float cx = cb->popValue().toFloat() + rad * 0.5;
	Circle circle(cx, cy, rad * 0.5, fill);
	glColor3ub(drawColor.r, drawColor.g, drawColor.b);
	window.Draw(circle);
}

void GfxInterface::commandLine(void){
    float y2 = cb->popValue().toFloat();
    float x2 = cb->popValue().toFloat();
    float y1 = cb->popValue().toFloat();
    float x1 = cb->popValue().toFloat();
	glColor3ub(drawColor.r, drawColor.g, drawColor.b);
	Line line(x1, y1, x2, y2);
	window.Draw(line);
}

void GfxInterface::commandDrawScreen(void) {
    bool vSync = cb->popValue().toInt();
    bool cls = cb->popValue().toInt();
	sf::Event e;
	
	while (window.PollEvent(e)) {
		switch (e.Type) {
			case sf::Event::Closed:
				cb->stop();
				break;
			//TODO: Inputs
			default:
				break;
		}
	}
    fpsCounter++;
    if ((clock()-lastSecTimer) >= CLOCKS_PER_SEC)
    {
        currentFPS = fpsCounter;
        fpsCounter = 0;
        lastSecTimer = clock();
    }
	window.Display();

	if (cls) window.Clear(clearColor);
}

void GfxInterface::commandLock(void) {
	
}

void GfxInterface::commandUnlock(void) {
	
}

void GfxInterface::commandPutPixel(void) {
	
}

void GfxInterface::commandPutPixel2(void) {
	
}

void GfxInterface::commandCopyBox(void) {
	
}

void GfxInterface::commandCls(void) {
	
}

void GfxInterface::commandDot(void) {
	
}

void GfxInterface::commandBox(void) {
	
}

void GfxInterface::commandEllipse(void) {
	
}

void GfxInterface::commandPickColor(void) {
	
}

void GfxInterface::commandScreenGamma(void) {
	
}

void GfxInterface::commandDrawToImage(void) {
	
}

void GfxInterface::commandDrawToScreen(void) {
	
}

void GfxInterface::commandDrawToWorld(void) {
	
}

void GfxInterface::commandSmooth2D(void) {
	
}

void GfxInterface::commandScreenShot(void) {
	
}

void GfxInterface::commandUpdateGame(void) {
	
}

void GfxInterface::commandDrawGame(void) {
	
}

void GfxInterface::functionSCREEN(void) {
	cb->pushValue(0);
}

void GfxInterface::functionImage(void) {
	
}

void GfxInterface::functionGetPixel(void) {
	
}

void GfxInterface::functionGetPixel2(void) {
	
}

void GfxInterface::functionGetRGB(void) {
	
}

void GfxInterface::functionScreenWidth(void) {
	
}

void GfxInterface::functionScreenHeight(void) {
	
}

void GfxInterface::functionScreenDepth(void) {
	
}

void GfxInterface::functionGFXModeExists(void) {
	
}

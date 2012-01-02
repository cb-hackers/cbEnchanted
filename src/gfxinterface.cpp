#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "drawingprimitives.h"
#include <SFML/Graphics/Shape.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
GfxInterface::GfxInterface() : cb(static_cast <CBEnchanted *> (this)), windowTitle(""), clearColor(0, 0, 0, 255), drawColor(255, 255, 255, 255), window(),drawDrawCommandToWorld(false),drawImageToWorld(false),drawTextToWorld(false),windowRenderTargetPointer(&window) {
    window.SetActive(true);
    fpsCounter = 0;
    currentFPS = 0;
    lastSecTimer = clock();
    sf::ContextSettings windowSettings;
    windowSettings.AntialiasingLevel = 1;
    windowSettings.DepthBits = 0;
    window.EnableVerticalSync(true);
    window.Create(sf::VideoMode(400, 300, 32), "", sf::Style::Close,windowSettings);
    windowSettings = window.GetSettings();
    INFO("Window antialiasing level: %i",windowSettings.AntialiasingLevel);
    currentRenderTarget = &windowRenderTargetPointer;
}

GfxInterface::~GfxInterface() {
}

void GfxInterface::commandScreen(void) {
    uint32_t state = cb->popValue().toInt();
    uint32_t depth = cb->popValue().toInt();
    uint32_t height = cb->popValue().toInt();
    uint32_t width = cb->popValue().toInt();
	uint32_t style;
    if (depth == 0) depth = 32;
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
    window.Create(sf::VideoMode(width, height, depth), windowTitle, style,window.GetSettings());
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
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    bool fill = cb->popValue().toInt();
    float rad = cb->popValue().toFloat();
    float cy = cb->popValue().toFloat() + rad * 0.5;
    float cx = cb->popValue().toFloat() + rad * 0.5;
    //sf::Shape circle = sf::Shape::Circle(cx,cy,rad,drawColor);

	Circle circle(cx, cy, rad * 0.5, fill);
    glColor3ub(drawColor.r, drawColor.g, drawColor.b);
    currentRenderTarget->draw(circle);
}

void GfxInterface::commandLine(void){
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    float y2 = cb->popValue().toFloat();
    float x2 = cb->popValue().toFloat();
    float y1 = cb->popValue().toFloat();
    float x1 = cb->popValue().toFloat();
    glColor3ub(drawColor.r, drawColor.g, drawColor.b);
    Line line(x1, y1, x2, y2);
    //sf::Shape line = sf::Shape::Line(x1,y1,x2,y2,1,drawColor);
    currentRenderTarget->draw(line);
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
            case sf::Event::KeyPressed:
                if (cb->isSafeExit() && e.Key.Code == sf::Keyboard::Escape) cb->stop(); //Safe exit
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
    currentRenderTarget->display();

    if (cls) currentRenderTarget->clear(clearColor);
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
    currentRenderTarget->clear(clearColor);
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
    switch (cb->popValue().toInt()) {
        case 0: cb->pushValue((int32_t)drawColor.r); break;
        case 1: cb->pushValue((int32_t)drawColor.g); break;
        case 2: cb->pushValue((int32_t)drawColor.b); break;
        default: cb->pushValue(0); break;
    }
}

void GfxInterface::functionScreenWidth(void) {
    cb->pushValue((int32_t)window.GetWidth());
}

void GfxInterface::functionScreenHeight(void) {
    cb->pushValue((int32_t)window.GetHeight());
}

void GfxInterface::functionScreenDepth(void) {
	
}

void GfxInterface::functionGFXModeExists(void) {
	
}


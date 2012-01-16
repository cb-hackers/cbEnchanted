#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"

#include <SFML/Graphics/Shape.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
GfxInterface::GfxInterface() : cb(static_cast <CBEnchanted *> (this)), windowTitle(""), clearColor(0, 0, 0, 255), drawColor(255, 255, 255, 255), window(),drawDrawCommandToWorld(false),drawImageToWorld(false),drawTextToWorld(false) {
    fpsCounter = 0;
    currentFPS = 0;
    lastSecTimer = clock();
}

GfxInterface::~GfxInterface() {
}

void GfxInterface::initializeGfx()
{
    sf::ContextSettings windowSettings;
    windowSettings.AntialiasingLevel = 0;
    windowSettings.DepthBits = 0;
    window.EnableVerticalSync(true);
    window.Create(sf::VideoMode(400, 300, 32), "", sf::Style::Close,windowSettings);
    windowSettings = window.GetSettings();
    windowRenderTarget.create(400,300);

    setCurrentRenderTarget(&windowRenderTarget);
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
    windowRenderTarget.create(width, height);

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
    currentRenderTarget->setColor(drawColor);
}
#define CIRCLE_SEGMENT_COUNT 100

void GfxInterface::commandCircle(void) {
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    bool fill = cb->popValue().toInt();
    float r = cb->popValue().toFloat()*0.5;
    float cy = cb->popValue().toFloat() + r;
    float cx = cb->popValue().toFloat() + r;
    currentRenderTarget->drawCircle(cx,cy,r,fill);
}

void GfxInterface::commandLine(void){
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    float y2 = cb->popValue().toFloat();
    float x2 = cb->popValue().toFloat();
    float y1 = cb->popValue().toFloat();
    float x1 = cb->popValue().toFloat();

    currentRenderTarget->drawLine(x1,y1,x2,y2);
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
    windowRenderTarget.display();
    sf::Sprite sprite(windowRenderTarget.getSurface()->GetTexture());
    sprite.SetPosition(0,0);
    sprite.SetScale((float)window.GetWidth()/(float)windowRenderTarget.width(),(float)window.GetHeight()/(float)windowRenderTarget.height());
    window.Draw(sprite);
    window.Display();
    if (cls) {
        windowRenderTarget.clear(clearColor);
    }
    windowRenderTarget.setup();
}

void GfxInterface::commandLock(void) {
	STUB;
}

void GfxInterface::commandUnlock(void) {
	STUB;
}

void GfxInterface::commandPutPixel(void) {
	STUB;
}

void GfxInterface::commandPutPixel2(void) {
	STUB;
}

void GfxInterface::commandCopyBox(void) {
	STUB;
}

void GfxInterface::commandCls(void) {
    currentRenderTarget->clear(clearColor);
}

void GfxInterface::commandDot(void) {
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    float y = cb->popValue().toFloat();
    float x = cb->popValue().toFloat();
    currentRenderTarget->drawDot(x,y);
}

void GfxInterface::commandBox(void) {
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    bool fill = cb->popValue().toInt();
    float h = cb->popValue().toFloat();
    float w = cb->popValue().toFloat();
    float y = cb->popValue().toFloat();
    float x = cb->popValue().toFloat();
    currentRenderTarget->drawBox(x,y,w,h,fill);
}

void GfxInterface::commandEllipse(void) {
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    bool fill = cb->popValue().toInt();
    float h = cb->popValue().toFloat();
    float w = cb->popValue().toFloat();
    float y = cb->popValue().toFloat();
    float x = cb->popValue().toFloat();
    currentRenderTarget->drawEllipse(x,y,w,h,fill);
}

void GfxInterface::commandPickColor(void) {
	STUB;
}

void GfxInterface::commandScreenGamma(void) {
	STUB;
}

void GfxInterface::commandDrawToImage(void) {
	STUB;
}

void GfxInterface::commandDrawToScreen(void) {
	STUB;
}

void GfxInterface::commandDrawToWorld(void) {
	STUB;
}

void GfxInterface::commandSmooth2D(void) {
	STUB;
}

void GfxInterface::commandScreenShot(void) {
	STUB;
}

void GfxInterface::commandUpdateGame(void) {
	STUB;
}

void GfxInterface::commandDrawGame(void) {
	STUB;
}

void GfxInterface::functionSCREEN(void) {
	cb->pushValue(0);
}

void GfxInterface::functionImage(void) {
	STUB;
}

void GfxInterface::functionGetPixel(void) {
	STUB;
}

void GfxInterface::functionGetPixel2(void) {
	STUB;
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
	STUB;
}

void GfxInterface::functionGFXModeExists(void) {
    STUB;
}

void GfxInterface::setCurrentRenderTarget(RenderTarget *t) {
    currentRenderTarget = t;
    t->setup();
}


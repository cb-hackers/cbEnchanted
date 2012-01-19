#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"

#include <SFML/Graphics/Shape.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
#include "cbimage.h"
GfxInterface::GfxInterface() : cb(static_cast <CBEnchanted *> (this)), windowTitle(""), clearColor(0, 0, 0, 255), drawColor(255, 255, 255, 255), window(),drawDrawCommandToWorld(false),drawImageToWorld(false),drawTextToWorld(false),currentRenderTarget(0) {
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

    bufferMap[windowRenderTarget.getId()] = &windowRenderTarget;
    currentRenderTarget = &windowRenderTarget;
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
    sprite.SetScale((float)window.GetWidth()/(float)windowRenderTarget.width(),(float)window.GetHeight()/(float)windowRenderTarget.height());
    window.Draw(sprite);
    window.Display();
    if (cls) {
        windowRenderTarget.clear(clearColor);
    }
    currentRenderTarget->setup();
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
        currentRenderTarget->putPixel2(x,y,pixel);
    }
    else {
        bufferMap[id]->putPixel2(x,y,pixel);
    }
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
    int32_t id = cb->popValue().getInt();
    setCurrentRenderTarget(cb->getImage(id)->getRenderTarget());
}

void GfxInterface::commandDrawToScreen(void) {
    setCurrentRenderTarget(&windowRenderTarget);
}

void GfxInterface::commandDrawToWorld(void) {
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
    cb->pushValue(windowRenderTarget.getId());
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
    sf::Color color;
    if (id == 0) {
        cb->pushValue(currentRenderTarget->getPixel2(x,y));
    }
    else {
        cb->pushValue(bufferMap[id]->getPixel2(x,y));
    }
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
    currentRenderTarget->display(); //Check if works without.
    currentRenderTarget = t;
    t->setup();
}


#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"

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
#define CIRCLE_SEGMENT_COUNT 100

void GfxInterface::commandCircle(void) {
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    bool fill = cb->popValue().toInt();
    float r = cb->popValue().toFloat()*0.5;
    float cy = cb->popValue().toFloat() + r;
    float cx = cb->popValue().toFloat() + r;
    //sf::Shape circle = sf::Shape::Circle(cx,cy,rad,drawColor);
    int segmentCount = CIRCLE_SEGMENT_COUNT;
    sf::Vertex vertices[CIRCLE_SEGMENT_COUNT+1];
    int index = 0;
    if (fill) {
        segmentCount -= 1;
        index = 1;
        vertices[0].Color = drawColor;
        vertices[0].Position.x = cx;
        vertices[0].Position.y = cy;
    }



    float theta = 2 * 3.1415926 / float(segmentCount);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;
    float x = r;
    float y = 0;

    for(int ii = 0; ii < segmentCount; ii++) {
        vertices[index].Position.x = x + cx;
        vertices[index].Position.y = y + cy;
        vertices[index].Color = drawColor;
        index++;
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    vertices[index].Position.x = x + cx;
    vertices[index].Position.y = y + cy;
    vertices[index].Color = drawColor;
    if (fill) {
        currentRenderTarget->draw(vertices,CIRCLE_SEGMENT_COUNT+1,sf::TrianglesFan);
    }
    else {
        currentRenderTarget->draw(vertices,CIRCLE_SEGMENT_COUNT+1,sf::LinesStrip);
    }
}

void GfxInterface::commandLine(void){
    currentRenderTarget->setViewTo(drawDrawCommandToWorld);
    sf::Vertex points[2];
    points[0].Position.y = cb->popValue().toFloat();
    points[0].Position.x = cb->popValue().toFloat();
    points[1].Position.y = cb->popValue().toFloat();
    points[1].Position.x = cb->popValue().toFloat();

    points[0].Color = drawColor;
    points[1].Color = drawColor;
    currentRenderTarget->draw(points,2,sf::Lines);
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
    window.Display();

    if (cls) window.Clear(clearColor);
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
	STUB;
}

void GfxInterface::commandBox(void) {
	STUB;
}

void GfxInterface::commandEllipse(void) {
	STUB;
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


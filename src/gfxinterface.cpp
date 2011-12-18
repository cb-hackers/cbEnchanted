#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"

#ifdef WIN32
#include <Windows.h>
#endif
#include <SFML/Graphics/Shape.hpp>
#include "drawingprimitives.h"

GfxInterface::GfxInterface() : cb(static_cast <CBEmu *> (this)), windowTitle(""), clearColor(0, 0, 0, 255), drawColor(255, 255, 255, 255), window(sf::VideoMode(400, 300, 32), "", sf::Style::Default) {
	window.SetActive(true);
}

GfxInterface::~GfxInterface() {
}

void GfxInterface::commandScreen(void) {
	uint32_t state = cb->popValue <int32_t>();
	uint32_t depth = cb->popValue <int32_t>();
	uint32_t height = cb->popValue <int32_t>();
	uint32_t width = cb->popValue <int32_t>();
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

void GfxInterface::functionScreen(void) {
	cb->pushValue(0);
}

void GfxInterface::commandClsColor(void) {
	float b = cb->popValue<float>();
	float g = cb->popValue<float>();
	float r = cb->popValue<float>();
	clearColor.r = (uint8_t)r;
	clearColor.g = (uint8_t)g;
	clearColor.b = (uint8_t)b;
}

void GfxInterface::commandColor(void) {
	float b = cb->popValue<float>();
	float g = cb->popValue<float>();
	float r = cb->popValue<float>();
	drawColor.r = (uint8_t)r;
	drawColor.g = (uint8_t)g;
	drawColor.b = (uint8_t)b;
}


void GfxInterface::commandCircle(void) {
	bool fill = cb->popValue<int32_t>();
	float rad = (float)cb->popValue<int32_t>();
	float cy = cb->popValue<float>() + rad *0.5;
	float cx = cb->popValue<float>() + rad *0.5;
	Circle circle(cx,cy,rad*0.5,fill);
	glColor3ub(drawColor.r,drawColor.g,drawColor.b);
	window.Draw(circle);
}

void GfxInterface::commandLine(void){
	float y2 = cb->popValue<float>();
	float x2 = cb->popValue<float>();
	float y1 = cb->popValue<float>();
	float x1 = cb->popValue<float>();
	glColor3ub(drawColor.r,drawColor.g,drawColor.b);
	Line line(x1,y1,x2,y2);
	window.Draw(line);
}

void GfxInterface::commandDrawScreen(void) {
	bool vSync = cb->popValue<int32_t>();
	bool cls = cb->popValue<int32_t>();

	sf::Event e;
	while (window.PollEvent(e)) {
		switch (e.Type) {
			case sf::Event::Closed:
				window.Close();
				break;
			//TODO: Inputs
			default:
				break;
		}
	}
	window.Display();

	if (cls) window.Clear(clearColor);
}

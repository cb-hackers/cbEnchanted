#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <SFML/Graphics/Shape.hpp>

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

#define CIRCLE_SEGMENT_COUNT 100
void GfxInterface::commandCircle(void) {
	bool fill = cb->popValue<int32_t>();
	int32_t rad = cb->popValue<int32_t>();
	float cy = cb->popValue<float>() + (float)rad * 0.5;
	float cx = cb->popValue<float>() + (float)rad * 0.5;

	sf::Shape circle(sf::Shape::Circle(cx, cy, rad, drawColor));
	circle.EnableFill(fill);
	window.Draw(circle);


	/*Doesn't work...
	float theta = 2 * 3.1415926 / float(CIRCLE_SEGMENT_COUNT); 
	float c = cosf(theta);
	float s = sinf(theta);
	float t;

	float x = rad;
	float y = 0; 
	renderer->SetColor(drawColor);
	if (fill)
	{
		renderer->Begin(GL_TRIANGLE_FAN);
		glVertex2f(cx,cy);
	}
	else
		glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii != CIRCLE_SEGMENT_COUNT; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();*/
}

void GfxInterface::commandLine(void){
	float y2 = cb->popValue<float>();
	float x2 = cb->popValue<float>();
	float y1 = cb->popValue<float>();
	float x1 = cb->popValue<float>();
	glColor3i(drawColor.r, drawColor.g, drawColor.b);
	glBegin(GL_LINE);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
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

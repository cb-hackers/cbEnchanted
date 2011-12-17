#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"

GfxInterface::GfxInterface() : cb(static_cast <CBEmu *> (this)) {
	window = new CL_DisplayWindow("CBEmu", 400, 300);
	slotInputDown = (window->get_ic().get_keyboard()).sig_key_down().connect((InputInterface *) static_cast<CBEmu *> (this), &InputInterface::onKeyDown);
	
	graphicContexts.push_back(&window->get_gc());
}

void GfxInterface::commandScreen(void) {
	uint32_t state = cb->popValue <int32_t>();
	uint32_t depth = cb->popValue <int32_t>();
	uint32_t height = cb->popValue <int32_t>();
	uint32_t width = cb->popValue <int32_t>();

	delete window;
	window = new CL_DisplayWindow("CBEmu", width, height, state == 0, state == 2);
	slotInputDown.destroy();
	slotInputDown = (window->get_ic().get_keyboard()).sig_key_down().connect((InputInterface *) static_cast<CBEmu *> (this), &InputInterface::onKeyDown);
	
}

void GfxInterface::functionScreen(void) {
	cb->pushValue(0);
}

void GfxInterface::commandClsColor(void) {
	float b = cb->popValue<float>();
	float g = cb->popValue<float>();
	float r = cb->popValue<float>();
	
	cb->setClsColor(CL_Colorf((uint8_t)r, (uint8_t)g, (uint8_t)b));
}

void GfxInterface::commandColor(void) {
	float b = cb->popValue<float>();
	float g = cb->popValue<float>();
	float r = cb->popValue<float>();
	
	cb->setDrawColor(CL_Colorf((uint8_t)r, (uint8_t)g, (uint8_t)b));
}

void GfxInterface::commandCircle(void) {
	bool fill = cb->popValue<int32_t>();
	int32_t rad = cb->popValue<int32_t>();
	float y = cb->popValue<float>() + rad / 2;
	float x = cb->popValue<float>() + rad / 2;

	CL_Draw::circle(window->get_gc(), x, y, rad / 2, cb->getDrawColor());
}

void GfxInterface::commandLine(void){
	float y2 = cb->popValue<float>();
	float x2 = cb->popValue<float>();
	float y1 = cb->popValue<float>();
	float x1 = cb->popValue<float>();

	CL_Draw::line(window->get_gc(),x1,y1,x2,y2,cb->getDrawColor());
}

void GfxInterface::commandDrawScreen(void) {
	bool vSync = cb->popValue<int32_t>();
	bool cls = cb->popValue<int32_t>();
	CL_KeepAlive::process();
	window->flip(1);
	if (cls) window->get_gc().clear(cb->getClsColor());
}

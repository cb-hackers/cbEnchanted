#include "precomp.h"
#include "game.h"
#include "map.h"

void Game::Run(void) {
	run = true;
	
	CL_DisplayWindowDescription desc;
	desc.set_title("Mushroom's World");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(false);

	CL_DisplayWindow window(desc);

	CL_Slot slot_quit = window.sig_window_close().connect(this, &Game::onCloseWindow);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Game::onKeyUp);
	CL_Slot slot_input_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &Game::onKeyDown);

	CL_GraphicContext gc = window.get_gc();

	CL_ResourceManager resources("resources.xml");
	//gc.set_scale(0.4, 0.4);
	Map map;
	map.load(gc, "map1", resources);
	float x, y;
	x = 0.0;
	y = 0.0;
	// Run until someone presses escape, or closes the window
	while (run) {
		gc.clear(CL_Colorf::black);
		
		if (keys.find(CL_KEY_RIGHT) != keys.end()) {
			x -= 2;
		}
		if (keys.find(CL_KEY_LEFT) != keys.end()) {
			x += 2;
		}
		if (keys.find(CL_KEY_UP) != keys.end()) {
			y += 2;
		}
		if (keys.find(CL_KEY_DOWN) != keys.end()) {
			y -= 2;
		}
		
		if (keys.find(CL_KEY_ESCAPE) != keys.end()) {
			return;
		}
		gc.set_translate(x, y);
		map.draw(gc);
		
		window.flip(1);

		CL_KeepAlive::process(0);
	}
}

void Game::onCloseWindow(void) {
	run = false;
}

void Game::onKeyUp(const CL_InputEvent &key, const CL_InputState &state) {
	keys.erase(key.id);
}

void Game::onKeyDown(const CL_InputEvent &key, const CL_InputState &state) {
	keys.insert(key.id);
}

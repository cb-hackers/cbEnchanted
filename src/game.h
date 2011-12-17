#ifndef GAME_H
#define GAME_H

class Game {
	public:
		void Run(void);
	private:
		void onKeyUp(const CL_InputEvent &key, const CL_InputState &state);
		void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
		void onCloseWindow(void);
		
		bool run;
		set <int32_t> keys;
};

#endif

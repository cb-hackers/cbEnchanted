#include "cbinput.h"
#include "utf8.h"

CBInput::CBInput(string pPrompt, string pReplace) {
	prompt = pPrompt;
	replace = pReplace;
	content = "";
	creationTime = al_get_time();
}

CBInput::~CBInput() {
	// ...
}

/** Returns the text that is to be displayed. */
string CBInput::getDisplayString() {
	string ret;

	if (replace != "") {
		ret.reserve(prompt.size() + content.size() * replace.size());
		ret.append(prompt);
		for (size_t i = 0; i < content.size(); i++) {
			ret.append(replace);
		}
	}
	else {
		ret.reserve(prompt.size() + content.size() + 1);
		ret.append(prompt);
		ret.append(content);
		// Add "_" every 500ms
		if (fmod((al_get_time() - creationTime) * 1000.0, 1000.0) < 500.0) {
			ret.append(1, '_');
		}
	}

	return ret;
}

/** Handles KEY_CHAR event for this input */
void CBInput::keyChar(ALLEGRO_EVENT *e) {
	if (e->keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
		if (!content.empty()) {
			content.erase(content.end() - 1);
		}
	}
	else if (e->keyboard.unichar > 0) {
		string utf8str;
		vector<unsigned int> utf32char(1);
		utf32char[0] = e->keyboard.unichar;
		utf8::utf32to8(utf32char.begin(), utf32char.end(), back_inserter(utf8str));
		// TODO: Convert utf8str to Windows-1252
		content.append(utf8str);
	}
}

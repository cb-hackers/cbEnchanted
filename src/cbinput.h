#ifndef CBINPUT_H
#define CBINPUT_H
#include "precomp.h"
#include <string>
#include <sstream>

/** Implements the stuff behind Input() */
class CBInput {
	public:
		CBInput(string pPrompt, string pReplace);
		~CBInput();
		/** Sets the input color. */
		void setColor(const ALLEGRO_COLOR &c) { color = c; }
		/** Gets the input color. */
		const ALLEGRO_COLOR &getColor() const { return color; }

		/** Returns the contents of this input */
		string getContent() const { return content; }
		/** Returns the text that is to be drawn. */
		string getDisplayString();

		/** Sets location of the prompt on screen */
		void setLocation(int32_t x, int32_t y) { locationX = x; locationY = y; }
		/** Gets X-location of the prompt on screen */
		int32_t getLocationX() { return locationX; }
		/** Gets Y-location of the prompt on screen */
		int32_t getLocationY() { return locationY; }

		/** Handles KEY_CHAR event for this input */
		void keyChar(ALLEGRO_EVENT *e);
	private:
		/** Holds the stream that is written to when typed */
		stringstream sstream;
		/** Holds the current string that is returned when needed */
		string content;

		/** The string that is displayed before any content. */
		string prompt;
		/** When returning the display string, this string replaces all input characters. */
		string replace;

		/** Location of the prompt on screen */
		int32_t locationX, locationY;

		/** Color of this input */
		ALLEGRO_COLOR color;

		/** The returned value of al_get_time() when this input was created. */
		double creationTime;
};

#endif // CBINPUT_H

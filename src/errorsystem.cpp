#include "errorsystem.h"
#include "cbenchanted.h"
#include "debug.h"
#include <allegro5/allegro_native_dialog.h>

/** A dull constructor. */
ErrorSystem::ErrorSystem() {
	errorCount = 0;
	isFatalByDefault = false;
	isSuppressed = false;
	lastError.fatal = false;
	lastError.message = "";
	lastError.heading = "";
	lastError.title = "";
	cb = CBEnchanted::instance();
}

/** A dull desctructor. */
ErrorSystem::~ErrorSystem() {
	// ...
}

/** Creates a new error.
 *
 * @returns True if program should continue going forward.
 */
bool ErrorSystem::createError(std::string heading, std::string message, std::string title) {
	if (isErrorMessagesOn) {
		lastError.fatal = isFatalByDefault;
		lastError.message = message;
		lastError.heading = heading;
		lastError.title = title;
		printf((string(title) + string(" -- ") + string(heading) + string(" -- ") + string(message) + string("\n")).c_str());
	}
	else {
		// The developer wanted only MAVs, so he'll get fatal MAVs only.
		lastError.fatal = true;
		lastError.message = "";
		lastError.heading = "Memory access violation";
		lastError.title = "Error!";
	}
	errorCount++;

	return this->execLastError();
}

/** Creates a new fatal error. */
void ErrorSystem::createFatalError(std::string heading, std::string message, std::string title) {
	if (isErrorMessagesOn) {
		lastError.fatal = true;
		lastError.message = message;
		lastError.heading = heading;
		lastError.title = title;
		printf((string(title) + string(" -- ") + string(heading) + string(" -- ") + string(message) + string("\n")).c_str());
	}
	else {
		// The developer wanted only MAVs, so he'll get fatal MAVs only.
		lastError.fatal = true;
		lastError.message = "";
		lastError.heading = "Memory access violation";
		lastError.title = "Error!";
	}
	errorCount++;

	this->execLastError();
}


/** Sets default fatality of all normal errors. */
void ErrorSystem::setDefaultFatality(bool fatality) {
	isFatalByDefault = fatality;
}

/** Sets whether message boxes are displayed or not. */
void ErrorSystem::setSuppressed(bool suppress) {
	isSuppressed = suppress;
}

/** Sets whether errors contain debug messages or just plain "Memory Access Violation". */
void ErrorSystem::setErrorMessages(bool showErrors) {
	isErrorMessagesOn = showErrors;
}

/** Does the message box thingy to show the error.
 *
 * @returns True if program should continue going forward.
 */
bool ErrorSystem::execLastError() {
	if (errorCount == 0) return true;

	if (lastError.fatal) {
		al_show_native_message_box(
					cb->getWindow(),
					lastError.title.c_str(),
					lastError.heading.c_str(),
					lastError.message.c_str(),
					NULL,
					ALLEGRO_MESSAGEBOX_ERROR
		);
		cb->stop();
		return false;
	}
#ifndef _WIN32 // Allegro doesn't support custom messagebox buttons with Windows
	else if (!isSuppressed) {
		int ret = al_show_native_message_box(
					cb->getWindow(),
					lastError.title.c_str(),
					lastError.heading.c_str(),
					lastError.message.c_str(),
					"Abort|Continue|Suppress errors",
					ALLEGRO_MESSAGEBOX_OK_CANCEL
		);
		switch (ret) {
			case 0: // No buttons clicked
			case 2: // User clicked continue
				return true;
			case 1: // User clicked abort
				cb->stop();
				return false;
			case 3: // Suppress errors
				isSuppressed = true;
				return true;
			default:
				FIXME("Undefined messagebox return value %i in ErrorSystem::execLastError()", ret);
				cb->stop();
				return false;
		}
	}
#else
	else if (!isSuppressed) {
		int ret = al_show_native_message_box(
					cb->getWindow(),
					lastError.title.c_str(),
					lastError.heading.c_str(),
					(lastError.message + "\n\nDo you still want to continue program execution?").c_str(),
					NULL,
					ALLEGRO_MESSAGEBOX_YES_NO
		);
		if (ret == 1) {
			// Every 5th error ask the user whether they'd like to suppress future errors.
			if (errorCount % 5 == 0) {
				int suppressMsg = al_show_native_message_box(
							cb->getWindow(),
							"Suppress future errors?",
							"Do you want to suppress future non-fatal errors?",
							"",
							NULL,
							ALLEGRO_MESSAGEBOX_YES_NO
				);
				if (suppressMsg == 1) {
					isSuppressed = true;
				}
			}
			return true;
		}
		else {
			cb->stop();
			return false;
		}
	}
#endif // _WIN32

	// If errors are suppressed, return true.
	return true;
}

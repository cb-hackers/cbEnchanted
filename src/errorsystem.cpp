#include "errorsystem.h"
#include "cbenchanted.h"
#include "debug.h"
#include "gfxinterface.h"
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
#ifdef _WIN32

#include "util.h"
#include <allegro5/allegro_windows.h>
// *******************************************
// Code related to custom message boxes
// *******************************************

// Declare functions
INT CBTMessageBox(HWND,LPWSTR,LPWSTR,UINT);
LRESULT CALLBACK CBTProc(INT, WPARAM, LPARAM);

// Global hook for message boxes
HHOOK hhk;

// Create the custom message box
INT CBTMessageBox(HWND hwnd, LPWSTR lpText, LPWSTR lpCaption, UINT uType) {
	hhk = SetWindowsHookEx(WH_CBT, &CBTProc, 0, GetCurrentThreadId());
	return MessageBoxW(hwnd, lpText, lpCaption, uType);
}

// Callback that handles the modification of the message box
LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam) {
	HWND hChildWnd; // msgbox is "child"
	// notification that a window is about to be activated
	// window handle is wParam
	if (nCode == HCBT_ACTIVATE) {
		// set window handles
		hChildWnd = (HWND)wParam;
		UINT result;

		// Set buttons
		if (GetDlgItem(hChildWnd, IDABORT) != NULL) {
			result = SetDlgItemText(hChildWnd, IDABORT, L"Abort");
		}
		if (GetDlgItem(hChildWnd, IDRETRY) != NULL) {
			result = SetDlgItemText(hChildWnd, IDRETRY, L"Continue");
		}
		if (GetDlgItem(hChildWnd, IDIGNORE) != NULL) {
			result = SetDlgItemText(hChildWnd, IDIGNORE, L"Ignore");
		}

		// exit CBT hook
		UnhookWindowsHookEx(hhk);
	}
	// otherwise, continue with any possible chained hooks
	else CallNextHookEx(hhk, nCode, wParam, lParam);
	return 0;
}
#endif // _WIN32

/** A dull constructor. */
ErrorSystem::ErrorSystem() {
	errorCount = 0;
	isFatalByDefault = false;
	isErrorMessagesOn = true;
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

/** Sets an error text to be suppressed. */
void ErrorSystem::suppressError(std::string errortxt) {
	suppressedErrors.insert(errortxt);
}

/** Checks whether an error text is suppressed. */
bool ErrorSystem::isSuppressed(std::string errortxt) {
	return suppressedErrors.count(errortxt) ? true : false;
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

	string concatError = lastError.title + " -- " + lastError.heading;
	if (lastError.message != "") {
		concatError += " -- " + lastError.message;
	}

	// If this error is not fatal and it is set to be suppressed, return true here already
	if (!lastError.fatal && this->isSuppressed(concatError)) {
		return true;
	}

#ifdef _WIN32
	// Windows wants it's console output in codepage 1252
	std::cerr << utf8toCP1252(concatError) << std::endl;
#else
	std::cerr << concatError << std::endl;
#endif

	if (lastError.fatal) {
		al_show_native_message_box(
					cb->gfxInterface->getWindow(),
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
	int ret = al_show_native_message_box(
				cb->gfxInterface->getWindow(),
				lastError.title.c_str(),
				lastError.heading.c_str(),
				lastError.message.c_str(),
				"Abort|Continue|Suppress this error",
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
			this->suppressError(concatError);
			return true;
		default:
			FIXME("Undefined messagebox return value %i in ErrorSystem::execLastError()", ret);
			cb->stop();
			return false;
	}
#else
	string message;
	if (lastError.message.empty()) {
		message = lastError.heading;
	}
	else {
		message = lastError.heading + "\n\n" + lastError.message;
	}

	// Convert message and title to utf-16 with the amazing UTF8-CPP library
	wstring wideMsg = utf8ToUtf16(message);
	wstring wideTitle = utf8ToUtf16(lastError.title);

	int ret = CBTMessageBox(al_get_win_window_handle(cb->getWindow()), &wideMsg[0], &wideTitle[0], MB_ABORTRETRYIGNORE | MB_ICONERROR);
	switch (ret) {
		case 0: // No buttons clicked
		case IDRETRY: // User clicked continue
			return true;
		case IDABORT: // User clicked abort
			cb->stop();
			return false;
		case IDIGNORE: // Suppress errors
			this->suppressError(concatError);
			return true;
		default:
			FIXME("Undefined messagebox return value %i in ErrorSystem::execLastError()", ret);
			cb->stop();
			return false;
	}
#endif // _WIN32
}

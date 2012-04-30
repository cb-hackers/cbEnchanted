#ifndef ERRORSYSTEM_H
#define ERRORSYSTEM_H

#include "precomp.h"
#include "cbenchanted.h"
#include <string>
#include <vector>

/** A generic, portable error system. */
class ErrorSystem {
	public:
		/** A dull constructor. */
		ErrorSystem();
		/** A dull desctructor. */
		~ErrorSystem();

		/** Creates a new error. */
		bool createError(std::string heading, std::string message = "", std::string title = "Error!");

		/** Creates a new fatal error. */
		void createFatalError(std::string heading, std::string message = "", std::string title = "Fatal error!");

		/** Sets default fatality of all normal errors. */
		void setDefaultFatality(bool fatality);

		/** Sets whether message boxes are displayed or not. */
		void setSuppressed(bool suppress);

		/** Sets whether errors contain debug messages or just plain "Memory Access Violation". */
		void setErrorMessages(bool showErrors);

	private:
		/** A private struct for easily saving errors */
		struct Error {
			/** The error message */
			std::string message;
			/** Content title */
			std::string heading;
			/** Window title */
			std::string title;
			/** Is this message fatal */
			bool fatal;
		};

		/** A pointer to the CBEnchanted instance */
		CBEnchanted *cb;

		/** Last error is stored here. */
		Error lastError;

		/** Amount of errors received. */
		uint32_t errorCount;

		/** Are errors fatal by default. */
		bool isFatalByDefault;

		/** Are errors suppressed so that errors are only logged to console (no native message box). */
		bool isSuppressed;

		/** Should errors display only "Memory Access Violation" or the actual error text. */
		bool isErrorMessagesOn;

		/** Does the message box thingy to show the error. */
		bool execLastError();

};

#endif // ERRORSYSTEM_H

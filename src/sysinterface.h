#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H
#include <string>
#include "precomp.h"
#include "cbdll.h"

using namespace std;

class CBEnchanted;

class SysInterface {
	public:
		SysInterface();
		~SysInterface();
		void initializeSysInterface();

		void commandWait(void);
		void commandSetWindow(void);
		void commandMakeError(void);
		void commandSaveProgram(void);
		void commandLoadProgram(void);
		void commandGotoSavedLocation(void);
		void commandFrameLimit(void);
		void commandEncrypt(void);
		void commandDecrypt(void);
		void commandCallDLL(void);
		void commandErrors(void);

		void commandEnd(void);

		void functionDate(void);
		void functionTime(void);
		void functionTimer(void);
		void functionCommandLine(void);
		void functionGetEXEName(void);
		void functionFPS(void);
		void functionCrc32(void);

		/** Asks for user confirmation before closing the program, returns true if program should be terminated. */
		bool askForExit();
	private:
		CBEnchanted *cb;

		/** Window title */
		string windowTitle;

		/** Confirmation string that is shown when user is trying to close the program manually. */
		string confirmationStr;

		/** Unordered map for dlls*/
		unordered_map<string, CBDLL*> dlls;
};

#endif

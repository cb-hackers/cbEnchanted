#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H

class CBEnchanted;

class SysInterface {
	public:
		SysInterface();

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
	private:
		InterfaceSystem *sys;
};

#endif

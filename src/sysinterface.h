#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H

class CBEmu;

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
		
		void functionTimer(void);
	private:
		CBEmu *cb;
};

#endif

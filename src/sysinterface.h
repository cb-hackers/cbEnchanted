#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H

class CBEmu;

class SysInterface {
	public:
		SysInterface();
		
		void commandSetWindow(void);
		void commandEnd(void);
		
		void functionTimer(void);
	private:
		CBEmu *cb;
};

#endif

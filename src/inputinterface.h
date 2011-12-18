#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

class CBEmu;

class InputInterface {
	public:
		InputInterface();
		
		void commandCloseInput(void);
		void commandWaitKey(void);
		void commandClearKeys(void);
		void commandPositionMouse(void);
		void commandWaitMouse(void);
		void commandShowMouse(void);
		void commandClearMouse(void);
		void commandSAFEEXIT(void);
		
		void functionWaitKey(void);
	private:
		CBEmu *cb;
};

#endif

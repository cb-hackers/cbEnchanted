#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

class CBEmu;

class InputInterface {
	public:
		InputInterface();
		
		void commandWaitKey(void);
		void functionWaitKey(void);
	private:
		CBEmu *cb;
};

#endif

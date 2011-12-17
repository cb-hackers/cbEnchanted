#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEmu;

class TextInterface {
	public:
		TextInterface();
		
		void commandPrint(void);
	private:
		CBEmu *cb;
};

#endif

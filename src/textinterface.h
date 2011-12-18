#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEmu;

class TextInterface {
	public:
		TextInterface();
		
		void commandSetFont(void);
		void commandDeleteFont(void);
		void commandText(void);
		void commandCenterText(void);
		void commandVerticalText(void);
		void commandPrint(void);
		void commandWrite(void);
		void commandLocate(void);
		void commandAddText(void);
		void commandClearText(void);
	private:
		CBEmu *cb;
};

#endif

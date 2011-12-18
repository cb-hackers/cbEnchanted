#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEnchanted;

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
		
		void functionLoadFont(void);
		void functionTextWidth(void);
		void functionTextHeight(void);
	private:
		CBEnchanted *cb;
};

#endif

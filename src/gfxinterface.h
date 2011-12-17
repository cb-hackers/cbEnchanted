#ifndef GFXINTERFACE_H
#define GFXINTERFACE_H

class CBEmu;

class GfxInterface {
	public:
		GfxInterface();
		
		void functionScreen(void);
		void commandScreen(void);
		void commandClsColor(void);
		void commandColor(void);
		void commandCircle(void);
		void commandDrawScreen(void);
		
		CL_DisplayWindow *getWindow(void) { return this->window; }
	private:
		CBEmu *cb;
		
		CL_Slot slotInputDown;
		
		vector <CL_GraphicContext *> graphicContexts;
		CL_DisplayWindow *window;
};

#endif

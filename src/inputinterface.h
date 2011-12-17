#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

class CBEmu;

class InputInterface {
	public:
		InputInterface();
		
		void commandWaitKey(void);
		void functionWaitKey(void);
		
		void stopKeyWait(uint32_t key) { keyWait = key; }
		
		void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	private:
		CBEmu *cb;
		
		bool keyWait;
};

#endif

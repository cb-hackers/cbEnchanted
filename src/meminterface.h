#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H

class CBEmu;

class MemInterface {
	public:
		MemInterface();
		~MemInterface();
		
		void commandDeleteMEMBlock(void);
		void commandResizeMEMBlock(void);
		void commandMemCopy(void);
		void commandPokeByte(void);
		void commandPokeShort(void);
		void commandPokeInt(void);
		void commandPokeFloat(void);
		
		void functionMakeMEMBlock(void);
		void functionMEMBlockSize(void);
		void functionPeekByte(void);
		void functionPeekShort(void);
		void functionPeekInt(void);
		void functionPeekFloat(void);
	private:
		CBEmu *cb;
	
};

#endif

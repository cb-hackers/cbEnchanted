#ifndef MEMINTERFACE_H
#define MEMINTERFACE_H
#include "precomp.h"
class CBEnchanted;

class MemInterface {
	public:
#ifndef CBE_LIB
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
#endif
		static int32_t getMEMBlockSize(uint8_t *mem) { return *(int32_t*)mem; }
		static uint8_t *getMEMBlockData(uint8_t *mem) { return mem + 4; }
		uint8_t* getMemblock(int32_t id);
	private:
		CBEnchanted *cb;
		std::map<int32_t, uint8_t*> memblockMap;

		int32_t nextId() const {
			static int32_t idCounter = 0;
			return ++idCounter;
		}

};

#endif

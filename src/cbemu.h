#ifndef CBEMU_H
#define CBEMU_H

#include "cbvariableholder.h"
#include "mathinterface.h"
#include "stringinterface.h"
#include "textinterface.h"
#include "fileinterface.h"
#include "meminterface.h"
#include "inputinterface.h"
#include "sysinterface.h"
#include "soundinterface.h"
#include "animinterface.h"
#include "imageinterface.h"
#include "gfxinterface.h"
#include "objectinterface.h"
#include "camerainterface.h"
#include "mapinterface.h"
#include "effectinterface.h"

#define LOG_DEBUG    0x01
#define LOG_INFO     0x02
#define LOG_FIXME    0x04
#define LOG_HCDEBUG 0x08

//#define LOG_LEVEL (LOG_DEBUG | LOG_INFO | LOG_FIXME | LOG_HCDEBUG)
#define LOG_LEVEL 0

#if LOG_LEVEL & 1
#define DEBUG(str, ...) printf(str, ##__VA_ARGS__);
#else
	#define DEBUG(str, ...)
#endif

#if (LOG_LEVEL >> 1) & 1
#define INFO(str, ...) printf(str, ##__VA_ARGS__);
#else
	#define INFO(str, ...)
#endif

#if (LOG_LEVEL >> 2) & 1
#define FIXME(str, ...) printf(str, ##__VA_ARGS__);
#else
	#define FIXME(str, ...)
#endif

#if (LOG_LEVEL >> 3) & 1
#define HCDEBUG(str, ...) printf(str, ##__VA_ARGS__);
#else
	#define HCDEBUG(str, ...)
#endif


class CBEmu : public CBVariableHolder, public MathInterface, public GfxInterface, public StringInterface, public TextInterface, public InputInterface, public SysInterface {
	public:
		CBEmu();
		~CBEmu();
	
		void init(string file);		
		void run();
		void stop();
		void cleanup();
		bool isSafeExit(){return safeExit;}
	private:
		
		char *code;
		uint32_t cpos;
		
		bool initialized;
		bool running;
		
		bool safeExit;

		map <uint32_t, uint32_t> offsets;
		map <uint32_t, uint32_t> roffsets;
		
		void handleSetInt(void);
		void handleSetFloat(void);
		void handleFunction(void);
		void handleCommand(void);
		void handlePushInt(void);
		void handlePushVariable(void);
		void handlePushSomething(void);
		void handleMathOperation(void);
		void handleJump(void);
		void handleIncVar(void);
		
		void uselessShitHandler(void);
		
		void command97_98(void);
		void command99(void);
		void commandGoto(void);
		void commandDim(void);
		void commandArrayAssign(void);
};

#endif

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

#define LOG_DEBUG    0b00000001
#define LOG_INFO     0b00000010
#define LOG_FIXME    0b00000100
#define LOG_HCDEBUG 0b00001000

#define LOG_LEVEL (LOG_DEBUG | LOG_INFO | LOG_FIXME | LOG_HCDEBUG)

#if LOG_LEVEL & 1
	#define DEBUG(str, args...) cl_log_event("DEBUG", str, ##args);
#else
	#define DEBUG(str, args...)
#endif

#if (LOG_LEVEL >> 1) & 1
	#define INFO(str, args...) cl_log_event("INFO", str, ##args);
#else
	#define INFO(str, args...)
#endif

#if (LOG_LEVEL >> 2) & 1
	#define FIXME(str, args...) cl_log_event("FIXME", str, ##args);
#else
	#define FIXME(str, args...)
#endif

#if (LOG_LEVEL >> 3) & 1
	#define HCDEBUG(str, args...) cl_log_event("HCDEBUG", str, ##args);
#else
	#define HCDEBUG(str, args...)
#endif


class CBEmu : public CBVariableHolder, public MathInterface, public GfxInterface, public StringInterface, public TextInterface, public InputInterface, public SysInterface {
	public:
		CBEmu();
		
		void init(string file);		
		void run();
		void stop();
		void cleanup();
	private:
		map <uint32_t, boost::function<void()> > functions;
		map <uint32_t, boost::function<void()> > commands;
		map <uint32_t, boost::function<void()> > handlers;
		
		char *code;
		uint32_t cpos;
		
		bool initialized;
		bool running;
		
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

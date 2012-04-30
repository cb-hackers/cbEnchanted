#ifndef CBENCHANTED_H
#define CBENCHANTED_H

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
#include "debug.h"
#include <vector>
#include "customfunctionhandler.h"

using std::vector;

class GfxInterface;
class ErrorSystem;


class CBEnchanted :
		public CBVariableHolder,
		public MathInterface,
		public GfxInterface,
		public StringInterface,
		public TextInterface,
		public InputInterface,
		public SysInterface,
		public ObjectInterface,
		public MemInterface,
		public FileInterface,
		public MapInterface,
		public CameraInterface,
		public ImageInterface,
		public EffectInterface,
		public AnimInterface,
		public SoundInterface

{
	public:
		CBEnchanted();
		~CBEnchanted();

		static CBEnchanted *instance();
		bool init(const char* file);
		void run();
		void stop();
		void cleanup();
		bool isSafeExit(){ return safeExit; }
		inline void setFrameLimit(float frameLimit){framesMs = 1000.0 / frameLimit;}
		inline float getFrameLimit(){return framesMs;}
		ALLEGRO_EVENT_QUEUE *getEventQueue(){return eventQueue;}

		/** Error system for public access */
		ErrorSystem *errors;
	private:

		char *code;
		char *codeBase;
		vector <char*> pos;
		bool initialized;
		bool running;
		bool safeExit;
		float framesMs;
		ALLEGRO_EVENT_QUEUE * eventQueue;
		CustomFunctionHandler customFunctionHandler;
		int32_t selectValue;

		void handlePushFuncptr(void);
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
		void handleIncGlobalVar(void);
		void handlePushTypeMemberVariable(void);
		void handleCustomFunctionCall(void);

		void commandSelect(void);
		void commandCase(void);
		void commandFunction(void);
		void commandSetGlobalVariable(void); //80
		void commandSetArrayNumbers(void); //97
		void commandSetGlobalVariableNumbers(void); //98
		void commandType(void);
		void commandSetTypeMemberField(void);
		void commandSetVariableNumbers(void);
		void commandGoto(void);
		void commandGosub(void);
		void commandReturn(void);
		void commandDim(void);
		void commandReDim(void);
		void commandClearArray(void);
		void commandInsert(void);
		void commandDelete(void);
		void commandSetVariable(void);
		void commandArrayAssign(void);
		void functionNew(void);
		void functionFirst(void);
		void functionLast(void);
		void functionBefore(void);
		void functionAfter(void);
		void functionRead(void);
		void functionConvertToInteger(void);
		void functionConvertToType(void);

		uint32_t popArrayDimensions1(int32_t arrayId, int32_t n, int32_t type);
		uint32_t popArrayDimensions2(int32_t arrayId, int32_t n, int32_t type);
};

#endif

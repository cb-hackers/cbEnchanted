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
#include <map>
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
#ifndef CBE_LIB
		CBEnchanted();
		~CBEnchanted();
		bool init(const char* file, int argc, char **argv);
		void run();
		void cleanup();
#endif
		void dllInit();
		void stop();
		static CBEnchanted *instance();
		bool isSafeExit() const { return safeExit; }
		void setSafeExit(bool toggled) { safeExit = toggled; }
		bool isSmooth2D() const { return smooth2d; }
		void setSmooth2D(bool toggled);
		inline void setFrameLimit(float pFrameLimit) { frameLimit = pFrameLimit; }
		inline float getFrameLimit() const { return frameLimit; }
		ALLEGRO_EVENT_QUEUE *getEventQueue() { return eventQueue; }
		CustomFunctionHandler *getCustomFunctionHandler() {return &customFunctionHandler;}

		/** Error system for public access */
		ErrorSystem *errors;
		/** Returns commandline arguments in a single string. */
		std::string getCommandLine() { return cmdLine; }
	private:

		char *code;
		char *codeBase;
		vector <char*> pos;
		bool initialized;
		bool running;
		bool safeExit;
		bool smooth2d;
		bool clearArray;
		float frameLimit;
		ALLEGRO_EVENT_QUEUE * eventQueue;
		CustomFunctionHandler customFunctionHandler;
		Any selectValue;

#ifndef CBE_LIB
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
		void handleData(void);

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
		void commandRestore(void);
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
		template<class T> uint32_t popArrayDimensions(Array<T> &a, int32_t n);

		void parseCustomFunction(uint32_t i, map<int32_t, int32_t> &tempMap);
#endif

		/** A map for functionConvertToInteger() and functionConvertToType(). */
		std::map<int32_t, void*> typeConvertMap;
		/** A reversed map for functionConvertToInteger() and functionConvertToType(). */
		std::map<void*, int32_t> rTypeConvertMap;

		/** Returns new ID for typeConvertMap */
		int32_t nextTypeId() {static int32_t idCounter = 0; return ++idCounter;}

		/** Commandline arguments, concatenated to a single string. */
		std::string cmdLine;

		/** Current datalist readpoint */
		uint32_t dataPos;
};



#endif

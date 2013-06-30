#include "precomp.h"
#include "cbenchanted.h"
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
#include "cbvariableholder.h"
#include "mathoperations.h"
#include "errorsystem.h"
#include <iostream>

static CBEnchanted *cbInstance;

enum OpCode {
	SetInt = 65,
	SetFloat,
	Command,
	Data,
	PushInt = 73,
	PushSomething,
	Jump = 78,
	MathOperation,
	IncVar,
	IncGlobalVar,
	PushTypeMemberVariable = 84,
	PushFuncPtr,
	PushVariable,
	Function = 90,
	Unknown1 = 97,
	Unknown2,
	Unknown3,
	CustomFunctionCall = 100
};

enum MathematicOperation {
	UnaryMinus = 1,
	UnaryPlus,
	Power,
	Addition,
	Subtraction,
	Multiplication,
	Division,
	Modulo,
	Shl,
	Shr,
	Sar,
	LessThan,
	GreaterThan,
	Equal,
	NotEqual,
	LessThanOrEqual,
	GreaterThanOrEqual,
	AND,
	OR,
	XOR,
	NOT
};

CBEnchanted *CBEnchanted::instance() {
	return cbInstance;
}

void CBEnchanted::stop() {
	running = false;
}

void CBEnchanted::dllInit() {
	cbInstance = this;
}

#ifndef CBE_LIB
CBEnchanted::CBEnchanted() {
	cbInstance = this;
	initialized = false;
	running = false;
	safeExit = 1;
	smooth2d = false;
	frameLimit = -1;
	clearArray = true;
}

CBEnchanted::~CBEnchanted() {
	//delete[] code;
}

/*
 * CBEnchanted::run - Interpret CoolBasic bytecode
 */
void CBEnchanted::run() {
	std::cout << sizeof(OpCode) << std::endl;
	// Make sure that we are initialized properly to avoid crashing
	assert(initialized == true);
	// Run until told to quit
	running = true;
	while (running) {
		uint32_t opCode = (uint32_t)*(uint8_t *)(code++);//[cpos++];

		HCDEBUG("[%i]: OpCode: %i", code - codeBase, opCode);
		switch (opCode) {
			case OpCode::SetInt: handleSetInt(); break;
			case OpCode::SetFloat: handleSetFloat(); break;
			case OpCode::Command: handleCommand(); break;
			case OpCode::Data: handleData(); break;
			case OpCode::PushInt: handlePushInt(); break;
			case OpCode::PushSomething: handlePushSomething(); break;
			case OpCode::Jump: handleJump(); break;
			case OpCode::MathOperation: handleMathOperation(); break;
			case OpCode::IncVar: handleIncVar(); break;
			case OpCode::IncGlobalVar: handleIncGlobalVar(); break;
			case OpCode::PushTypeMemberVariable: handlePushTypeMemberVariable(); break;
			case OpCode::PushFuncPtr: handlePushFuncptr(); break;
			case OpCode::PushVariable: handlePushVariable(); break;
			case OpCode::Function: handleFunction(); break;
			case OpCode::Unknown1:
			case OpCode::Unknown2:
			case OpCode::Unknown3: break;
			case OpCode::CustomFunctionCall: handleCustomFunctionCall(); break;
			default: FIXME("Unimplemented handler: %i", opCode);
		}
	}
}

/*
 * CBEnchanted::init - Initialize the interpreter
 */
bool CBEnchanted::init(const char* file, int argc, char** argv) {
#ifdef _WIN32
	// Set console output codepage to Windows-1252
	SetConsoleOutputCP(1252);
	#if !defined(TEST) && defined(NDEBUG)
		// Not on test or debug mode and on Windows, hide the console immediately.
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#endif
#endif

	mathInterface = new MathInterface();
	gfxInterface = new GfxInterface();
	stringInterface = new StringInterface();
	textInterface = new TextInterface();
	inputInterface = new InputInterface();
	sysInterface = new SysInterface();
	objectInterface = new ObjectInterface();
	memInterface = new MemInterface();
	fileInterface = new FileInterface();
	mapInterface = new MapInterface();
	cameraInterface = new CameraInterface();
	imageInterface = new ImageInterface();
	effectInterface = new EffectInterface();
	animInterface = new AnimInterface();
	soundInterface = new SoundInterface();
		
	INFO("Initializing");

	// Store commandline parameters. If testable exe, skip first one.
	ostringstream ss;
	int argStart = (file != NULL) ? 2 : 1;
	for (int i = argStart; i < argc; i++) {
		ss << argv[i];
		ss << ' ';
	}
	cmdLine = ss.str();
	if (!cmdLine.empty()) {
		boost::algorithm::trim(cmdLine);
	}

	// Initialize error system first, because we can.
	errors = new ErrorSystem();

	// Boot up allegro
	if (!al_init()) {
		errors->createFatalError("Initialization error", "Failed to initialize Allegro");
		return false;
	}

	int32_t startPos; // Beginning of the CoolBasic data
	int32_t endPos; // End of the executable

	uint32_t nStrings; // Number of strings
	uint32_t size; // Length of CoolBasic data

	// Input file, opened for reading only
	ifstream input;

	// If file is NULL, we need to find the real path to the current executable
	if (file == NULL) {
		ALLEGRO_PATH *tmpPath = al_get_standard_path(ALLEGRO_EXENAME_PATH);
		input.open(al_path_cstr(tmpPath, ALLEGRO_NATIVE_PATH_SEP), ios::in | ios::binary);
		al_destroy_path(tmpPath);
	}
	else {
		input.open(file, ios::in | ios::binary);
	}

	if (!input.is_open()) {
		if (file != NULL) {
			errors->createFatalError("Initialization error", "Can't open exe!\n" + string(file));
		}
		else {
			errors->createFatalError("Initialization error", "Can't open exe!");
		}
		return false;
	}

	// Find out size of the file by seeking to it's end
	input.seekg(-4, ios::end);
	endPos = input.tellg();
	
	// Read the offset for CB-bytecode and go there
	input.read((char *)(&startPos), 4);
	input.seekg(24 - startPos, ios::end);
	
	// Number of strings
	input.read((char *)(&nStrings), 4);

	// Read and decrypt strings
	initStrings(nStrings);
	const char key[] = "Mark Sibly is my idol!";
	for (uint32_t i = 1; i <= nStrings; i++) {
		uint32_t len;
		input.read((char *)(&len), 4);
		string s;
		bool requireEncoding = false;
		unsigned char c;
		for (uint32_t j = 0; j < len; j++) {
			input >> c;
			c = c - key[j % 22];
			if (c > 178 || c == 0) {
				requireEncoding = true;
				if (c > 161) {
					c--;
				}
				if (c == 0) {
					c = 255;
				}
			}
			s += c;
		}
		ISString iss(s);
		iss.requireEncoding(requireEncoding);
		setString(i, s);
	}

	// Skip useless data and go to beginning of the bytecode
	input.seekg(31, ios::cur);
	char pushIntCheck;
	input.read(&pushIntCheck, 1);
	if (pushIntCheck == 73) {//PushInt
		input.seekg(-1, ios_base::cur);
	}

	startPos = input.tellg();
	// Read code to memory and close the file
	size = endPos - startPos;
	code = new char [size];
	codeBase = code;

	input.read(code, size);
	input.close();

	std::map <uint32_t, uint32_t> offsets;
	//std::map <uint32_t, uint32_t> roffsets;
	// Precalculation
	// ToDo:	JIT-optimization?
	// 			Handle functions and types
	uint32_t ncmd = 0;
	uint32_t i = 0;
	HCDEBUG("Bytecode:");
	if (*(int32_t *)(code + i) == 0) {
		i += 4;
	}
	while (i < size) {
		offsets[ncmd] = i;
		//roffsets[i] = ncmd;
		uint8_t cmd = *(uint8_t *)(code + i);
		HCDEBUG("[%i] %i", i, (int)cmd);
		ncmd++;
		i++;
		switch (cmd) {
			case 65:
			case 66:
			case 67:
			case 73:
			case 74:
			case 78:
			case 80:
			case 81:
			case 84:
			case 85:
			case 86:
			case 90: i += 4; break;
			case 68:
			case 79: i ++; break;
			default: FIXME("[%i] Unhandled preparsing1: %i", i - 1, (uint32_t) cmd);
		}
	}
	map <int32_t, int32_t> functionMaping;
	//Goto and if
	i = 0;
	if (*(int32_t *)(code + i) == 0) {
		i += 4;
	}
	while (i < size) {
		//roffsets[i] = ncmd;
		uint8_t cmd = *(uint8_t *)(code + i);
		i++;
		switch (cmd) {
			case 65:
			case 66:
			case 73:
			case 74:
			case 80:
			case 81:
			case 84:
			case 86:
			case 90: i += 4; break;
			case 68:
			case 79: i ++; break;
			case 67: {
				int32_t cmdOpCode = (*(int32_t *)(code + i));
				if (cmdOpCode == 12 || cmdOpCode == 21 || cmdOpCode == 6 || cmdOpCode == 62) { //commandGoto, commandGosub, commandSelect, commandRestore
					int32_t id = *(int32_t *)(code + i + 5);
					*(int32_t *)(code + i + 5) = offsets[id];
				}
				if ((*(int32_t *)(code + i)) == 7) { //Command case
					int32_t id = *(int32_t *)(code + i + 10);
					*(int32_t *)(code + i + 10) = offsets[id];
				}
				i += 4;
				break;
			}
			case 85: //pushFunctionPtr
			case 78: { //Jump
				int32_t id = *(int32_t *)(code + i);
				*(int32_t *)(code + i) = offsets[id];
#ifndef DISABLE_CUSTOMS
				if (cmd == 85) { //custom function check
					parseCustomFunction(i, functionMaping);
				}
#endif //DISABLE_CUSTOMS

				i += 4;
				break;
			}

			default: FIXME("[%i] Unhandled preparsing2: %i",i, (uint32_t) cmd);
		}
	}

#ifndef DISABLE_CUSTOMS
	customFunctionHandler.link();
#endif

	eventQueue = al_create_event_queue();
	if (!eventQueue) {
		errors->createFatalError("Initialization error", "Failed to initialize event queue");
		return false;
	}
	//Create screen
	if (!gfxInterface->initializeGfx()) {
		errors->createFatalError("Initialization error", "Failed to initialize graphics");
		return false;
	}
	if (!inputInterface->initializeInputs()) {
		errors->createFatalError("Initialization error", "Failed to initialize inputs");
		return false;
	}
	if (!soundInterface->initializeSounds()){
		errors->createFatalError("Initialization error", "Failed to initialize sounds");
		return false;
	}
	if (!textInterface->initializeFonts()) {
		errors->createFatalError("Initialization error", "Failed to initialize fonts");
		return false;
	}
	imageInterface->initializeImages();
	sysInterface->initializeSysInterface();

	initialized = true;
	INFO("Initialized");
	return true;
}

void CBEnchanted::parseCustomFunction(uint32_t i, map<int32_t, int32_t> &tempMap) {
	CustomFunction func;
	uint32_t i2 = *(int32_t *)(code + i);
	map <int32_t, int32_t>::iterator funcIt = tempMap.find(i2);
	if (funcIt != tempMap.end()) { //Already parsed function
		if (funcIt->second == -1) { //Not custom function
			return;
		}
		*(uint8_t *)(code + i - 1) = 100; //Custom function call
		*(int32_t *)(code + i) = funcIt->second;
		return;
	}
	//command 99
	if (code[i2++] != 73) {
		FIXME("[%i] Unexpected opcode1: %i, expecting 73.", i2, code[i2]);
		goto not_custom_function;
	}
	i2 += 4;

	for (int32_t c = 0; c != 5; c++) {
		if (code[i2++] != 73) {
			FIXME("[%i] Unexpected opcode2: %i, expecting 73.", i2, code[i2]);
			goto not_custom_function;
		}
		i2 += 4;
	}
	if (code[i2++] != 67) {
		FIXME("[%i] Unexpected opcode3: %i, expecting 67.", i2, code[i2]);
		goto not_custom_function;
	}
	if (*(int32_t*)(code + i2) != 99) {
		FIXME("[%i] Unexpected command %i, expecting 79.", i2, code[i2]);
		goto not_custom_function;
	}
	i2 += 4;
	//commandFunction
	int32_t paramCount;
	int32_t opc;
	int32_t comc;
	paramCount = 0; //See line 385
	while ((opc = code[i2]) == 67 && (comc = *(int32_t*)(code + i2 + 1)) == 79) {
		i2 += 6;
		paramCount++;
		i2 += 9;
	}
	int32_t groupId;
	int32_t funcId;

	//Group id
	if (code[i2++] != 73) { //PushInt
		goto not_custom_function;
	}
	groupId = *(int32_t*)(code + i2);
	i2 += 4;
	if (code[i2++] != 65) { //Set int var
		goto not_custom_function;
	}
	i2 += 4;

	//Func id
	if (code[i2++] != 73) { //PushInt
		goto not_custom_function;
	}
	funcId = *(int32_t*)(code + i2);
	i2 += 4;
	if (code[i2++] != 65) { //Set int var
		goto not_custom_function;
	}
	i2 += 4;

	//Return
	if (code[i2++] != 73) { //PushInt
		goto not_custom_function;
	}
	int pushValue;
	pushValue = *(int32_t*)(code + i2); //Not running constructor fixes cross initialization
	if (pushValue != 0) { //Return == 0
		if (pushValue != 2 && pushValue != 5) { //handlePushSomething() IDs of float(2) and string(5)
			goto not_custom_function;
		}
		i2 += 4;
		if (code[i2++] != 74) { //PushSomething
			goto not_custom_function;
		}
		pushValue = *(int32_t*)(code + i2);
		if (pushValue != 0) { //Return 0.0 or ""
			goto not_custom_function;
		}
	}
	i2 += 4;

	if (code[i2++] != 67) { //call command
		goto not_custom_function;
	}
	if (*(int32_t*)(code + i2) != 22) { //Return
		goto not_custom_function;
	}
	func.setFuncId(funcId);
	func.setGroupId(groupId);
	customFunctionHandler.addDefinition(func);
	tempMap[*(int32_t *)(code + i)] = func.getHandle();
	*(uint8_t *)(code + i - 1) = 100; //Custom function call
	*(int32_t *)(code + i) = func.getHandle();
	INFO("Added custom function with handle %i", func.getHandle());
	return;

	//Not custom function
	not_custom_function:
	tempMap[*(int32_t *)(code + i)] = -1;
}

void CBEnchanted::cleanup() {
	soundInterface->cleanupSoundInterface();
}

FORCEINLINE void CBEnchanted::handlePushFuncptr(void) {
	int32_t ptr = *(int32_t *)(code);
	code += 4;

	pos.push_back(code);

	code = codeBase + ptr;
}

void CBEnchanted::commandFunction(void) {
	code++;
	int32_t type = *(int32_t *)(code);
	code += 5;
	int32_t var = *(int32_t *)(code);
	code += 4;

	switch (type) {
		case 1: setIntegerVariable(var, popValue().toInt()); break;
		case 2: setFloatVariable(var, popValue().toFloat()); break;
		case 3:	setStringVariable(var, popValue().toString()); break;
		case 4:	setShortVariable(var, popValue().toShort()); break;
		case 5: setByteVariable(var, popValue().toByte()); break;
	}
}

void CBEnchanted::commandSelect(void) {
#ifdef LOG_LEVEL_HCDEBUG
	HCDEBUG("commandSelect stack dump:");
	internalStack.dump();
#endif
	selectValue = popValue();
	++code;
	code = codeBase + *(int32_t *)(code);
}

void CBEnchanted::commandCase(void) {
#ifdef LOG_LEVEL_HCDEBUG
	HCDEBUG("commandCase stack dump:");
	internalStack.dump();
#endif
	++code;
	int32_t testCount = *(int32_t *)(code);
	code += 5;
	int32_t nextCase = *(int32_t *)(code);
	for (int i = 0; i < testCount; ++i) {
		code += 5;
		bool matched = false;
		uint8_t nextOpcode = *(uint8_t *)(code+4);
		if (nextOpcode == 74) {//Float or string
			int32_t type = *(int32_t *)(code); //pushSomething's type
			code += 5;
			switch (type) {
				case 2: {//Float
					if (selectValue == *(float *)(code)) {
						matched = true;
					}
					break;
				}
				case 5: {//String
					uint32_t strId = *(uint32_t *)(code);
					if (selectValue == getString(strId)) {
						matched = true;
					}
					break;
				}
				default:
					FIXME("Unhandled case");
			}
		}
		else { //Int
			if (selectValue == *(int32_t *)(code)) {
				matched = true;
			}
		}
		if (matched) {
			code += (testCount - i - 1) * 5 + 4;
			return;
		}
	}
	code = codeBase + nextCase;
}

/*
 * CBEnchanted::handleSetInt - Set value of integer
 */
FORCEINLINE void CBEnchanted::handleSetInt(void) {
	uint32_t var = *(uint32_t *)(code);
	code += 4;

	int32_t value = popValue().toInt();
	setIntegerVariable(var, value);
	HCDEBUG("[%i]: Setting int variable %i to value: %i", code - codeBase, var, value);
}

/*
 * CBEnchanted::handleSetFloat - Set value of float
 */
FORCEINLINE void CBEnchanted::handleSetFloat(void) {
	uint32_t var = *(uint32_t *)(code);
	code += 4;

	setFloatVariable(var, popValue().toFloat());
}

/*
 * CBEnchanted::handleCommand - Run command
 */
FORCEINLINE void CBEnchanted::handleCommand(void) {
	uint32_t command = *(uint32_t *)(code);
	code += 4;

	HCDEBUG("Command: %i", command);

	switch (command) {
		/*case 1: commandIf(); break;
		case 3: commandElse(); break;
		case 4: commandElseIf(); break;
		case 5: commandEndIf(); break;*/
		case 6: commandSelect(); break;
		case 7: commandCase(); break;
		/*case 8: commandDefault(); break;
		case 9: commandEndSelect(); break;
		case 10: commandRepeat(); break;
		case 11: commandUntil(); break;*/
		case 12: commandGoto(); break;
		case 21: commandGosub(); break;
		case 22: commandReturn(); break;
		case 42: commandDim(); break;
		case 43: commandReDim(); break;
		case 44: commandClearArray(); break;
		case 56: commandInsert(); break;
		case 57: commandDelete(); break;
		case 62: commandRestore(); break;
		case 65: objectInterface->commandInitObjectList(); break;
		case 69: sysInterface->commandEnd(); break;
		case 70: popValue(); break;
		case 77: commandSetVariable(); break;
		case 78: commandArrayAssign(); break;
		case 79: commandFunction(); break;
		case 80: commandSetGlobalVariable(); break;
		case 81: commandSetTypeMemberField(); break;
		case 95: commandType(); break;
		case 97: commandSetArrayNumbers(); break;
		case 98: commandSetGlobalVariableNumbers(); break;
		case 99: commandSetVariableNumbers(); break;
		case 125: mathInterface->commandRandomize(); break;
		case 201: textInterface->commandSetFont(); break;
		case 202: textInterface->commandDeleteFont(); break;
		case 205: textInterface->commandText(); break;
		case 206: textInterface->commandCenterText(); break;
		case 207: textInterface->commandPrint(); break;
		case 208: textInterface->commandWrite(); break;
		case 209: textInterface->commandLocate(); break;
		case 210: textInterface->commandClearText(); break;
		case 211: textInterface->commandAddText(); break;
		case 212: textInterface->commandVerticalText(); break;
		case 224: inputInterface->commandWaitKey(); break;
		case 225: inputInterface->commandClearKeys(); break;
		case 226: inputInterface->commandPositionMouse(); break;
		case 230: inputInterface->commandWaitMouse(); break;
		case 237: inputInterface->commandShowMouse(); break;
		case 238: inputInterface->commandClearMouse(); break;
		case 239: inputInterface->commandSAFEEXIT(); break;
		case 247: inputInterface->commandCloseInput(); break;
		case 301: memInterface->commandDeleteMEMBlock(); break;
		case 303: memInterface->commandResizeMEMBlock(); break;
		case 304: memInterface->commandMemCopy(); break;
		case 309: memInterface->commandPokeByte(); break;
		case 310: memInterface->commandPokeShort(); break;
		case 311: memInterface->commandPokeInt(); break;
		case 312: memInterface->commandPokeFloat(); break;
		case 323: fileInterface->commandCloseFile(); break;
		case 325: fileInterface->commandSeekFile(); break;
		case 326: fileInterface->commandStartSearch(); break;
		case 327: fileInterface->commandEndSearch(); break;
		case 330: fileInterface->commandChDir(); break;
		case 331: fileInterface->commandMakeDir(); break;
		case 336: fileInterface->commandCopyFile(); break;
		case 337: fileInterface->commandDeleteFile(); break;
		case 338: fileInterface->commandExecute(); break;
		case 341: fileInterface->commandReadByte(); break;
		case 342: fileInterface->commandReadShort(); break;
		case 343: fileInterface->commandReadInt(); break;
		case 344: fileInterface->commandReadFloat(); break;
		case 345: fileInterface->commandReadString(); break;
		case 346: fileInterface->commandReadLine(); break;
		case 348: fileInterface->commandWriteByte(); break;
		case 349: fileInterface->commandWriteShort(); break;
		case 350: fileInterface->commandWriteInt(); break;
		case 351: fileInterface->commandWriteFloat(); break;
		case 352: fileInterface->commandWriteString(); break;
		case 353: fileInterface->commandWriteLine(); break;
		case 401: animInterface->commandStopAnimation(); break;
		case 402: animInterface->commandDrawAnimation(); break;
		case 416: sysInterface->commandEncrypt(); break;
		case 417: sysInterface->commandDecrypt(); break;
		case 423: sysInterface->commandWait(); break;
		case 425: sysInterface->commandSetWindow(); break;
		case 426: sysInterface->commandMakeError(); break;
		case 428: sysInterface->commandSaveProgram(); break;
		case 429: sysInterface->commandLoadProgram(); break;
		case 430: sysInterface->commandGotoSavedLocation(); break;
		case 431: sysInterface->commandFrameLimit(); break;
		case 436: sysInterface->commandCallDLL(); break;
		case 437: sysInterface->commandErrors(); break;
		case 450: soundInterface->commandPlaySound(); break;
		case 451: soundInterface->commandSetSound(); break;
		case 453: soundInterface->commandDeleteSound(); break;
		case 454: soundInterface->commandStopSound(); break;
		case 481: gfxInterface->commandScreen(); break;
		case 482: gfxInterface->commandLock(); break;
		case 483: gfxInterface->commandUnlock(); break;
		case 485: gfxInterface->commandPutPixel(); break;
		case 487: gfxInterface->commandPutPixel2(); break;
		case 490: gfxInterface->commandCopyBox(); break;
		case 491: gfxInterface->commandColor(); break;
		case 492: gfxInterface->commandClsColor(); break;
		case 493: gfxInterface->commandCls(); break;
		case 494: gfxInterface->commandDot(); break;
		case 495: gfxInterface->commandLine(); break;
		case 496: gfxInterface->commandBox(); break;
		case 497: gfxInterface->commandEllipse(); break;
		case 498: gfxInterface->commandCircle(); break;
		case 499: gfxInterface->commandPickColor(); break;
		case 503: gfxInterface->commandScreenGamma(); break;
		case 504: gfxInterface->commandDrawToImage(); break;
		case 505: gfxInterface->commandDrawToScreen(); break;
		case 506: gfxInterface->commandSmooth2D(); break;
		case 511: gfxInterface->commandScreenShot(); break;
		case 512: gfxInterface->commandDrawGame(); break;
		case 513: gfxInterface->commandDrawScreen(); break;
		case 514: gfxInterface->commandUpdateGame(); break;
		case 519: gfxInterface->commandDrawToWorld(); break;
		case 524: imageInterface->commandSaveImage(); break;
		case 525: imageInterface->commandDrawImage(); break;
		case 526: imageInterface->commandDrawImageBox(); break;
		case 527: imageInterface->commandMaskImage(); break;
		case 528: imageInterface->commandResizeImage(); break;
		case 529: imageInterface->commandRotateImage(); break;
		case 534: imageInterface->commandDeleteImage(); break;
		case 535: imageInterface->commandDefaultMask(); break;
		case 536: imageInterface->commandHotSpot(); break;
		case 537: imageInterface->commandPickImageColor(); break;
		case 538: imageInterface->commandPickImageColor2(); break;
		case 539: imageInterface->commandDrawGhostImage(); break;
		case 616: objectInterface->commandDeleteObject(); break;
		case 617: objectInterface->commandMoveObject(); break;
		case 618: objectInterface->commandTranslateObject(); break;
		case 619: objectInterface->commandPositionObject(); break;
		case 620: objectInterface->commandCloneObjectPosition(); break;
		case 621: cameraInterface->commandCloneCameraPosition(); break;
		case 622: objectInterface->commandTurnObject(); break;
		case 623: objectInterface->commandRotateObject(); break;
		case 624: objectInterface->commandPointObject(); break;
		case 625: objectInterface->commandCloneObjectOrientation(); break;
		case 626: cameraInterface->commandCloneCameraOrientation(); break;
		case 628: objectInterface->commandObjectOrder(); break;
		case 629: objectInterface->commandMaskObject(); break;
		case 630: objectInterface->commandShowObject(); break;
		case 631: objectInterface->commandGhostObject(); break;
		case 633: objectInterface->commandPaintObject(); break;
		case 635: objectInterface->commandObjectRange(); break;
		case 638: objectInterface->commandObjectInteger(); break;
		case 639: objectInterface->commandObjectFloat(); break;
		case 640: objectInterface->commandObjectString(); break;
		case 641: objectInterface->commandObjectPickable(); break;
		case 642: objectInterface->commandObjectPick(); break;
		case 647: cameraInterface->commandCameraPick(); break;
		case 648: objectInterface->commandScreenPositionObject(); break;
		case 649: objectInterface->commandObjectLife(); break;
		case 650: objectInterface->commandPlayObject(); break;
		case 651: objectInterface->commandLoopObject(); break;
		case 652: objectInterface->commandStopObject(); break;
		case 703: objectInterface->commandClearCollisions(); break;
		case 704: objectInterface->commandResetObjectCollision(); break;
		case 705: objectInterface->commandSetupCollision(); break;
		case 723: cameraInterface->commandPointCamera(); break;
		case 724: cameraInterface->commandTurnCamera(); break;
		case 725: cameraInterface->commandRotateCamera(); break;
		case 726: cameraInterface->commandMoveCamera(); break;
		case 727: cameraInterface->commandTranslateCamera(); break;
		case 728: cameraInterface->commandPositionCamera(); break;
		case 729: cameraInterface->commandCameraFollow(); break;
		case 752: mapInterface->commandEditMap(); break;
		case 757: mapInterface->commandSetMap(); break;
		case 758: mapInterface->commandSetTile(); break;
		case 791: effectInterface->commandParticleMovement(); break;
		case 792: effectInterface->commandParticleAnimation(); break;
		case 793: effectInterface->commandParticleEmission(); break;
		case 796: objectInterface->commandDefaultVisible(); break;
		case 797: objectInterface->commandMirrorObject(); break;
		case 798: objectInterface->commandPixelPick(); break;
		case 799: objectInterface->commandClearObjects(); break;
		default: FIXME("Unimplemented command: %i", command);
	}
}

/*
 * CBEnchanted::handleFunction - Run function
 */
void CBEnchanted::handleFunction(void) {
	uint32_t func = *(uint32_t *)(code);
	code += 4;
	HCDEBUG("Function: %i", func);

	switch(func) {
		case 50: functionNew(); break;
		case 52: functionFirst(); break;
		case 53: functionLast(); break;
		case 54: functionBefore(); break;
		case 55: functionAfter(); break;
		case 61: functionRead(); break;
		case 63: functionConvertToInteger(); break;
		case 64: functionConvertToType(); break;
		case 66: objectInterface->functionNextObject(); break;
		case 100: mathInterface->functionInt(); break;
		case 101: mathInterface->functionFloat(); break;
		case 102: mathInterface->functionRoundUp(); break;
		case 103: mathInterface->functionRoundDown(); break;
		case 104: mathInterface->functionAbs(); break;
		case 105: mathInterface->functionSqrt(); break;
		case 106: mathInterface->functionSin(); break;
		case 107: mathInterface->functionCos(); break;
		case 108: mathInterface->functionTan(); break;
		case 109: mathInterface->functionASin(); break;
		case 110: mathInterface->functionACos(); break;
		case 111: mathInterface->functionATan(); break;
		case 112: mathInterface->functionGetAngle(); break;
		case 113: objectInterface->functionGetAngle2(); break;
		case 114: mathInterface->functionLog(); break;
		case 115: mathInterface->functionLog10(); break;
		case 116: mathInterface->functionRnd(); break;
		case 117: mathInterface->functionRand(); break;
		case 118: mathInterface->functionMin(); break;
		case 119: mathInterface->functionMax(); break;
		case 120: mathInterface->functionCurveValue(); break;
		case 121: mathInterface->functionCurveAngle(); break;
		case 122: mathInterface->functionWrapAngle(); break;
		case 123: mathInterface->functionDistance(); break;
		case 124: objectInterface->functionDistance2(); break;
		case 126: mathInterface->functionBoxOverlap(); break;
		case 150: stringInterface->functionStr(); break;
		case 151: stringInterface->functionLeft(); break;
		case 152: stringInterface->functionRight(); break;
		case 153: stringInterface->functionMid(); break;
		case 154: stringInterface->functionReplace(); break;
		case 155: stringInterface->functionInStr(); break;
		case 156: stringInterface->functionUpper(); break;
		case 157: stringInterface->functionLower(); break;
		case 158: stringInterface->functionTrim(); break;
		case 159: stringInterface->functionLSet(); break;
		case 160: stringInterface->functionRSet(); break;
		case 161: stringInterface->functionChr(); break;
		case 162: stringInterface->functionAsc(); break;
		case 163: stringInterface->functionLen(); break;
		case 164: stringInterface->functionHex(); break;
		case 165: stringInterface->functionBin(); break;
		case 166: stringInterface->functionString(); break;
		case 167: stringInterface->functionFlip(); break;
		case 168: stringInterface->functionStrInsert(); break;
		case 169: stringInterface->functionStrRemove(); break;
		case 170: stringInterface->functionStrMove(); break;
		case 171: stringInterface->functionCountWords(); break;
		case 172: stringInterface->functionGetWord(); break;
		case 200: textInterface->functionLoadFont(); break;
		case 203: textInterface->functionTextWidth(); break;
		case 204: textInterface->functionTextHeight(); break;
		case 220: inputInterface->functionInput(); break;
		case 221: inputInterface->functionKeyDown(); break;
		case 222: inputInterface->functionKeyHit(); break;
		case 223: inputInterface->functionGetKey(); break;
		case 224: inputInterface->functionWaitKey(); break;
		case 227: inputInterface->functionMouseDown(); break;
		case 228: inputInterface->functionMouseHit(); break;
		case 229: inputInterface->functionGetMouse(); break;
		case 230: inputInterface->functionWaitMouse(); break;
		case 231: inputInterface->functionMouseX(); break;
		case 232: inputInterface->functionMouseY(); break;
		case 233: inputInterface->functionMouseZ(); break;
		case 234: inputInterface->functionMouseMoveX(); break;
		case 235: inputInterface->functionMouseMoveY(); break;
		case 236: inputInterface->functionMouseMoveZ(); break;
		case 241: inputInterface->functionLeftKey(); break;
		case 242: inputInterface->functionRightKey(); break;
		case 243: inputInterface->functionUpKey(); break;
		case 244: inputInterface->functionDownKey(); break;
		case 245: inputInterface->functionEscapeKey(); break;
		case 246: inputInterface->functionMouseWX(); break;
		case 247: inputInterface->functionMouseWY(); break;
		case 248: inputInterface->functionKeyUp(); break;
		case 249: inputInterface->functionMouseUp(); break;
		case 300: memInterface->functionMakeMEMBlock(); break;
		case 302: memInterface->functionMEMBlockSize(); break;
		case 305: memInterface->functionPeekByte(); break;
		case 306: memInterface->functionPeekShort(); break;
		case 307: memInterface->functionPeekInt(); break;
		case 308: memInterface->functionPeekFloat(); break;
		case 320: fileInterface->functionOpenToRead(); break;
		case 321: fileInterface->functionOpenToWrite(); break;
		case 322: fileInterface->functionOpenToEdit(); break;
		case 324: fileInterface->functionFileOffset(); break;
		case 328: fileInterface->functionFindFile(); break;
		case 329: fileInterface->functionCurrentDir(); break;
		case 333: fileInterface->functionFileExists(); break;
		case 334: fileInterface->functionIsDirectory(); break;
		case 335: fileInterface->functionFileSize(); break;
		case 339: fileInterface->functionEOF(); break;
		case 341: fileInterface->functionReadByte(); break;
		case 342: fileInterface->functionReadShort(); break;
		case 343: fileInterface->functionReadInt(); break;
		case 344: fileInterface->functionReadFloat(); break;
		case 345: fileInterface->functionReadString(); break;
		case 346: fileInterface->functionReadLine(); break;
		case 400: animInterface->functionPlayAnimation(); break;
		case 403: animInterface->functionAnimationWidth(); break;
		case 404: animInterface->functionAnimationHeight(); break;
		case 405: animInterface->functionAnimationPlaying(); break;
		case 415: sysInterface->functionCrc32(); break;
		case 420: sysInterface->functionDate(); break;
		case 421: sysInterface->functionTime(); break;
		case 422: sysInterface->functionTimer(); break;
		case 424: sysInterface->functionCommandLine(); break;
		case 427: sysInterface->functionGetEXEName(); break;
		case 432: sysInterface->functionFPS(); break;
		case 450: soundInterface->functionPlaySound(); break;
		case 452: soundInterface->functionLoadSound(); break;
		case 455: soundInterface->functionSoundPlaying(); break;
		case 481: gfxInterface->functionSCREEN(); break;
		case 484: gfxInterface->functionGetPixel(); break;
		case 486: gfxInterface->functionGetPixel2(); break;
		case 500: gfxInterface->functionGetRGB(); break;
		case 501: gfxInterface->functionSCREEN(); break;
		case 502: gfxInterface->functionImage(); break;
		case 507: gfxInterface->functionScreenWidth(); break;
		case 508: gfxInterface->functionScreenHeight(); break;
		case 509: gfxInterface->functionScreenDepth(); break;
		case 510: gfxInterface->functionGFXModeExists(); break;
		case 520: imageInterface->functionLoadImage(); break;
		case 521: imageInterface->functionLoadAnimImage(); break;
		case 522: imageInterface->functionMakeImage(); break;
		case 523: imageInterface->functionCloneImage(); break;
		case 530: imageInterface->functionImageWidth(); break;
		case 531: imageInterface->functionImageHeight(); break;
		case 532: imageInterface->functionImagesOverlap(); break;
		case 533: imageInterface->functionImagesCollide(); break;
		case 600: objectInterface->functionLoadObject(); break;
		case 601: objectInterface->functionLoadAnimObject(); break;
		case 602: objectInterface->functionMakeObject(); break;
		case 608: objectInterface->functionMakeObjectFloor(); break;
		case 615: objectInterface->functionCloneObject(); break;
		case 638: objectInterface->functionObjectInteger(); break;
		case 639: objectInterface->functionObjectFloat(); break;
		case 640: objectInterface->functionObjectString(); break;
		case 643: objectInterface->functionPickedObject(); break;
		case 644: objectInterface->functionPickedX(); break;
		case 645: objectInterface->functionPickedY(); break;
		case 649: objectInterface->functionObjectLife(); break;
		case 670: objectInterface->functionObjectAngle(); break;
		case 674: objectInterface->functionObjectX(); break;
		case 675: objectInterface->functionObjectY(); break;
		case 677: objectInterface->functionObjectSizeX(); break;
		case 678: objectInterface->functionObjectSizeY(); break;
		case 680: objectInterface->functionObjectPlaying(); break;
		case 690: objectInterface->functionObjectFrame(); break;
		case 693: cameraInterface->functionCameraX(); break;
		case 694: cameraInterface->functionCameraY(); break;
		case 696: cameraInterface->functionCameraAngle(); break;
		case 700: objectInterface->functionObjectsOverlap(); break;
		case 701: objectInterface->functionObjectSight(); break;
		case 702: objectInterface->functionCountCollisions(); break;
		case 706: objectInterface->functionGetCollision(); break;
		case 709: objectInterface->functionCollisionX(); break;
		case 710: objectInterface->functionCollisionY(); break;
		case 712: objectInterface->functionCollisionAngle(); break;
		case 750: mapInterface->functionLoadMap(); break;
		case 751: mapInterface->functionMakeMap(); break;
		case 753: mapInterface->functionGetMap(); break;
		case 754: mapInterface->functionGetMap2(); break;
		case 755: mapInterface->functionMapWidth(); break;
		case 756: mapInterface->functionMapHeight(); break;
		case 790: effectInterface->functionMakeEmitter(); break;
		default: FIXME("Unimplemented function: %i", func);
	}
}

/*
 * CBEnchanted::handlePushInt - Push integer to stack
 */
FORCEINLINE void CBEnchanted::handlePushInt(void) {
	HCDEBUG("[%i] Push Int %i", code - codeBase, *(int32_t *)(code));
	pushValue(*(int32_t *)(code));
	code += 4;
}

/*
 * CBEnchanted::commandDim - Create array
 */
void CBEnchanted::commandDim(void) {
	code ++;
	uint32_t n = *(uint32_t *)(code); // Number of dimensions
	code += 4;
	code += 1;
	uint32_t type = *(uint32_t *)(code);
	code += 4;
	code += 1;
	uint32_t arrId = *(uint32_t *)(code); // Array ID
	code += 4;
	uint32_t size = 1;
	uint32_t dimensions[5] = {0};
	for (int32_t i = n - 1; i >= 0; --i) {
		dimensions[i] = popValue().toInt() + 1; // Size of dimension
	}

	switch (type){
		case 3: {
			Array<int32_t> a;
			a.init(dimensions, n);
			setArray(arrId, a);
			break;
		}
		case 7: {
			Array<uint16_t> a;
			a.init(dimensions, n);
			setArray(arrId, a);
			break;
		}
		case 8: {
			Array<uint8_t> a;
			a.init(dimensions, n);
			setArray(arrId, a);
			break;
		}
		case 4: {
			Array<float> a;
			a.init(dimensions, n);
			setArray(arrId, a);
			break;
		}
		case 6: {
			Array<ISString> a;
			a.init(dimensions, n);
			setArray(arrId, a);
			break;
		}
	}
}

/*
 * CBEnchanted::commandArrayAssing - Assing value of array element
 */
FORCEINLINE void CBEnchanted::commandArrayAssign(void) {
	uint32_t type = popValue().getInt();
	code ++;
	uint32_t n = *(uint32_t *)(code);
	code += 4;
	code += 1;
	uint32_t id = *(uint32_t *)(code);
	code += 4;

	uint32_t pos = popArrayDimensions1(id, n, type);
	switch (type) {
		case 1: getIntegerArray(id).set(pos, popValue().toInt()); break;
		case 2: getFloatArray(id).set(pos, popValue().toFloat()); break;
		case 4: getShortArray(id).set(pos, popValue().toShort()); break;
		case 5: getByteArray(id).set(pos, popValue().toByte()); break;
		case 3: getStringArray(id).set(pos, popValue().toString()); break;
		default: FIXME("commandArrayAssing: Undefined array type %i", type);
	}
}

/*
 * CBEnchanted::handlePushVariable - Push value of variable to stack
 */
FORCEINLINE void CBEnchanted::handlePushVariable(void) {
	uint32_t type = popValue().getInt();
	int32_t var = *(int32_t *)(code);
	code += 4;
	//HCDEBUG("[%i] Push variable: %i", cpos, type);
	switch (type) {
		case 1: pushValue(getIntegerVariable(var)); break;
		case 2: pushValue(getFloatVariable(var)); break;
		case 3: pushValue(getGlobalIntegerVariable(var)); break;
		case 4: pushValue(getGlobalFloatVariable(var)); break;
		case 5: pushValue(getStringVariable(var)); break;
		case 6: pushValue(getGlobalStringVariable(var)); break;
		case 7: pushValue(int32_t(getShortVariable(var))); break;
		case 8: pushValue(int32_t(getByteVariable(var))); break;
		case 9: pushValue(int32_t(getGlobalShortVariable(var))); break;
		case 10: pushValue(int32_t(getGlobalByteVariable(var))); break;
		case 11:
			if (var == 0) {
				pushValue(reinterpret_cast<void*>(0));
				break;
			}
			pushValue(getTypePointerVariable(var)); break;
		default: FIXME("Unimplemented variable push: %i", type);
	}
}

/*
 * CBEnchanted::handlePushSomething - Push float, string or something else to stack
 */
FORCEINLINE void CBEnchanted::handlePushSomething(void) {
	uint32_t type = popValue().getInt();
	switch (type) {
		case 2: { // Float
			float value = *(float *)(code);
			code += 4;

			pushValue(value);
			break;
		}
		case 3: //Integer array
		case 4: //Float array
		case 6:
		case 7:
		case 8: {
			uint32_t id = *(uint32_t *)(code);
			code += 4;

			int32_t dimensions = popValue().getInt();
			uint32_t pos = popArrayDimensions2(id, dimensions, type);

			switch (type) {
				case 3: pushValue(getIntegerArray(id).get(pos)); break;
				case 4: pushValue(getFloatArray(id).get(pos)); break;
				case 7: pushValue((int32_t)getShortArray(id).get(pos)); break;
				case 8: pushValue((int32_t)getByteArray(id).get(pos)); break;
				case 6: pushValue(getStringArray(id).get(pos)); break;
				default: FIXME("handlePushSomething: Undefined array type %i", type);
			}
			break;
		}
		case 5: { // String
			uint32_t strId = *(uint32_t *)(code);
			code += 4;

			// Push empty string
			if (strId == 0) {
				pushValue(string(""));
			}
			else { // Push string set by user
				pushValue(getString(strId));
			}
			break;
		}
		default: {
			FIXME("Unimplemented push: %i", type);
		}
	}
}

/*
 * CBEnchanted::handleMathOperation - Handle mathematical operation
 */
FORCEINLINE void CBEnchanted::handleMathOperation(void) {
	uint8_t op = *(uint8_t *)(code);
	code++;
	HCDEBUG("Mathoperation: %i", uint32_t(op));
	switch (op) {
		case MathematicOperation::UnaryMinus: Any::unaryMinus(&internalStack); break;
		case MathematicOperation::UnaryPlus: Any::unaryPlus(&internalStack); break;
		case MathematicOperation::Power: Any::power(&internalStack); break;
		case MathematicOperation::Addition: Any::addition(&internalStack); break;
		case MathematicOperation::Subtraction: Any::subtraction(&internalStack); break;
		case MathematicOperation::Multiplication: Any::multiplication(&internalStack); break;
		case MathematicOperation::Division: Any::division(&internalStack); break;
		case MathematicOperation::Modulo: Any::modulo(&internalStack); break;
		case MathematicOperation::Shl: Any::shl(&internalStack); break;
		case MathematicOperation::Shr: Any::shr(&internalStack); break;
		case MathematicOperation::Sar: Any::sar(&internalStack); break;
		case MathematicOperation::LessThan: Any::lessThan(&internalStack); break;
		case MathematicOperation::GreaterThan: Any::greaterThan(&internalStack); break;
		case MathematicOperation::Equal: Any::equal(&internalStack); break;
		case MathematicOperation::NotEqual: Any::notEqual(&internalStack); break;
		case MathematicOperation::LessThanOrEqual: Any::lessThanOrEqual(&internalStack); break;
		case MathematicOperation::GreaterThanOrEqual: Any::greaterThanOrEqual(&internalStack); break;
		case MathematicOperation::AND: Any::AND(&internalStack); break;
		case MathematicOperation::OR: Any::OR(&internalStack); break;
		case MathematicOperation::XOR: Any::XOR(&internalStack); break;
		case MathematicOperation::NOT: Any::NOT(&internalStack); break;
		default: FIXME("Unimplemented mathematical operation: %i", op);
	}
}

/*
 * CBEnchanted::handleJump - Jump if last operation was false
 */
FORCEINLINE void CBEnchanted::handleJump(void) {
	uint32_t dest = *(uint32_t *)(code);
	code += 4;

	if (!popValue().toBool()) {
		code = codeBase + dest;
	}
}

/*
 * CBEnchanted::handleIncVar - Increase integer variable
 */
FORCEINLINE void CBEnchanted::handleIncVar(void) {
	getIntegerVariable(*(uint32_t *)(code)) += 1;
	code += 4;
}

/*
 * CBEnchanted::handleIncGlobalVar - Increase global integer variable
 */
FORCEINLINE void CBEnchanted::handleIncGlobalVar(void) {
	getGlobalIntegerVariable(*(uint32_t *)(code)) += 1;
	code += 4;
}

FORCEINLINE void CBEnchanted::handlePushTypeMemberVariable(void) {
	void * typePtr = getTypePointerVariable(*((int32_t*)(code)));
	code += 4;
	int32_t varType = popValue().getInt();
	int32_t place = popValue().getInt() - 12;
	switch (varType) {
		case 1: pushValue(Type::getMembersType(typePtr)->getIntField(typePtr, place)); break;
		case 2: pushValue(Type::getMembersType(typePtr)->getFloatField(typePtr, place)); break;
		case 3: pushValue(Type::getMembersType(typePtr)->getStringField(typePtr, place)); break;
		case 4: pushValue(Type::getMembersType(typePtr)->getShortField(typePtr, place)); break;
		case 5: pushValue(Type::getMembersType(typePtr)->getByteField(typePtr, place)); break;
		default: FIXME("handlePushTypeMemberVariable:Unhandled varType %i", varType); break;
	}
}

FORCEINLINE void CBEnchanted::handleCustomFunctionCall() {
	int32_t handle = *(int32_t *)(code);
	code += 4;
	customFunctionHandler.call(this, handle);
}

/*
 * CBEnchanted::commandGoto - Jump to different location
 */
FORCEINLINE void CBEnchanted::commandGoto(void) {
	code++;
	code = codeBase + *(uint32_t *)(code);
}

void CBEnchanted::commandDelete(void) {
	int32_t memberId = *(int32_t*)(code - 9);
	void *typeMember = popValue().getTypePtr();

	setTypePointerVariable(memberId, Type::getMembersType(typeMember)->deleteMember(typeMember));

	map<void*, int32_t>::iterator findIt = rTypeConvertMap.find(typeMember);
	if (findIt != rTypeConvertMap.end()) {
		// If one was found, it means that ConvertToInteger() has been used.
		typeConvertMap.erase(findIt->second);
		rTypeConvertMap.erase(findIt);
	}
}

void CBEnchanted::commandInsert(void) {
	void *target = popValue().getTypePtr();
	void *toBeMoved = popValue().getTypePtr();

	Type *type = Type::getMembersType(toBeMoved);

	type->insert(toBeMoved, target);
}

void CBEnchanted::commandClearArray(void) {
	clearArray = popValue().toBool();
}

void CBEnchanted::commandReDim(void) {
	code ++;
	uint32_t n = *(uint32_t *)(code); // Number of dimensions
	code += 4;
	code += 1;
	uint32_t type = *(uint32_t *)(code);
	code += 4;
	code += 1;
	uint32_t arrId = *(uint32_t *)(code); // Array ID
	code += 4;
	uint32_t size = 1;
	uint32_t dimensions[5] = {0};
	for (int32_t i = n - 1; i >= 0; --i) {
		dimensions[i] = popValue().toInt() + 1; // Size of dimension
	}

	switch (type){
		case 3: {
			Array<int32_t> &a = getIntegerArray(arrId);
			a.resize(dimensions, n, !clearArray);
			break;
		}
		case 7: {
			Array<uint16_t> & a = getShortArray(arrId);
			a.resize(dimensions, n, !clearArray);
			break;
		}
		case 8: {
			Array<uint8_t> &a = getByteArray(arrId);
			a.resize(dimensions, n, !clearArray);
			break;
		}
		case 4: {
			Array<float> &a = getFloatArray(arrId);
			a.resize(dimensions, n, !clearArray);
			break;
		}
		case 6: {
			Array<ISString> &a = getStringArray(arrId);
			a.resize(dimensions, n, !clearArray);
			break;
		}
	}
}

FORCEINLINE void CBEnchanted::commandReturn(void) {
	if (pos.empty()) return;
	code = pos.back();
	pos.pop_back();

	if (inFunction()) {
		popScope();
	}
}

void CBEnchanted::commandGosub(void) {
	int32_t ptr = *(int32_t *)(++code);
	code += 4;

	pos.push_back(code);

	code = codeBase + ptr;
}

void CBEnchanted::functionNew(void) {
	int32_t typeId = popValue().getInt();
	void *member = getType(typeId)->newMember();
	pushValue(Any(member));
}

void CBEnchanted::functionFirst(void) {
	int32_t typeId = popValue().getInt();
	pushValue(getType(typeId)->getFirst());
}

void CBEnchanted::functionLast(void) {
	int32_t typeId = popValue().getInt();
	pushValue(getType(typeId)->getLast());
}

void CBEnchanted::functionBefore(void) {
	void *typeMember = popValue().getTypePtr();
	pushValue(Type::getBefore(typeMember));
}

void CBEnchanted::functionAfter(void) {
	void *typeMember = popValue().getTypePtr();
	pushValue(Type::getAfter(typeMember));
}

void CBEnchanted::functionConvertToInteger(void) {
	void *typeMember = popValue().getTypePtr();
	if (typeMember == 0) {
		this->errors->createError("ConvertToInteger() failed!", "Can't convert to integer from NULL");
		pushValue(0);
	}

	map<void*, int32_t>::iterator findIt = rTypeConvertMap.find(typeMember);
	if (findIt != rTypeConvertMap.end()) {
		// This has been already converted.
		pushValue(findIt->second);
	}
	else {
		// Not converted before.
		int32_t id = nextTypeId();
		typeConvertMap[id] = typeMember;
		rTypeConvertMap[typeMember] = id;
		pushValue(id);
	}
}

void CBEnchanted::functionConvertToType(void) {
	int32_t typePtr = popValue().getInt();
	if (typePtr == 0) {
		this->errors->createError("ConvertToType() failed!", "Can't convert to type from 0");
		pushValue(0);
		return;
	}

	map<int32_t, void*>::iterator findIt = typeConvertMap.find(typePtr);
	if (findIt != typeConvertMap.end()) {
		// This has been already converted.
		pushValue(findIt->second);
	}
	else {
		// Not converted before. Invalid ID
		this->errors->createError("ConvertToType() failed!", "Could not find a converted type with ID " + boost::lexical_cast<string>(typePtr) + ".");
		pushValue(0);
	}
}

template<class T> FORCEINLINE uint32_t CBEnchanted::popArrayDimensions(Array<T> &a, int32_t n) {
	uint32_t pos(0);
#ifdef CBE_ARRAY_BOUNDS_CHECK
	uint32_t index[5] = {0, 0, 0, 0, 0};
	bool outOfBounds = false;
	for (int32_t i = n - 1; i >= 0; --i) {
		index[i] = popValue().getInt();
		if (index[i] >= a.getDimensionSizes()[i] || index[i] < 0) {
			outOfBounds = true;
		}
		pos +=  index[i] * a.getDimensionMultiplier(i);
	}
	if (outOfBounds) {
		string arrayStr;
		string accessStr;
		for (int i = 0; i < n; i++) {
			if (i != 0) {
				arrayStr += ", ";
				accessStr += ", ";
			}
			arrayStr += boost::lexical_cast<string>(a.getDimensionSizes()[i] - 1);
			accessStr += boost::lexical_cast<string>(index[i]);
		}
		errors->createError("Array access out of bounds", "Accessing ("+ accessStr +") array (" + arrayStr + ") ");
		pos = 0;
	}
#else
	for (int32_t i = n - 1; i >= 0; --i) {
		pos += popValue().getInt() * a.getDimensionMultiplier(i);
	}
#endif
	return pos;
}

FORCEINLINE uint32_t CBEnchanted::popArrayDimensions1(int32_t arrayId, int32_t n, int32_t type) {
	uint32_t pos = 0;
	switch (type) {
		case 1: {
			Array<int32_t> &a = getIntegerArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 4: {
			Array<uint16_t> &a = getShortArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 5: {
			Array<uint8_t> &a = getByteArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 2: {
			Array<float> &a = getFloatArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 3: {
			Array<ISString> &a = getStringArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
	}
	return pos;
}

FORCEINLINE uint32_t CBEnchanted::popArrayDimensions2(int32_t arrayId, int32_t n, int32_t type) {
	uint32_t pos = 0;
	switch (type) {
		case 3: {
			Array<int32_t> &a = getIntegerArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 7: {
			Array<uint16_t> &a = getShortArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 8: {
			Array<uint8_t> &a = getByteArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 4: {
			Array<float> &a = getFloatArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
		case 6: {
			Array<ISString> &a = getStringArray(arrayId);
			return popArrayDimensions(a, n);
			break;
		}
	}
	return pos;
}

void CBEnchanted::commandSetVariable(void) {
	int32_t type = popValue().getInt();

	switch (type) {
		case 1: { // String
			int32_t id = popValue().getInt();
			ISString value = popValue().toString();

			setStringVariable(id, value);
			break;
		}
		case 2: { // Short
			int32_t id = popValue().getInt();
			uint16_t value = popValue().toShort();

			setShortVariable(id, value);
			break;
		}
		case 3: { // Byte
			int32_t id = popValue().getInt();
			uint8_t value = popValue().toByte();

			setByteVariable(id, value);
			break;
		}
		case 4: { // Typepointer tjsp
			int32_t id = popValue().getInt();
			void *ptr = popValue().toTypePtr();

			setTypePointerVariable(id, ptr);
			break;
		}
		default:
			FIXME("Unimplemented SetVariable. Type: %i", type);
	}
	HCDEBUG("Push value, type: %i", type);
}

void CBEnchanted::commandSetGlobalVariable(void) {
	int32_t type = popValue().getInt();
	int32_t var = popValue().getInt();
	switch  (type) {
		case 1: setGlobalIntegerVariable(var, popValue().toInt()); break;
		case 2: setGlobalFloatVariable(var, popValue().toFloat()); break;
		case 3:	setGlobalStringVariable(var, popValue().toString()); break;
		case 4:	setGlobalShortVariable(var, popValue().toShort()); break;
		case 5: setGlobalByteVariable(var, popValue().toByte()); break;
	}
}

void CBEnchanted::commandSetArrayNumbers(void) {
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	initArrays(byteCount, shortCount, stringCount, floatCount, integerCount);
}

void CBEnchanted::commandSetGlobalVariableNumbers(void) {
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	initGlobalVars(byteCount, shortCount, stringCount, floatCount, integerCount);
}

void CBEnchanted::commandType(void) {
	int32_t typeMemberSize = popValue().getInt();
	addType(typeMemberSize - 4);
}

void CBEnchanted::commandSetTypeMemberField(void) {
	int32_t varType = popValue().getInt();
	void * typePtr = getTypePointerVariable(popValue().getInt());
	int32_t place = popValue().getInt() - 12;
	switch (varType) {
		case 1: Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toInt()); break;
		case 2: Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toFloat()); break;
		case 3: Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toString()); break;
		case 4: Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toShort()); break;
		case 5: Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toByte()); break;
		default: FIXME("setTypeMemberField:Unhandled varType %i", varType);
	}
}

void CBEnchanted::commandSetVariableNumbers(void) {
	//TODO: Check if right order
	int32_t typePtrCount = popValue().getInt();
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	pushScope(byteCount, shortCount, stringCount, floatCount, integerCount, typePtrCount);
}

void CBEnchanted::commandRestore(void) {
	code++;
	handlePushInt();
	dataPos = popValue().getInt();
}

void CBEnchanted::handleData(void) {
	// Move the code-variable forward for the amount needed
	uint8_t type = *(uint8_t *)(code);
	switch (type) {
		case 1: // Int, Short, Byte
			code += 6;
		break;
		case 2: // Float
		case 4: // String
			code += 11;
		break;
	}
}

void CBEnchanted::functionRead(void) {
	char * tempCode = code;
	code = codeBase + dataPos;
	if (*(uint8_t *)(code++) != 68) {
		errors->createError("Read() failed!", "Tried to read past DATA");
		code = tempCode;
		pushValue(0);
		return;
	}
	uint8_t type = *(uint8_t *)(code);
	code += 2;

	handlePushInt();
	if (type == 2 || type == 4) { // Float or String
		code++;
		dataPos += 5;
		handlePushSomething();
	}

	dataPos += 7;

	code = tempCode;
}
#endif


void CBEnchanted::setSmooth2D(bool toggled) {
	if (toggled) {
		// Set new display flags for antialiasing
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
		al_set_new_display_option(ALLEGRO_SAMPLES, 6, ALLEGRO_REQUIRE);
		// Set linear filtering for image operations
		al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	}
	else {
		// Remove antialiasing flags
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_DONTCARE);
		al_set_new_display_option(ALLEGRO_SAMPLES, 6, ALLEGRO_DONTCARE);
		// Unset linear filtering for image operations
		al_set_new_bitmap_flags(al_get_new_bitmap_flags() & ~(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR));
	}
	smooth2d = toggled;
}

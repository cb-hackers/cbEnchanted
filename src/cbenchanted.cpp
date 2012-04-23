#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include "stringinterface.h"
#include "textinterface.h"
#include "inputinterface.h"
#include "mathinterface.h"
#include "cbvariableholder.h"
#include "mathoperations.h"
#include "errorsystem.h"

static CBEnchanted *cbInstance;

CBEnchanted::CBEnchanted():
	caller(&internalStack)
{
	cbInstance = this;
	initialized = false;
	running = false;
	safeExit = 1;
}

CBEnchanted::~CBEnchanted() {
	//delete[] code;
}

CBEnchanted *CBEnchanted::instance() {
	return cbInstance;
}

/*
 * CBEnchanted::run - Interpret CoolBasic bytecode
 */
void CBEnchanted::run() {
	// Make sure that we are initialized properly to avoid crashing
	assert(initialized == true);

	// Run until told to quit
	running = true;
	while (running) {
		uint32_t opCode = (uint32_t)*(uint8_t *)(code++);//[cpos++];

		HCDEBUG("[%i]: OpCode: %i", code - codeBase, opCode);
		switch (opCode) {
			case 65: handleSetInt(); break;
			case 66: handleSetFloat(); break;
			case 67: handleCommand(); break;
			case 73: handlePushInt(); break;
			case 74: handlePushSomething(); break;
			case 78: handleJump(); break;
			case 79: handleMathOperation(); break;
			case 80: handleIncVar(); break;
			case 81: handleIncGlobalVar(); break;
			case 84: handlePushTypeMemberVariable();break;
			case 85: handlePushFuncptr(); break;
			case 86: handlePushVariable(); break;
			case 90: handleFunction(); break;
			case 97:
			case 98:
			case 99: break;
			case 100: handleCustomFunctionCall(); break;
			default: FIXME("Unimplemented handler: %i", opCode);
		}
	}
}

/*
 * CBEnchanted::init - Initialize the interpreter
 */
bool CBEnchanted::init(string file) {
	INFO("Initializing");
	int32_t startPos; // Beginning of the CoolBasic data
	int32_t endPos; // End of the executable

	uint32_t nStrings; // Number of strings
	uint32_t size; // Length of CoolBasic data

	// Open file for reading
	ifstream input(file.c_str(), ios::binary);

	if (!input.is_open()) {
		FIXME("Can't open exe! %s", file.c_str());
		return false;
	}

	input.seekg(-4, ios::end);
	endPos = input.tellg();
	input.read((char *)(&startPos), 4);
	input.seekg(24 - startPos, ios::end);
	input.read((char *)(&nStrings), 4);

	// Read and decrypt strings
	initStrings(nStrings);
	string key = "Mark Sibly is my idol!";
	for (uint32_t i = 1; i <= nStrings; i++) {
		uint32_t len;
		input.read((char *)(&len), 4);
		string s = "";
		char c;
		for (uint32_t j = 0; j < len; j++) {
			input >> c;
			s += char(c - key[j % key.length()]);
		}
		setString(i, s);
	}

	// Skip useless data and directly to beginning of the bytecode
	input.seekg(32, ios::cur);
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
	while (i < size) {
		offsets[ncmd] = i;
		//roffsets[i] = ncmd;
		uint8_t cmd = *(uint8_t *)(code + i);
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
			case 79: i ++; break;
			default: FIXME("[%i] Unhandled preparsing1: %i",i, (uint32_t) cmd);
		}
	}
	map<int32_t,int32_t> functionMaping;
	//Goto and if
	i = 0;
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
			case 79: i ++; break;
			case 67: {
				if ((*(int32_t *)(code + i)) == 12 || (*(int32_t *)(code + i)) == 21 || (*(int32_t *)(code + i)) == 6) { //commandGoto, commandGosub and commandSelect
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
					uint32_t i2 = *(int32_t *)(code + i);
					if (functionMaping.find(i2) != functionMaping.end()) { //Already parsed function
						*(uint8_t *)(code + i - 1) = 100; //Custom function call
						*(int32_t *)(code + i) = functionMaping[i2];
						goto already_parsed;
					}
					//command 99
					if (code[i2++] != 73) {
						FIXME("[%i] Unexpected opcode1: %i, expecting 73.",i2,code[i2]);
						goto not_custom_function;
					}
					i2 += 4;

					for (int32_t c = 0; c != 5; c++) {
						if (code[i2++] != 73) {
							FIXME("[%i] Unexpected opcode2: %i, expecting 73.",i2,code[i2]);
							goto not_custom_function;
						}
						i2 += 4;
					}
					if (code[i2++] != 67) {
						FIXME("[%i] Unexpected opcode3: %i, expecting 67.",i2,code[i2]);
						goto not_custom_function;
					}
					if (*(int32_t*)(code + i2) != 99) {
						FIXME("[%i] Unexpected command %i, expecting 79.",i2,code[i2]);
						goto not_custom_function;
					}
					i2 += 4;
					//commandFunction
					int32_t paramCount = 0;
					vector<int32_t> params;
					int32_t opc;
					int32_t comc;
					while ((opc = code[i2]) == 67 && (comc = *(int32_t*)(code + i2 + 1)) == 79) {
						i2 += 6;
						paramCount++;
						params.insert(params.begin(), *(int32_t*)(code + i2));
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
					if (*(int32_t*)(code + i2) != 0) { //Return == 0
						goto not_custom_function;
					}
					i2 += 4;

					if (code[i2++] != 67) { //call command
						goto not_custom_function;
					}
					if (*(int32_t*)(code + i2) != 22) { //Return
						goto not_custom_function;
					}

					CustomFunction func(0,groupId,funcId);
					func.setParams(params);
					int32_t handle = customFunctionHandler.getHandle(func);
					functionMaping[*(int32_t *)(code + i)] = handle;
					*(uint8_t *)(code + i - 1) = 100; //Custom function call
					*(int32_t *)(code + i) = handle;
					INFO("Added custom function with handle %i",handle);
				}
				already_parsed:
				not_custom_function:
#endif //DISABLE_CUSTOMS

				i +=4;
				break;
			}

			default: FIXME("[%i] Unhandled preparsing2: %i",i, (uint32_t) cmd);
		}
	}

	// Initialize error system first, because we can.
	errors = new ErrorSystem();

	if (!al_init()) {
		errors->createFatalError("Initialization error", "Failed to initialize Allegro");
		return false;
	}
	eventQueue = al_create_event_queue();
	if (!eventQueue) {
		errors->createFatalError("Initialization error", "Failed to initialize event queue");
		return false;
	}
	//Create screen
	if (!caller.init()) return false;
	if (!initializeGfx()) {
		errors->createFatalError("Initialization error", "Failed to initialize graphics");
		return false;
	}
	if (!initializeInputs()) {
		errors->createFatalError("Initialization error", "Failed to initialize inputs");
		return false;
	}
	if (!initializeSounds()){
		errors->createFatalError("Initialization error", "Failed to initialize sounds");
		return false;
	}
	if (!initializeFonts()) {
		errors->createFatalError("Initialization error", "Failed to initialize fonts");
		return false;
	}

	initialized = true;
	INFO("Initialized");
	return true;
}

void CBEnchanted::stop() {
	running = false;
}

void CBEnchanted::cleanup() {
	cleanupSoundInterface();
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

void CBEnchanted::commandSelect() {
	selectValue = popValue().toInt();
	++code;
	code = codeBase + *(int32_t *)(code);
}

void CBEnchanted::commandCase() {
	++code;
	int32_t testCount = *(int32_t *)(code);
	code += 5;
	int32_t nextCase = *(int32_t *)(code);
	for (int i = 0; i < testCount;++i) {
		code += 5;
		if (*(int32_t *)(code) == selectValue) {
			code += 4;
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
	HCDEBUG("[%i]: Setting int variable %i to value: %i", code - codeBase, var,value);
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
		/*case 1: caller.commandIf(); break;
		case 3: caller.commandElse(); break;
		case 4: caller.commandElseIf(); break;
		case 5: caller.commandEndIf(); break;*/
		case 6: commandSelect(); break;
		case 7: commandCase(); break;
		/*case 8: caller.commandDefault(); break;
		case 9: caller.commandEndSelect(); break;
		case 10: caller.commandRepeat(); break;
		case 11: caller.commandUntil(); break;*/
		case 12: commandGoto(); break;
		case 21: commandGosub(); break;
		case 22: commandReturn(); break;
		case 42: commandDim(); break;
		case 43: commandReDim(); break;
		case 44: commandClearArray(); break;
		case 56: commandInsert(); break;
		case 57: commandDelete(); break;
		case 65: caller.commandInitObjectList(); break;
		case 69: caller.commandEnd(); break;
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
		case 125: caller.commandRandomize(); break;
		case 201: caller.commandSetFont(); break;
		case 202: caller.commandDeleteFont(); break;
		case 205: caller.commandText(); break;
		case 206: caller.commandCenterText(); break;
		case 207: caller.commandPrint(); break;
		case 208: caller.commandWrite(); break;
		case 209: caller.commandLocate(); break;
		case 210: caller.commandClearText(); break;
		case 211: caller.commandAddText(); break;
		case 212: caller.commandVerticalText(); break;
		case 224: caller.commandWaitKey(); break;
		case 225: caller.commandClearKeys(); break;
		case 226: caller.commandPositionMouse(); break;
		case 230: caller.commandWaitMouse(); break;
		case 237: caller.commandShowMouse(); break;
		case 238: caller.commandClearMouse(); break;
		case 239: caller.commandSAFEEXIT(); break;
		case 301: caller.commandDeleteMEMBlock(); break;
		case 303: caller.commandResizeMEMBlock(); break;
		case 304: caller.commandMemCopy(); break;
		case 309: caller.commandPokeByte(); break;
		case 310: caller.commandPokeShort(); break;
		case 311: caller.commandPokeInt(); break;
		case 312: caller.commandPokeFloat(); break;
		case 323: caller.commandCloseFile(); break;
		case 325: caller.commandSeekFile(); break;
		case 326: caller.commandStartSearch(); break;
		case 327: caller.commandEndSearch(); break;
		case 330: caller.commandChDir(); break;
		case 331: caller.commandMakeDir(); break;
		case 336: caller.commandCopyFile(); break;
		case 337: caller.commandDeleteFile(); break;
		case 338: caller.commandExecute(); break;
		case 341: caller.commandReadByte(); break;
		case 342: caller.commandReadShort(); break;
		case 343: caller.commandReadInt(); break;
		case 344: caller.commandReadFloat(); break;
		case 345: caller.commandReadString(); break;
		case 346: caller.commandReadLine(); break;
		case 348: caller.commandWriteByte(); break;
		case 349: caller.commandWriteShort(); break;
		case 350: caller.commandWriteInt(); break;
		case 351: caller.commandWriteFloat(); break;
		case 352: caller.commandWriteString(); break;
		case 353: caller.commandWriteLine(); break;
		case 401: caller.commandStopAnimation(); break;
		case 402: caller.commandDrawAnimation(); break;
		case 416: caller.commandEncrypt(); break;
		case 417: caller.commandDecrypt(); break;
		case 423: caller.commandWait(); break;
		case 425: caller.commandSetWindow(); break;
		case 426: caller.commandMakeError(); break;
		case 428: caller.commandSaveProgram(); break;
		case 429: caller.commandLoadProgram(); break;
		case 430: caller.commandGotoSavedLocation(); break;
		case 431: caller.commandFrameLimit(); break;
		case 436: caller.commandCallDLL(); break;
		case 437: caller.commandErrors(); break;
		case 450: caller.commandPlaySound(); break;
		case 451: caller.commandSetSound(); break;
		case 453: caller.commandDeleteSound(); break;
		case 454: caller.commandStopSound(); break;
		case 481: caller.commandScreen(); break;
		case 482: caller.commandLock(); break;
		case 483: caller.commandUnlock(); break;
		case 485: caller.commandPutPixel(); break;
		case 487: caller.commandPutPixel2(); break;
		case 490: caller.commandCopyBox(); break;
		case 491: caller.commandColor(); break;
		case 492: caller.commandClsColor(); break;
		case 493: caller.commandCls(); break;
		case 494: caller.commandDot(); break;
		case 495: caller.commandLine(); break;
		case 496: caller.commandBox(); break;
		case 497: caller.commandEllipse(); break;
		case 498: caller.commandCircle(); break;
		case 499: caller.commandPickColor(); break;
		case 503: caller.commandScreenGamma(); break;
		case 504: caller.commandDrawToImage(); break;
		case 505: caller.commandDrawToScreen(); break;
		case 506: caller.commandSmooth2D(); break;
		case 511: caller.commandScreenShot(); break;
		case 512: caller.commandDrawGame(); break;
		case 513: caller.commandDrawScreen(); break;
		case 514: caller.commandUpdateGame(); break;
		case 519: caller.commandDrawToWorld(); break;
		case 524: caller.commandSaveImage(); break;
		case 525: caller.commandDrawImage(); break;
		case 526: caller.commandDrawImageBox(); break;
		case 527: caller.commandMaskImage(); break;
		case 528: caller.commandResizeImage(); break;
		case 529: caller.commandRotateImage(); break;
		case 534: caller.commandDeleteImage(); break;
		case 535: caller.commandDefaultMask(); break;
		case 536: caller.commandHotSpot(); break;
		case 537: caller.commandPickImageColor(); break;
		case 538: caller.commandPickImageColor2(); break;
		case 539: caller.commandDrawGhostImage(); break;
		case 616: caller.commandDeleteObject(); break;
		case 617: caller.commandMoveObject(); break;
		case 618: caller.commandTranslateObject(); break;
		case 619: caller.commandPositionObject(); break;
		case 620: caller.commandCloneObjectPosition(); break;
		case 621: caller.commandCloneCameraPosition(); break;
		case 622: caller.commandTurnObject(); break;
		case 623: caller.commandRotateObject(); break;
		case 624: caller.commandPointObject(); break;
		case 625: caller.commandCloneObjectOrientation(); break;
		case 626: caller.commandCloneCameraOrientation(); break;
		case 628: caller.commandObjectOrder(); break;
		case 629: caller.commandMaskObject(); break;
		case 630: caller.commandShowObject(); break;
		case 631: caller.commandGhostObject(); break;
		case 633: caller.commandPaintObject(); break;
		case 635: caller.commandObjectRange(); break;
		case 638: caller.commandObjectInteger(); break;
		case 639: caller.commandObjectFloat(); break;
		case 640: caller.commandObjectString(); break;
		case 641: caller.commandObjectPickable(); break;
		case 642: caller.commandObjectPick(); break;
		case 647: caller.commandCameraPick(); break;
		case 648: caller.commandScreenPositionObject(); break;
		case 649: caller.commandObjectLife(); break;
		case 650: caller.commandPlayObject(); break;
		case 651: caller.commandLoopObject(); break;
		case 652: caller.commandStopObject(); break;
		case 703: caller.commandClearCollisions(); break;
		case 704: caller.commandResetObjectCollision(); break;
		case 705: caller.commandSetupCollision(); break;
		case 723: caller.commandPointCamera(); break;
		case 724: caller.commandTurnCamera(); break;
		case 725: caller.commandRotateCamera(); break;
		case 726: caller.commandMoveCamera(); break;
		case 727: caller.commandTranslateCamera(); break;
		case 728: caller.commandPositionCamera(); break;
		case 729: caller.commandCameraFollow(); break;
		case 752: caller.commandEditMap(); break;
		case 757: caller.commandSetMap(); break;
		case 758: caller.commandSetTile(); break;
		case 791: caller.commandParticleMovement(); break;
		case 792: caller.commandParticleAnimation(); break;
		case 793: caller.commandParticleEmission(); break;
		case 796: caller.commandDefaultVisible(); break;
		case 797: caller.commandMirrorObject(); break;
		case 798: caller.commandPixelPick(); break;
		case 799: caller.commandClearObjects(); break;
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
		case 66: caller.functionNextObject(); break;
		case 100: caller.functionInt(); break;
		case 101: caller.functionFloat(); break;
		case 102: caller.functionRoundUp(); break;
		case 103: caller.functionRoundDown(); break;
		case 104: caller.functionAbs(); break;
		case 105: caller.functionSqrt(); break;
		case 106: caller.functionSin(); break;
		case 107: caller.functionCos(); break;
		case 108: caller.functionTan(); break;
		case 109: caller.functionASin(); break;
		case 110: caller.functionACos(); break;
		case 111: caller.functionATan(); break;
		case 112: caller.functionGetAngle(); break;
		case 113: caller.functionGetAngle2(); break;
		case 114: caller.functionLog(); break;
		case 115: caller.functionLog10(); break;
		case 116: caller.functionRnd(); break;
		case 117: caller.functionRand(); break;
		case 118: caller.functionMin(); break;
		case 119: caller.functionMax(); break;
		case 120: caller.functionCurveValue(); break;
		case 121: caller.functionCurveAngle(); break;
		case 122: caller.functionWrapAngle(); break;
		case 123: caller.functionDistance(); break;
		case 124: caller.functionDistance2(); break;
		case 126: caller.functionBoxOverlap(); break;
		case 150: caller.functionStr(); break;
		case 151: caller.functionLeft(); break;
		case 152: caller.functionRight(); break;
		case 153: caller.functionMid(); break;
		case 154: caller.functionReplace(); break;
		case 155: caller.functionInStr(); break;
		case 156: caller.functionUpper(); break;
		case 157: caller.functionLower(); break;
		case 158: caller.functionTrim(); break;
		case 159: caller.functionLSet(); break;
		case 160: caller.functionRSet(); break;
		case 161: caller.functionChr(); break;
		case 162: caller.functionAsc(); break;
		case 163: caller.functionLen(); break;
		case 164: caller.functionHex(); break;
		case 165: caller.functionBin(); break;
		case 166: caller.functionString(); break;
		case 167: caller.functionFlip(); break;
		case 168: caller.functionStrInsert(); break;
		case 169: caller.functionStrRemove(); break;
		case 170: caller.functionStrMove(); break;
		case 171: caller.functionCountWords(); break;
		case 172: caller.functionGetWord(); break;
		case 200: caller.functionLoadFont(); break;
		case 203: caller.functionTextWidth(); break;
		case 204: caller.functionTextHeight(); break;
		case 220: caller.functionInput(); break;
		case 221: caller.functionKeyDown(); break;
		case 222: caller.functionKeyHit(); break;
		case 223: caller.functionGetKey(); break;
		case 224: caller.functionWaitKey(); break;
		case 227: caller.functionMouseDown(); break;
		case 228: caller.functionMouseHit(); break;
		case 229: caller.functionGetMouse(); break;
		case 230: caller.functionWaitMouse(); break;
		case 231: caller.functionMouseX(); break;
		case 232: caller.functionMouseY(); break;
		case 233: caller.functionMouseZ(); break;
		case 234: caller.functionMouseMoveX(); break;
		case 235: caller.functionMouseMoveY(); break;
		case 236: caller.functionMouseMoveZ(); break;
		case 241: caller.functionLeftKey(); break;
		case 242: caller.functionRightKey(); break;
		case 243: caller.functionUpKey(); break;
		case 244: caller.functionDownKey(); break;
		case 245: caller.functionEscapeKey(); break;
		case 246: caller.functionMouseWX(); break;
		case 247: caller.functionMouseWY(); break;
		case 248: caller.functionKeyUp(); break;
		case 249: caller.functionMouseUp(); break;
		case 300: caller.functionMakeMEMBlock(); break;
		case 302: caller.functionMEMBlockSize(); break;
		case 305: caller.functionPeekByte(); break;
		case 306: caller.functionPeekShort(); break;
		case 307: caller.functionPeekInt(); break;
		case 308: caller.functionPeekFloat(); break;
		case 320: caller.functionOpenToRead(); break;
		case 321: caller.functionOpenToWrite(); break;
		case 322: caller.functionOpenToEdit(); break;
		case 324: caller.functionFileOffset(); break;
		case 328: caller.functionFindFile(); break;
		case 329: caller.functionCurrentDir(); break;
		case 333: caller.functionFileExists(); break;
		case 334: caller.functionIsDirectory(); break;
		case 335: caller.functionFileSize(); break;
		case 339: caller.functionEOF(); break;
		case 341: caller.functionReadByte(); break;
		case 342: caller.functionReadShort(); break;
		case 343: caller.functionReadInt(); break;
		case 344: caller.functionReadFloat(); break;
		case 345: caller.functionReadString(); break;
		case 346: caller.functionReadLine(); break;
		case 400: caller.functionPlayAnimation(); break;
		case 403: caller.functionAnimationWidth(); break;
		case 404: caller.functionAnimationHeight(); break;
		case 405: caller.functionAnimationPlaying(); break;
		case 415: caller.functionCrc32(); break;
		case 420: caller.functionDate(); break;
		case 421: caller.functionTime(); break;
		case 422: caller.functionTimer(); break;
		case 424: caller.functionCommandLine(); break;
		case 427: caller.functionGetEXEName(); break;
		case 432: caller.functionFPS(); break;
		case 450: caller.functionPlaySound(); break;
		case 452: caller.functionLoadSound(); break;
		case 455: caller.functionSoundPlaying(); break;
		case 481: caller.functionSCREEN(); break;
		case 484: caller.functionGetPixel(); break;
		case 486: caller.functionGetPixel2(); break;
		case 500: caller.functionGetRGB(); break;
		case 501: caller.functionSCREEN(); break;
		case 502: caller.functionImage(); break;
		case 507: caller.functionScreenWidth(); break;
		case 508: caller.functionScreenHeight(); break;
		case 509: caller.functionScreenDepth(); break;
		case 510: caller.functionGFXModeExists(); break;
		case 520: caller.functionLoadImage(); break;
		case 521: caller.functionLoadAnimImage(); break;
		case 522: caller.functionMakeImage(); break;
		case 523: caller.functionCloneImage(); break;
		case 530: caller.functionImageWidth(); break;
		case 531: caller.functionImageHeight(); break;
		case 532: caller.functionImagesOverlap(); break;
		case 533: caller.functionImagesCollide(); break;
		case 600: caller.functionLoadObject(); break;
		case 601: caller.functionLoadAnimObject(); break;
		case 602: caller.functionMakeObject(); break;
		case 608: caller.functionMakeObjectFloor(); break;
		case 615: caller.functionCloneObject(); break;
		case 638: caller.functionObjectInteger(); break;
		case 639: caller.functionObjectFloat(); break;
		case 640: caller.functionObjectString(); break;
		case 643: caller.functionPickedObject(); break;
		case 644: caller.functionPickedX(); break;
		case 645: caller.functionPickedY(); break;
		case 649: caller.functionObjectLife(); break;
		case 670: caller.functionObjectAngle(); break;
		case 674: caller.functionObjectX(); break;
		case 675: caller.functionObjectY(); break;
		case 677: caller.functionObjectSizeX(); break;
		case 678: caller.functionObjectSizeY(); break;
		case 680: caller.functionObjectPlaying(); break;
		case 690: caller.functionObjectFrame(); break;
		case 693: caller.functionCameraX(); break;
		case 694: caller.functionCameraY(); break;
		case 696: caller.functionCameraAngle(); break;
		case 700: caller.functionObjectsOverlap(); break;
		case 701: caller.functionObjectSight(); break;
		case 702: caller.functionCountCollisions(); break;
		case 706: caller.functionGetCollision(); break;
		case 709: caller.functionCollisionX(); break;
		case 710: caller.functionCollisionY(); break;
		case 712: caller.functionCollisionAngle(); break;
		case 750: caller.functionLoadMap(); break;
		case 751: caller.functionMakeMap(); break;
		case 753: caller.functionGetMap(); break;
		case 754: caller.functionGetMap2(); break;
		case 755: caller.functionMapWidth(); break;
		case 756: caller.functionMapHeight(); break;
		case 790: caller.functionMakeEmitter(); break;
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
	for (int32_t i = 0; i != n; ++i) {
		dimensions[i] = popValue().toInt() + 1; // Size of dimension
	}


	switch (type){
		case 3: {
			Array<int32_t> a;
			for (int i = n - 1; i >= 0;--i) {
				a.setDimensions(i, size);
				size *= dimensions[i];
			}
			a.init(size);
			setArray(arrId, a);
			break;
		}
		case 7: {
			Array<uint16_t> a;
			for (int i = n - 1; i >= 0;--i) {
				a.setDimensions(i, size);
				size *= dimensions[i];
			}
			a.init(size);
			setArray(arrId, a);
			break;
		}
		case 8: {
			Array<uint8_t> a;
			for (int i = n - 1; i >= 0;--i) {
				a.setDimensions(i, size);
				size *= dimensions[i];
			}
			a.init(size);
			setArray(arrId, a);
			break;
		}
		case 4: {
			Array<float> a;
			for (int i = n - 1; i >= 0;--i) {
				a.setDimensions(i, size);
				size *= dimensions[i];
			}
			a.init(size);
			setArray(arrId, a);
			break;
		}
		case 6: {
			Array<ISString> a;
			for (int i = n - 1; i >= 0;--i) {
				a.setDimensions(i, size);
				size *= dimensions[i];
			}
			a.init(size);
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

	uint32_t pos = popArrayDimensions1(id,n,type);
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
	//HCDEBUG("[%i] Push variable: %i",cpos,type);
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
		case 8:
		{
			uint32_t id = *(uint32_t *)(code);
			code += 4;

			int32_t dimensions = popValue().getInt();
			uint32_t pos = popArrayDimensions2(id, dimensions, type);

			switch (type){
				case 3:
					pushValue(getIntegerArray(id).get(pos));break;
				case 4:
					pushValue(getFloatArray(id).get(pos));break;
				break;
				case 7:
					pushValue((int32_t)getShortArray(id).get(pos));break;
				case 8:
					pushValue((int32_t)getByteArray(id).get(pos));break;

				case 6:
					pushValue(getStringArray(id).get(pos));break;
				default:
					FIXME("handlePushSomething: Undefined array type %i", type);
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
		case 1: Any::unaryMinus(&internalStack); break;
		case 2: Any::unaryPlus(&internalStack); break;
		case 3: Any::power(&internalStack); break;
		case 4: Any::addition(&internalStack); break;
		case 5: Any::subtraction(&internalStack); break;
		case 6: Any::multiplication(&internalStack); break;
		case 7: Any::division(&internalStack); break;
		case 8: Any::modulo(&internalStack); break;
		case 9: Any::shl(&internalStack); break;
		case 10: Any::shr(&internalStack); break;
		case 11: Any::sar(&internalStack); break;
		case 12: Any::lessThan(&internalStack); break;
		case 13: Any::greaterThan(&internalStack); break;
		case 14: Any::equal(&internalStack); break;
		case 15: Any::notEqual(&internalStack); break;
		case 16: Any::lessThanOrEqual(&internalStack); break;
		case 17: Any::greaterThanOrEqual(&internalStack); break;
		case 18: Any::AND(&internalStack); break;
		case 19: Any::OR(&internalStack); break;
		case 20: Any::XOR(&internalStack); break;
		case 21: Any::NOT(&internalStack); break;
		default:
			FIXME("Unimplemented mathematical operation: %i", op);
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

FORCEINLINE void CBEnchanted::handlePushTypeMemberVariable() {
	void * typePtr = getTypePointerVariable(*((int32_t*)(code)));
	code += 4;
	int32_t varType = popValue().getInt();
	int32_t place = popValue().getInt() - 12;
	switch (varType) {
		case 1: pushValue(Type::getMembersType(typePtr)->getIntField(typePtr, place)); break;
		case 2: pushValue(Type::getMembersType(typePtr)->getFloatField(typePtr, place)); break;
		case 3: pushValue(Type::getMembersType(typePtr)->getStringField(typePtr, place)); break;
		default: FIXME("handlePushTypeMemberVariable:Unhandled varType %i", varType); break;
	}
}

FORCEINLINE void CBEnchanted::handleCustomFunctionCall() {
	int32_t handle = *(int32_t *)(code);
	code += 4;
	customFunctionHandler.call(this,handle);
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
}

void CBEnchanted::commandInsert(void) {
	STUB;
}

void CBEnchanted::commandClearArray(void) {
	STUB;
}

void CBEnchanted::commandReDim(void) {
	STUB;
}

FORCEINLINE void CBEnchanted::commandReturn(void) {
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
	pushValue(getType(typeId)->getFirst());
}

void CBEnchanted::functionBefore(void) {
	void *typeMember = popValue().getTypePtr();
	pushValue(Type::getBefore(typeMember));
}

void CBEnchanted::functionAfter(void) {
	void *typeMember = popValue().getTypePtr();
	pushValue(Type::getAfter(typeMember));
}

void CBEnchanted::functionRead(void) {

}

void CBEnchanted::functionConvertToInteger(void) {
	void *typeMember = popValue().getTypePtr();
	if (BUILD_32_BIT) {
		pushValue(reinterpret_cast<int32_t>(typeMember));
	}
	else {
		FIXME("ConvertToInteger() doesn't work yet on 64-bit builds!");
		pushValue(0);
	}
}

void CBEnchanted::functionConvertToType(void) {
	int32_t typePtr = popValue().getInt();
	if (BUILD_32_BIT) {
		pushValue(reinterpret_cast<void*>(typePtr));
	}
	else {
		FIXME("ConvertToType() doesn't work yet on 64-bit builds!");
		pushValue(0);
	}
}

FORCEINLINE uint32_t CBEnchanted::popArrayDimensions1(int32_t arrayId, int32_t n, int32_t type)
{
	uint32_t pos = 0;
	switch (type) {
		case 1: {
			Array<int32_t> &a = getIntegerArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 4: {
			Array<uint16_t> &a = getShortArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 5: {
			Array<uint8_t> &a = getByteArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 2: {
			Array<float> &a = getFloatArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 3: {
			Array<ISString> &a = getStringArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
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
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 7: {
			Array<uint16_t> &a = getShortArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 8: {
			Array<uint8_t> &a = getByteArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 4: {
			Array<float> &a = getFloatArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
			break;
		}
		case 6: {
			Array<ISString> &a = getStringArray(arrayId);
			for (int32_t i = 0; i != n; ++i) {
				pos += popValue().getInt() * a.getDimensions(i);
			}
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
			void * ptr = popValue().getTypePtr();

			setTypePointerVariable(id,ptr);
			break;
		}
		default:
			FIXME("Unimplemented SetVariable. Type: %i", type);
	}
	HCDEBUG("Push value, type: %i", type);
}

void CBEnchanted::commandSetGlobalVariable() {
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

void CBEnchanted::commandSetArrayNumbers() {
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	initArrays(byteCount, shortCount, stringCount, floatCount, integerCount);
}

void CBEnchanted::commandSetGlobalVariableNumbers() {
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	initGlobalVars(byteCount, shortCount, stringCount, floatCount, integerCount);
}

void CBEnchanted::commandType(void)
{
	int32_t typeMemberSize = popValue().getInt();
	addType(typeMemberSize - 4);
	//cpos += 5;
}

void CBEnchanted::commandSetTypeMemberField(void)
{
	int32_t varType = popValue().getInt();
	void * typePtr = getTypePointerVariable(popValue().getInt());
	int32_t place = popValue().getInt() - 12;
	switch (varType) {
		case 1:
			Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toInt());
		break;
		case 2:
			Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toFloat());
		break;
		case 3:
			Type::getMembersType(typePtr)->setField(typePtr, place, popValue().toString());
		break;
		default:
			FIXME("setTypeMemberField:Unhandled varType %i", varType);
	}
}

void CBEnchanted::commandSetVariableNumbers() {
	//TODO: Check if right order
	int32_t typePtrCount = popValue().getInt();
	int32_t shortCount = popValue().getInt();
	int32_t byteCount = popValue().getInt();
	int32_t stringCount = popValue().getInt();
	int32_t floatCount = popValue().getInt();
	int32_t integerCount = popValue().getInt();
	pushScope(byteCount, shortCount, stringCount, floatCount, integerCount, typePtrCount);
}

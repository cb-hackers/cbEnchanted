#include "precomp.h"
#include "cbemu.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include "stringinterface.h"
#include "textinterface.h"
#include "inputinterface.h"
#include "mathinterface.h"
#include "cbvariableholder.h"
#include "mathoperations.h"

CBEmu::CBEmu() {
	initialized = false;
	running = false;
	cpos = 0;
}

CBEmu::~CBEmu() {
	delete code;
}

/*
 * CBEmu::run - Interpret CoolBasic bytecode
 */
void CBEmu::run() {
	// Make sure that we are initialized properly to avoid crashing
	assert(initialized == true);

	// Run until told to quit
	running = true;
	while (getWindow()->IsOpened()) {
		uint32_t opCode = (uint32_t)code[cpos++];

		HCDEBUG("OpCode: %1", opCode);
		switch (opCode) {
			case 65: handleSetInt(); break;
			case 66: handleSetFloat(); break;
			case 67: handleCommand(); break;
			case 73: handlePushInt(); break;
			case 74: handlePushSomething(); break;
			case 78: handleJump(); break;
			case 79: handleMathOperation(); break;
			case 80: handleIncVar(); break;
			case 86: handlePushVariable(); break;
			case 90: handleFunction(); break;
			case 97:
			case 98:
			case 99: uselessShitHandler(); break;
			default: FIXME("Unimplemented handler: %1", opCode);
		}
	}
}

/* 
 * CBEmu::init - Initialize the interpreter 
 */
void CBEmu::init(string file) {
	INFO("Initializing");
	int32_t startPos; // Beginning of the CoolBasic data
	int32_t endPos; // End of the executable
	
	int32_t nStrings; // Number of strings
	int32_t size; // Length of CoolBasic data
	
	// Open file for reading
	ifstream input(file.c_str(), ios::binary);

	assert(input.is_open());

	input.seekg(-4, ios::end);
	endPos = input.tellg();
	input.read((char *)(&startPos), 4);
	input.seekg(24 - startPos, ios::end);
	input.read((char *)(&nStrings), 4);
	
	// Read and decrypt strings
	string key = "Mark Sibly is my idol!";
	for (uint32_t i = 1; i <= nStrings; i++) {
		uint32_t len;
		input.read((char *)(&len), 4);
		string s = "";
		char c;
		for (uint32_t j = 0; j < len; j++) {
			input >> c;
			s += c - key[i % key.length()];
		}
		setString(i, s);
	}
	
	// Skip useless data and directly to beginning of the bytecode
	input.seekg(32, ios::cur);
	startPos = input.tellg();
	
	// Read code to memory and close the file
	size = endPos - startPos;
	code = new char [size];
	input.read(code, size);
	input.close();
	
	// Precalculation
	// ToDo:	JIT-optimization?
	// 			Handle functions and types
	uint32_t ncmd = 0;
	uint32_t i = 0;
	while (i < size) {
		offsets[ncmd] = i;
		roffsets[i] = ncmd;
		
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
			case 86:
			case 90: i += 4; break;
			case 79: i ++; break;
			default: FIXME("Unhandled preparsing: %1", (uint32_t) cmd);
		}
		
	}

	assert(i == size);

	initialized = true;
	INFO("Initialized");
}

void CBEmu::stop() {
	running = false;
}

void CBEmu::cleanup() {
	
}

/*
 * CBEmu::handleSetInt - Set value of integer
 */
void CBEmu::handleSetInt(void) {
	uint32_t var = *(uint32_t *)(code + cpos);
	cpos += 4;

	setIntegerVariable(var, popValue<int32_t>());
}

/*
 * CBEmu::handleSetFloat - Set value of float
 */
void CBEmu::handleSetFloat(void) {
	uint32_t var = *(uint32_t *)(code + cpos);
	cpos += 4;
	
	setFloatVariable(var, popValue<float>());
}

/*
 * CBEmu::handleCommand - Run command
 */
void CBEmu::handleCommand(void) {
	uint32_t command = *(uint32_t *)(code + cpos);
	cpos += 4;
	
	HCDEBUG("Command: %1", command);

	switch (command) {
		/*case 1: commandIf(); break;
		case 3: commandElse(); break;
		case 4: commandElseIf(); break;
		case 5: commandEndIf(); break;
		case 6: commandSelect(); break;
		case 7: commandCase(); break;
		case 8: commandDefault(); break;
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
		case 65: commandInitObjectList(); break;
		case 69: commandEnd(); break;
		case 125: commandRandomize(); break;
		case 201: commandSetFont(); break;
		case 202: commandDeleteFont(); break;
		case 205: commandText(); break;
		case 206: commandCenterText(); break;
		case 207: commandPrint(); break;
		case 208: commandWrite(); break;
		case 209: commandLocate(); break;
		case 210: commandClearText(); break;
		case 211: commandAddText(); break;
		case 212: commandVerticalText(); break;
		case 224: commandWaitKey(); break;
		case 225: commandClearKeys(); break;
		case 226: commandPositionMouse(); break;
		case 230: commandWaitMouse(); break;
		case 237: commandShowMouse(); break;
		case 238: commandClearMouse(); break;
		case 239: commandSAFEEXIT(); break;
		case 301: commandDeleteMEMBlock(); break;
		case 303: commandResizeMEMBlock(); break;
		case 304: commandMemCopy(); break;
		case 309: commandPokeByte(); break;
		case 310: commandPokeShort(); break;
		case 311: commandPokeInt(); break;
		case 312: commandPokeFloat(); break;
		case 323: commandCloseFile(); break;
		case 325: commandSeekFile(); break;
		case 326: commandStartSearch(); break;
		case 327: commandEndSearch(); break;
		case 330: commandChDir(); break;
		case 331: commandMakeDir(); break;
		case 336: commandCopyFile(); break;
		case 337: commandDeleteFile(); break;
		case 338: commandExecute(); break;
		case 341: commandReadByte(); break;
		case 342: commandReadShort(); break;
		case 343: commandReadInt(); break;
		case 344: commandReadFloat(); break;
		case 345: commandReadFloat(); break;
		case 346: commandReadString(); break;
		case 347: commandReadLine(); break;
		case 348: commandWriteByte(); break;
		case 349: commandWriteShort(); break;
		case 350: commandWriteInt(); break;
		case 351: commandWriteFloat(); break;
		case 352: commandWriteString(); break;
		case 353: commandWriteLine(); break;
		case 401: commandStopAnimation(); break;
		case 402: commandDrawAnimation(); break;
		case 416: commandEncrypt(); break;
		case 417: commandDecrypt(); break;
		case 423: commandWait(); break;
		case 425: commandSetWindow(); break;
		case 426: commandMakeError(); break;
		case 428: commandSaveProgram(); break;
		case 429: commandLoadProgram(); break;
		case 430: commandGotoSavedLocation(); break;
		case 431: commandFrameLimit(); break;
		case 436: commandCallDLL(); break;
		case 437: commandErrors(); break;
		case 450: commandPlaySound(); break;
		case 451: commandSetSound(); break;
		case 453: commandDeleteSound(); break;
		case 454: commandStopSound(); break;
		case 481: commandScreen(); break;
		case 482: commandLock(); break;
		case 483: commandUnlock(); break;
		case 485: commandPutPixel(); break;
		case 487: commandPutPixel2(); break;
		case 490: commandCopyBox(); break;
		case 491: commandColor(); break;
		case 492: commandClsColor(); break;
		case 493: commandCls(); break;
		case 494: commandDot(); break;
		case 495: commandLine(); break;
		case 496: commandBox(); break;
		case 497: commandEllipse(); break;
		case 498: commandCircle(); break;
		case 499: commandPickColor(); break;
		case 503: commandScreenGamma(); break;
		case 504: commandDrawToImage(); break;
		case 505: commandDrawToScreen(); break;
		case 506: commandSmooth2D(); break;
		case 511: commandScreenShot(); break;
		case 512: commandDrawGame(); break;
		case 513: commandDrawScreen(); break;
		case 514: commandUpdateGame(); break;
		case 519: commandDrawToWorld(); break;
		case 524: commandSaveImage(); break;
		case 525: commandDrawImage(); break;
		case 526: commandDrawImageBox(); break;
		case 527: commandMaskImage(); break;
		case 528: commandResizeImage(); break;
		case 529: commandRotateImage(); break;
		case 534: commandDeleteImage(); break;
		case 535: commandDefaultMask(); break;
		case 536: commandHotSpot(); break;
		case 537: commandPickImageColor(); break;
		case 538: commandPickImageColor2(); break;
		case 539: commandDrawGhostImage(); break;
		case 616: commandDeleteObject(); break;
		case 617: commandMoveObject(); break;
		case 618: commandTranslateObject(); break;
		case 619: commandPositionObject(); break;
		case 620: commandCloneObjectPosition(); break;
		case 621: commandCloneCameraPosition(); break;
		case 622: commandTurnObject(); break;
		case 623: commandRotateObject(); break;
		case 624: commandPointObject(); break;
		case 625: commandCloneObjectOrientation(); break;
		case 626: commandCloneCameraOrientation(); break;
		case 628: commandObjectOrder(); break;
		case 629: commandMaskObject(); break;
		case 630: commandShowObject(); break;
		case 631: commandGhostObject(); break;
		case 633: commandPaintObject(); break;
		case 635: commandObjectRange(); break;
		case 638: commandObjectInteger(); break;
		case 639: commandObjectFloat(); break;
		case 640: commandObjectString(); break;
		case 641: commandObjectPickable(); break;
		case 642: commandObjectPick(); break;
		case 647: commandCameraPick(); break;
		case 648: commandScreenPositionObject(); break;
		case 649: commandObjectLife(); break;
		case 650: commandPlayObject(); break;
		case 651: commandLoopObject(); break;
		case 652: commandStopObject(); break;
		case 703: commandClearCollisions(); break;
		case 704: commandResetObjectCollision(); break;
		case 705: commandSetupCollision(); break;
		case 723: commandPointCamera(); break;
		case 724: commandTurnCamera(); break;
		case 725: commandRotateCamera(); break;
		case 726: commandMoveCamera(); break;
		case 727: commandTranslateCamera(); break;
		case 728: commandPositionCamera(); break;
		case 729: commandCameraFollow(); break;
		case 752: commandEditMap(); break;
		case 757: commandSetMap(); break;
		case 758: commandSetTile(); break;
		case 791: commandParticleMovement(); break;
		case 792: commandParticleAnimation(); break;
		case 793: commandParticleEmission(); break;
		case 796: commandDefaultVisible(); break;
		case 797: commandMirrorObject(); break;
		case 798: commandPixelPick(); break;
		case 799: commandClearObjects(); break;
		/*case 12: commandGoto(); break;
		case 42: commandDim(); break;
		case 69: commandEnd(); break;
		case 78: commandArrayAssign(); break;
		case 97:
		case 98: command97_98(); break;
		case 99: command99(); break;
		case 207: commandPrint(); break;
		case 224: commandWaitKey(); break;
		case 425: commandSetWindow(); break;
		case 481: commandScreen(); break;
		case 491: commandColor(); break;
		case 492: commandClsColor(); break;
		case 498: commandCircle(); break;
		case 513: commandDrawScreen(); break;*/
		default: FIXME("Unimplemented command: %1", command);
	}
}

/*
 * CBEmu::handleFunction - Run function
 */
void CBEmu::handleFunction(void) {
	uint32_t func = *(uint32_t *)(code + cpos);
	cpos += 4;
	HCDEBUG("Function: %1", func);

	switch(func) {
		case 106: functionSin(); break;
		case 107: functionCos(); break;
		case 122: functionWrapAngle(); break;
		case 150: functionStr(); break;
		case 442: functionTimer(); break;
		default: FIXME("Unimplemented function: %1", func);
	}
}

/*
 * CBEmu::handlePushInt - Push integer to stack
 */
void CBEmu::handlePushInt(void) {
	pushValue(*(int32_t *)(code + cpos));
	cpos += 4;
}

/*
 * CBEmu::uselessShitHandler - Do nothing
 */ 
void CBEmu::uselessShitHandler(void) {
	
}

/*
 * CBEmu::command97_98 - Pop 5 values from stack
 */
void CBEmu::command97_98(void) {
	for (uint32_t i = 0; i < 5; i++) {
		popValue();
	}
}

/*
 * CBEmu::command99 - Pop 6 values from stack
 */
void CBEmu::command99(void) {
	for (uint32_t i = 0; i < 6; i++) {
		popValue();
	}
}

/*
 * CBEmu::commandDim - Create array
 */
void CBEmu::commandDim(void) {
	cpos ++;
	uint32_t n = *(uint32_t *)(code + cpos); // Number of dimensions
	cpos += 4;
	
	uint32_t size = 1;
	Array a;
	for (int32_t i = n - 1; i >= 0; --i) {
		int32_t dim = popValue<int32_t>() + 1; // Size of dimension
		a.dimensions[i] = dim;
		size *= dim;
	}
	
	cpos += 1;
	uint32_t type = *(uint32_t *)(code + cpos);
	cpos += 4;
	cpos += 1;
	uint32_t arrId = *(uint32_t *)(code + cpos); // Array ID
	cpos += 4;
	a.type = type;
	a.data = new boost::any [size];
	setArray(arrId, a);
}

/*
 * CBEmu::commandArrayAssing - Assing value of array element
 */
void CBEmu::commandArrayAssign(void) {
	uint32_t type = popValue<int32_t>();
	cpos ++;
	uint32_t n = *(uint32_t *)(code + cpos);
	cpos += 4;
	cpos += 1;
	uint32_t id = *(uint32_t *)(code + cpos);
	cpos += 4;

	uint32_t pos = 0;
	for (int32_t i = n - 1; i >= 0; --i) {
		if (i != n - 1) {
			pos += popValue<int32_t>() * getArray(id).dimensions[i + 1];
		}
		else {
			pos += popValue<int32_t>();
		}
	}
	getArray(id).data[pos] = popValue();
}

/*
 * CBEmu::handlePushVariable - Push value of variable to stack
 */
void CBEmu::handlePushVariable(void) {
	uint32_t type = popValue<int32_t>();
	
	switch (type) {
		case 1:
			pushValue(getIntegerVariable(*(int32_t *)(code + cpos)));
			cpos += 4;
			break;
		case 2:
			pushValue(getFloatVariable(*(int32_t *)(code + cpos)));
			cpos += 4;
			break;
		default:
			FIXME("Unimplemented variable push: %1", type);
	}
}

/*
 * CBEmu::handlePushSomething - Push float, string or something else to stack
 */
void CBEmu::handlePushSomething(void) {
	uint32_t type = popValue<int32_t>();
	switch (type) {
		case 2: { // Float
			float value = *(float *)(code + cpos);
			cpos += 4;
			
			pushValue(value);
			break;
		}
		case 5: { // String
			uint32_t strId = *(uint32_t *)(code + cpos);
			cpos += 4;
			
			if (strId == 0) {
				pushValue(string(""));
			}
			else {
				cout << "FIXME: Push string" << endl;
			}
			break;
		}
		default: {
			FIXME("Unimplemented push: %1", type);
		}
	}
}

/*
 * CBEmu::handleMathOperation - Handle mathematical operation
 */
void CBEmu::handleMathOperation(void) {
	uint8_t op = *(uint8_t *)(code + cpos);
	cpos++;
	HCDEBUG("Mathoperation: %1", uint32_t(op));
	switch (op) {
		case 1: {
			boost::any r = popValue();
			
			pushValue(-r);
			break;
		}
		case 2: {
			boost::any r = popValue();
			
			pushValue(+r);
			break;
		}
		case 3: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l ^ r);
			break;
		}
		case 4: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l + r);
			break;
		}
		case 5: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l - r);
			break;
		}
		case 6: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l * r);
			break;
		}
		case 7: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l / r);
			break;
		}
		case 8: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l % r);
			break;
		}
		case 9: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l << r);
			break;
		}
		case 10: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l >> r);
			break;
		}
		case 11: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l >> r);
			break;
		}
		case 12: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l < r);
			break;
		}
		case 13: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l > r);
			break;
		}
		case 14: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l == r);
			break;
		}
		case 15: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l != r);
			break;
		}
		case 16: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l <= r);
			break;
		}
		case 17: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l >= r);
			break;
		}
		case 18: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l && r);
			break;
		}
		case 19: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue(l || r);
			break;
		}
		case 20: {
			boost::any r = popValue();
			boost::any l = popValue();
			
			pushValue((l || r) && !(r && l));
			break;
		}
		case 21: {
			boost::any r = popValue();

			pushValue(!r);
			break;
		}
		default:
			FIXME("Unimplemented mathematical operation: %1", op);
	}
}

/*
 * CBEmu::handleJump - Jump if last operation was true
 */
void CBEmu::handleJump(void) {
	uint32_t dest = *(uint32_t *)(code + cpos);
	cpos += 4;

	if (!popValue<int32_t>()) {
		cpos = offsets[dest];
	}
}

/*
 * CBEmu::handleIncVar - Increase integer variable
 */
void CBEmu::handleIncVar(void) {
	setIntegerVariable(*(uint32_t *)(code + cpos), getIntegerVariable(*(uint32_t *)(code + cpos)) + 1);
	cpos += 4;
}

/*
 * CBEmu::commandGoto - Jump to different location
 */
void CBEmu::commandGoto(void) {
	cpos ++;
	cpos = offsets[*(uint32_t *)(code + cpos)];
}

void CBEmu::commandDelete(void) {
	
}

void CBEmu::commandInsert(void) {
	
}

void CBEmu::commandClearArray(void) {
	
}

void CBEmu::commandReDim(void) {
	
}

void CBEmu::commandReturn(void) {
	
}

void CBEmu::commandGosub(void) {
	
}

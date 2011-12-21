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

CBEnchanted::CBEnchanted() {
	initialized = false;
	running = false;
	cpos = 0;
}

CBEnchanted::~CBEnchanted() {
	delete code;
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
		uint32_t opCode = (uint32_t)code[cpos++];

		HCDEBUG("OpCode: %i", opCode);
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
			default: FIXME("Unimplemented handler: %i", opCode);
		}
	}
}

/* 
 * CBEnchanted::init - Initialize the interpreter 
 */
void CBEnchanted::init(string file) {
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
			default: FIXME("Unhandled preparsing: %i", (uint32_t) cmd);
		}
		
	}

	assert(i == size);

	initialized = true;
	INFO("Initialized");
}

void CBEnchanted::stop() {
	running = false;
}

void CBEnchanted::cleanup() {
	
}

/*
 * CBEnchanted::handleSetInt - Set value of integer
 */
void CBEnchanted::handleSetInt(void) {
	uint32_t var = *(uint32_t *)(code + cpos);
	cpos += 4;

    setIntegerVariable(var, popValue().getInt());
}

/*
 * CBEnchanted::handleSetFloat - Set value of float
 */
void CBEnchanted::handleSetFloat(void) {
	uint32_t var = *(uint32_t *)(code + cpos);
	cpos += 4;
	
    setFloatVariable(var, popValue().getFloat());
}

/*
 * CBEnchanted::handleCommand - Run command
 */
void CBEnchanted::handleCommand(void) {
	uint32_t command = *(uint32_t *)(code + cpos);
	cpos += 4;
	
	HCDEBUG("Command: %i", command);

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
		case 78: commandArrayAssign(); break;
		case 97:
		case 98: command97_98(); break;
		case 99: command99(); break;
		default: FIXME("Unimplemented command: %i", command);
	}
}

/*
 * CBEnchanted::handleFunction - Run function
 */
void CBEnchanted::handleFunction(void) {
	uint32_t func = *(uint32_t *)(code + cpos);
	cpos += 4;
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
		case 66: functionNextObject(); break;
		case 100: functionInt(); break;
		case 101: functionFloat(); break;
		case 102: functionRoundUp(); break;
		case 103: functionRoundDown(); break;
		case 104: functionAbs(); break;
		case 105: functionSqrt(); break;
		case 106: functionSin(); break;
		case 107: functionCos(); break;
		case 108: functionTan(); break;
		case 109: functionASin(); break;
		case 110: functionACos(); break;
		case 111: functionATan(); break;
		case 112: functionGetAngle(); break;
		case 113: functionGetAngle2(); break;
		case 114: functionLog(); break;
		case 115: functionLog10(); break;
		case 116: functionRnd(); break;
		case 117: functionRand(); break;
		case 118: functionMin(); break;
		case 119: functionMax(); break;
		case 120: functionCurveValue(); break;
		case 121: functionCurveAngle(); break;
		case 122: functionWrapAngle(); break;
		case 123: functionDistance(); break;
		case 124: functionDistance2(); break;
		case 126: functionBoxOverlap(); break;
		case 150: functionStr(); break;
		case 151: functionLeft(); break;
		case 152: functionRight(); break;
		case 153: functionMid(); break;
		case 154: functionReplace(); break;
		case 155: functionInStr(); break;
		case 156: functionUpper(); break;
		case 157: functionLower(); break;
		case 158: functionTrim(); break;
		case 159: functionLSet(); break;
		case 160: functionRSet(); break;
		case 161: functionChr(); break;
		case 162: functionAsc(); break;
		case 163: functionLen(); break;
		case 164: functionHex(); break;
		case 165: functionBin(); break;
		case 166: functionString(); break;
		case 167: functionFlip(); break;
		case 168: functionStrInsert(); break;
		case 169: functionStrRemove(); break;
		case 170: functionStrMove(); break;
		case 171: functionCountWords(); break;
		case 172: functionGetWord(); break;
		case 200: functionLoadFont(); break;
		case 203: functionTextWidth(); break;
		case 204: functionTextHeight(); break;
		case 220: functionInput(); break;
		case 221: functionKeyDown(); break;
		case 222: functionKeyHit(); break;
		case 223: functionGetKey(); break;
        case 224: functionWaitKey(); break;
		case 227: functionMouseDown(); break;
		case 228: functionMouseHit(); break;
		case 229: functionGetMouse(); break;
		case 230: functionWaitMouse(); break;
		case 231: functionMouseX(); break;
		case 232: functionMouseY(); break;
		case 233: functionMouseZ(); break;
		case 234: functionMouseMoveX(); break;
		case 235: functionMouseMoveY(); break;
		case 236: functionMouseMoveZ(); break;
		case 241: functionLeftKey(); break;
		case 242: functionRightKey(); break;
		case 243: functionUpKey(); break;
		case 244: functionDownKey(); break;
		case 245: functionEscapeKey(); break;
		case 246: functionMouseWX(); break;
		case 247: functionMouseWY(); break;
		case 248: functionKeyUp(); break;
		case 249: functionMouseUp(); break;
		case 300: functionMakeMEMBlock(); break;
		case 302: functionMEMBlockSize(); break;
		case 305: functionPeekByte(); break;
		case 306: functionPeekShort(); break;
		case 307: functionPeekInt(); break;
		case 308: functionPeekFloat(); break;
		case 320: functionOpenToRead(); break;
		case 321: functionOpenToWrite(); break;
		case 322: functionOpenToEdit(); break;
		case 324: functionFileOffset(); break;
		case 328: functionFindFile(); break;
		case 329: functionCurrentDir(); break;
		case 333: functionFileExists(); break;
		case 334: functionIsDirectory(); break;
		case 335: functionFileSize(); break;
		case 339: functionEOF(); break;
		case 341: functionReadByte(); break;
		case 342: functionReadShort(); break;
		case 343: functionReadInt(); break;
		case 344: functionReadFloat(); break;
		case 345: functionReadFloat(); break;
		case 346: functionReadString(); break;
		case 400: functionPlayAnimation(); break;
		case 403: functionAnimationWidth(); break;
		case 404: functionAnimationHeight(); break;
		case 405: functionAnimationPlaying(); break;
		case 415: functionCrc32(); break;
		case 420: functionDate(); break;
		case 421: functionTime(); break;
		case 422: functionTimer(); break;
		case 424: functionCommandLine(); break;
		case 427: functionGetEXEName(); break;
		case 432: functionFPS(); break;
		case 450: functionPlaySound(); break;
		case 452: functionLoadSound(); break;
		case 455: functionSoundPlaying(); break;
		case 481: functionSCREEN(); break;
		case 484: functionGetPixel(); break;
		case 486: functionGetPixel2(); break;
		case 500: functionGetRGB(); break;
		case 501: functionSCREEN(); break;
		case 502: functionImage(); break;
		case 507: functionScreenWidth(); break;
		case 508: functionScreenHeight(); break;
		case 509: functionScreenDepth(); break;
		case 510: functionGFXModeExists(); break;
		case 520: functionLoadImage(); break;
		case 521: functionLoadAnimImage(); break;
		case 522: functionMakeImage(); break;
		case 523: functionCloneImage(); break;
		case 530: functionImageWidth(); break;
		case 531: functionImageHeight(); break;
		case 532: functionImagesOverlap(); break;
		case 533: functionImagesCollide(); break;
		case 600: functionLoadObject(); break;
		case 601: functionLoadAnimObject(); break;
		case 602: functionMakeObject(); break;
		case 608: functionMakeObjectFloor(); break;
		case 615: functionCloneObject(); break;
		case 638: functionObjectInteger(); break;
		case 639: functionObjectFloat(); break;
		case 640: functionObjectString(); break;
		case 643: functionPickedObject(); break;
		case 644: functionPickedX(); break;
		case 645: functionPickedY(); break;
		case 649: functionObjectLife(); break;
		case 670: functionObjectAngle(); break;
		case 674: functionObjectX(); break;
		case 675: functionObjectY(); break;
		case 677: functionObjectSizeX(); break;
		case 678: functionObjectSizeY(); break;
		case 680: functionObjectPlaying(); break;
		case 690: functionObjectFrame(); break;
		case 693: functionCameraX(); break;
		case 694: functionCameraY(); break;
		case 696: functionCameraAngle(); break;
		case 700: functionObjectsOverlap(); break;
		case 701: functionObjectSight(); break;
		case 702: functionCountCollisions(); break;
		case 706: functionGetCollision(); break;
		case 709: functionCollisionX(); break;
		case 710: functionCollisionY(); break;
		case 712: functionCollisionAngle(); break;
		case 750: functionLoadMap(); break;
		case 751: functionMakeMap(); break;
		case 753: functionGetMap(); break;
		case 754: functionGetMap2(); break;
		case 755: functionMapWidth(); break;
		case 756: functionMapHeight(); break;
		case 790: functionMakeEmitter(); break;
		default: FIXME("Unimplemented function: %i", func);
	}
}

/*
 * CBEnchanted::handlePushInt - Push integer to stack
 */
void CBEnchanted::handlePushInt(void) {
	pushValue(*(int32_t *)(code + cpos));
	cpos += 4;
}

/*
 * CBEnchanted::uselessShitHandler - Do nothing
 */ 
void CBEnchanted::uselessShitHandler(void) {
	
}

/*
 * CBEnchanted::command97_98 - Pop 5 values from stack
 */
void CBEnchanted::command97_98(void) {
	for (uint32_t i = 0; i < 5; i++) {
		popValue();
	}
}

/*
 * CBEnchanted::command99 - Pop 6 values from stack
 */
void CBEnchanted::command99(void) {
	for (uint32_t i = 0; i < 6; i++) {
		popValue();
	}
}

/*
 * CBEnchanted::commandDim - Create array
 */
void CBEnchanted::commandDim(void) {
	cpos ++;
	uint32_t n = *(uint32_t *)(code + cpos); // Number of dimensions
	cpos += 4;
	
	uint32_t size = 1;
	Array a;
	for (int32_t i = n - 1; i >= 0; --i) {
        int32_t dim = popValue().getInt() + 1; // Size of dimension
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
    a.data = new Any [size];
	setArray(arrId, a);
}

/*
 * CBEnchanted::commandArrayAssing - Assing value of array element
 */
void CBEnchanted::commandArrayAssign(void) {
    uint32_t type = popValue().getInt();
	cpos ++;
	uint32_t n = *(uint32_t *)(code + cpos);
	cpos += 4;
	cpos += 1;
	uint32_t id = *(uint32_t *)(code + cpos);
	cpos += 4;

	uint32_t pos = 0;
	for (int32_t i = n - 1; i >= 0; --i) {
		if (i != n - 1) {
            pos += popValue().getInt() * getArray(id).dimensions[i + 1];
		}
		else {
            pos += popValue().getInt();
		}
	}
	getArray(id).data[pos] = popValue();
}

/*
 * CBEnchanted::handlePushVariable - Push value of variable to stack
 */
void CBEnchanted::handlePushVariable(void) {
    uint32_t type = popValue().getInt();
	
	switch (type) {
		case 1:
			pushValue(getIntegerVariable(*(int32_t *)(code + cpos)));
			cpos += 4;
			break;
		case 2:
			pushValue(getFloatVariable(*(int32_t *)(code + cpos)));
			cpos += 4;
			break;
		case 7:
			pushValue(getShortVariable(*(int32_t*)(code +cpos)));
			cpos += 4;
			break;
		default:
			FIXME("Unimplemented variable push: %i", type);
	}
}

/*
 * CBEnchanted::handlePushSomething - Push float, string or something else to stack
 */
void CBEnchanted::handlePushSomething(void) {
    uint32_t type = popValue().getInt();
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
void CBEnchanted::handleMathOperation(void) {
	uint8_t op = *(uint8_t *)(code + cpos);
	cpos++;
	HCDEBUG("Mathoperation: %i", uint32_t(op));
	switch (op) {
		case 1: {
            Any r = popValue();
			
			pushValue(-r);
			break;
		}
		case 2: {
            Any r = popValue();
			
			pushValue(+r);
			break;
		}
		case 3: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l ^ r);
			break;
		}
		case 4: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l + r);
			break;
		}
		case 5: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l - r);
			break;
		}
		case 6: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l * r);
			break;
		}
		case 7: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l / r);
			break;
		}
		case 8: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l % r);
			break;
		}
		case 9: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l << r);
			break;
		}
		case 10: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l >> r);
			break;
		}
		case 11: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l >> r);
			break;
		}
		case 12: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l < r);
			break;
		}
		case 13: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l > r);
			break;
		}
		case 14: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l == r);
			break;
		}
		case 15: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l != r);
			break;
		}
		case 16: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l <= r);
			break;
		}
		case 17: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l >= r);
			break;
		}
		case 18: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l && r);
			break;
		}
		case 19: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue(l || r);
			break;
		}
		case 20: {
            Any r = popValue();
            Any l = popValue();
			
			pushValue((l || r) && !(r && l));
			break;
		}
		case 21: {
            Any r = popValue();

			pushValue(!r);
			break;
		}
		default:
			FIXME("Unimplemented mathematical operation: %i", op);
	}
}

/*
 * CBEnchanted::handleJump - Jump if last operation was true
 */
void CBEnchanted::handleJump(void) {
	uint32_t dest = *(uint32_t *)(code + cpos);
	cpos += 4;

    if (!popValue().getInt()) {
		cpos = offsets[dest];
	}
}

/*
 * CBEnchanted::handleIncVar - Increase integer variable
 */
void CBEnchanted::handleIncVar(void) {
	setIntegerVariable(*(uint32_t *)(code + cpos), getIntegerVariable(*(uint32_t *)(code + cpos)) + 1);
	cpos += 4;
}

/*
 * CBEnchanted::commandGoto - Jump to different location
 */
void CBEnchanted::commandGoto(void) {
	cpos ++;
	cpos = offsets[*(uint32_t *)(code + cpos)];
}


void CBEnchanted::commandDelete(void) {
	
}

void CBEnchanted::commandInsert(void) {
	
}

void CBEnchanted::commandClearArray(void) {
	
}

void CBEnchanted::commandReDim(void) {
	
}

void CBEnchanted::commandReturn(void) {
	
}

void CBEnchanted::commandGosub(void) {
	
}

void CBEnchanted::functionNew(void) {
	
}

void CBEnchanted::functionFirst(void) {
	
}

void CBEnchanted::functionLast(void) {
	
}

void CBEnchanted::functionBefore(void) {
	
}

void CBEnchanted::functionAfter(void) {
	
}

void CBEnchanted::functionRead(void) {
	
}

void CBEnchanted::functionConvertToInteger(void) {
	
}

void CBEnchanted::functionConvertToType(void) {
	
}

#include "interfacecaller.h"
#include "interfacesystem.h"
#include "cbvariableholder.h"

InterfaceCaller::InterfaceCaller(CBVariableHolder *s):
	system( new InterfaceSystem),
	stack(s)
{
}

bool InterfaceCaller::init() {
	return system->init();
}


void InterfaceCaller::commandAddText(void) {
	// ...
}

void InterfaceCaller::commandBox(void) {
	// ...
}

void InterfaceCaller::commandCallDLL(void) {
	// ...
}

void InterfaceCaller::commandCameraFollow(void) {
	// ...
}

void InterfaceCaller::commandCameraPick(void) {
	// ...
}

void InterfaceCaller::commandCenterText(void) {
	// ...
}

void InterfaceCaller::commandChDir(void) {
	// ...
}

void InterfaceCaller::commandCircle(void) {
	// ...
}

void InterfaceCaller::commandClearCollisions(void) {
	// ...
}

void InterfaceCaller::commandClearKeys(void) {
	// ...
}

void InterfaceCaller::commandClearMouse(void) {
	// ...
}

void InterfaceCaller::commandClearObjects(void) {
	// ...
}

void InterfaceCaller::commandClearText(void) {
	// ...
}

void InterfaceCaller::commandCloneCameraOrientation(void) {
	// ...
}

void InterfaceCaller::commandCloneCameraPosition(void) {
	// ...
}

void InterfaceCaller::commandCloneObjectOrientation(void) {
	// ...
}

void InterfaceCaller::commandCloneObjectPosition(void) {
	// ...
}

void InterfaceCaller::commandCloseFile(void) {
	// ...
}

void InterfaceCaller::commandCloseInput(void) {
	// ...
}

void InterfaceCaller::commandCls(void) {
	// ...
}

void InterfaceCaller::commandClsColor(void) {
	// ...
}

void InterfaceCaller::commandColor(void) {
	// ...
}

void InterfaceCaller::commandCopyBox(void) {
	// ...
}

void InterfaceCaller::commandCopyFile(void) {
	// ...
}

void InterfaceCaller::commandDecrypt(void) {
	// ...
}

void InterfaceCaller::commandDefaultMask(void) {
	// ...
}

void InterfaceCaller::commandDefaultVisible(void) {
	// ...
}

void InterfaceCaller::commandDeleteFile(void) {
	// ...
}

void InterfaceCaller::commandDeleteFont(void) {
	// ...
}

void InterfaceCaller::commandDeleteImage(void) {
	// ...
}

void InterfaceCaller::commandDeleteMEMBlock(void) {
	// ...
}

void InterfaceCaller::commandDeleteObject(void) {
	// ...
}

void InterfaceCaller::commandDeleteSound(void) {
	// ...
}

void InterfaceCaller::commandDot(void) {
	// ...
}

void InterfaceCaller::commandDrawAnimation(void) {
	// ...
}

void InterfaceCaller::commandDrawGame(void) {
	// ...
}

void InterfaceCaller::commandDrawGhostImage(void) {
	// ...
}

void InterfaceCaller::commandDrawImage(void) {
	// ...
}

void InterfaceCaller::commandDrawImageBox(void) {
	// ...
}

void InterfaceCaller::commandDrawScreen(void) {
	uint32_t state = stack->popValue().toInt();
	uint32_t depth = stack->popValue().toInt();
	uint32_t height = stack->popValue().toInt();
	uint32_t width = stack->popValue().toInt();
	system->drawScreen(width, height, depth, state);
}

void InterfaceCaller::commandDrawToImage(void) {
	// ...
}

void InterfaceCaller::commandDrawToScreen(void) {
	// ...
}

void InterfaceCaller::commandDrawToWorld(void) {
	// ...
}

void InterfaceCaller::commandEditMap(void) {
	// ...
}

void InterfaceCaller::commandEllipse(void) {
	// ...
}

void InterfaceCaller::commandEncrypt(void) {
	// ...
}

void InterfaceCaller::commandEnd(void) {
	// ...
}

void InterfaceCaller::commandEndSearch(void) {
	// ...
}

void InterfaceCaller::commandErrors(void) {
	// ...
}

void InterfaceCaller::commandExecute(void) {
	// ...
}

void InterfaceCaller::commandFrameLimit(void) {
	// ...
}

void InterfaceCaller::commandGhostObject(void) {
	// ...
}

void InterfaceCaller::commandGotoSavedLocation(void) {
	// ...
}

void InterfaceCaller::commandHotSpot(void) {
	// ...
}

void InterfaceCaller::commandInitObjectList(void) {
	// ...
}

void InterfaceCaller::commandInsert(void) {
	// ...
}

void InterfaceCaller::commandLine(void) {
	// ...
}

void InterfaceCaller::commandLoadProgram(void) {
	// ...
}

void InterfaceCaller::commandLocate(void) {
	// ...
}

void InterfaceCaller::commandLock(void) {
	// ...
}

void InterfaceCaller::commandLoopObject(void) {
	// ...
}

void InterfaceCaller::commandMakeDir(void) {
	// ...
}

void InterfaceCaller::commandMakeError(void) {
	// ...
}

void InterfaceCaller::commandMaskImage(void) {
	// ...
}

void InterfaceCaller::commandMaskObject(void) {
	// ...
}

void InterfaceCaller::commandMemCopy(void) {
	// ...
}

void InterfaceCaller::commandMirrorObject(void) {
	// ...
}

void InterfaceCaller::commandMoveCamera(void) {
	// ...
}

void InterfaceCaller::commandMoveObject(void) {
	// ...
}

void InterfaceCaller::commandObjectFloat(void) {
	// ...
}

void InterfaceCaller::commandObjectInteger(void) {
	// ...
}

void InterfaceCaller::commandObjectLife(void) {
	// ...
}

void InterfaceCaller::commandObjectOrder(void) {
	// ...
}

void InterfaceCaller::commandObjectPick(void) {
	// ...
}

void InterfaceCaller::commandObjectPickable(void) {
	// ...
}

void InterfaceCaller::commandObjectRange(void) {
	// ...
}

void InterfaceCaller::commandObjectString(void) {
	// ...
}

void InterfaceCaller::commandPaintObject(void) {
	// ...
}

void InterfaceCaller::commandParticleAnimation(void) {
	// ...
}

void InterfaceCaller::commandParticleEmission(void) {
	// ...
}

void InterfaceCaller::commandParticleMovement(void) {
	// ...
}

void InterfaceCaller::commandPickColor(void) {
	// ...
}

void InterfaceCaller::commandPickImageColor(void) {
	// ...
}

void InterfaceCaller::commandPickImageColor2(void) {
	// ...
}

void InterfaceCaller::commandPixelPick(void) {
	// ...
}

void InterfaceCaller::commandPlayObject(void) {
	// ...
}

void InterfaceCaller::commandPlaySound(void) {
	// ...
}

void InterfaceCaller::commandPointCamera(void) {
	// ...
}

void InterfaceCaller::commandPointObject(void) {
	// ...
}

void InterfaceCaller::commandPokeByte(void) {
	// ...
}

void InterfaceCaller::commandPokeFloat(void) {
	// ...
}

void InterfaceCaller::commandPokeInt(void) {
	// ...
}

void InterfaceCaller::commandPokeShort(void) {
	// ...
}

void InterfaceCaller::commandPositionCamera(void) {
	// ...
}

void InterfaceCaller::commandPositionMouse(void) {
	// ...
}

void InterfaceCaller::commandPositionObject(void) {
	// ...
}

void InterfaceCaller::commandPrint(void) {
	// ...
}

void InterfaceCaller::commandPutPixel(void) {
	// ...
}

void InterfaceCaller::commandPutPixel2(void) {
	// ...
}

/** Math: Randomize() */
void InterfaceCaller::commandRandomize(void) {
	int32_t seed = stack->popValue().getInt();
	system->randomize(seed);
}

void InterfaceCaller::commandReadByte(void) {
	// ...
}

void InterfaceCaller::commandReadFloat(void) {
	// ...
}

void InterfaceCaller::commandReadInt(void) {
	// ...
}

void InterfaceCaller::commandReadLine(void) {
	// ...
}

void InterfaceCaller::commandReadShort(void) {
	// ...
}

void InterfaceCaller::commandReadString(void) {
	// ...
}

void InterfaceCaller::commandResetObjectCollision(void) {
	// ...
}

void InterfaceCaller::commandResizeImage(void) {
	// ...
}

void InterfaceCaller::commandResizeMEMBlock(void) {
	// ...
}

void InterfaceCaller::commandRotateCamera(void) {
	// ...
}

void InterfaceCaller::commandRotateImage(void) {
	// ...
}

void InterfaceCaller::commandRotateObject(void) {
	// ...
}

void InterfaceCaller::commandSAFEEXIT(void) {
	// ...
}

void InterfaceCaller::commandSaveImage(void) {
	// ...
}

void InterfaceCaller::commandSaveProgram(void) {
	// ...
}

void InterfaceCaller::commandScreen(void) {
	// ...
}

void InterfaceCaller::commandScreenGamma(void) {
	// ...
}

void InterfaceCaller::commandScreenPositionObject(void) {
	// ...
}

void InterfaceCaller::commandScreenShot(void) {
	// ...
}

void InterfaceCaller::commandSeekFile(void) {
	// ...
}

void InterfaceCaller::commandSetFont(void) {
	// ...
}

void InterfaceCaller::commandSetMap(void) {
	// ...
}

void InterfaceCaller::commandSetSound(void) {
	// ...
}

void InterfaceCaller::commandSetTile(void) {
	// ...
}

void InterfaceCaller::commandSetupCollision(void) {
	// ...
}

void InterfaceCaller::commandSetWindow(void) {
	// ...
}

void InterfaceCaller::commandShowMouse(void) {
	// ...
}

void InterfaceCaller::commandShowObject(void) {
	// ...
}

void InterfaceCaller::commandSmooth2D(void) {
	// ...
}

void InterfaceCaller::commandStartSearch(void) {
	// ...
}

void InterfaceCaller::commandStopAnimation(void) {
	// ...
}

void InterfaceCaller::commandStopObject(void) {
	// ...
}

void InterfaceCaller::commandStopSound(void) {
	// ...
}

void InterfaceCaller::commandText(void) {
	// ...
}

void InterfaceCaller::commandTranslateCamera(void) {
	// ...
}

void InterfaceCaller::commandTranslateObject(void) {
	// ...
}

void InterfaceCaller::commandTurnCamera(void) {
	// ...
}

void InterfaceCaller::commandTurnObject(void) {
	// ...
}

void InterfaceCaller::commandUnlock(void) {
	// ...
}

void InterfaceCaller::commandUpdateGame(void) {
	// ...
}

void InterfaceCaller::commandVerticalText(void) {
	// ...
}

void InterfaceCaller::commandWait(void) {
	// ...
}

void InterfaceCaller::commandWaitKey(void) {
	// ...
}

void InterfaceCaller::commandWaitMouse(void) {
	// ...
}

void InterfaceCaller::commandWrite(void) {
	// ...
}

void InterfaceCaller::commandWriteByte(void) {
	// ...
}

void InterfaceCaller::commandWriteFloat(void) {
	// ...
}

void InterfaceCaller::commandWriteInt(void) {
	// ...
}

void InterfaceCaller::commandWriteLine(void) {
	// ...
}

void InterfaceCaller::commandWriteShort(void) {
	// ...
}

void InterfaceCaller::commandWriteString(void) {
	// ...
}


/** Math: Abs() */
void InterfaceCaller::functionAbs(void) {
	Any v = stack->popValue();

	if (v.type() == Any::Float) {
		stack->pushValue(system->abs(v.getFloat()));
		return;
	}
	stack->pushValue(system->abs(v.getInt()));
}

/** Math: ACos() */
void InterfaceCaller::functionACos(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->acos(v));
}

void InterfaceCaller::functionAnimationHeight(void) {
	// ...
}

void InterfaceCaller::functionAnimationPlaying(void) {
	// ...
}

void InterfaceCaller::functionAnimationWidth(void) {
	// ...
}

void InterfaceCaller::functionAsc(void) {
	// ...
}

/** Math: ASin() */
void InterfaceCaller::functionASin(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->asin(v));
}

/** Math: ATan() */
void InterfaceCaller::functionATan(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->atan(v));
}

void InterfaceCaller::functionBin(void) {
	// ...
}

/** Math: BoxOverlap() */
void InterfaceCaller::functionBoxOverlap(void) {
	float h2 = stack->popValue().toFloat() * 0.5f;
	float w2 = stack->popValue().toFloat() * 0.5f;
	float y2 = stack->popValue().toFloat();
	float x2 = stack->popValue().toFloat();
	float h1 = stack->popValue().toFloat() * 0.5f;
	float w1 = stack->popValue().toFloat() * 0.5f;
	float y1 = stack->popValue().toFloat();
	float x1 = stack->popValue().toFloat();

	stack->pushValue(system->boxOverlap(x1, y1, w1, h1, x2, y2, w2, h2) ? 1 : 0);
}

void InterfaceCaller::functionCameraAngle(void) {
	// ...
}

void InterfaceCaller::functionCameraX(void) {
	// ...
}

void InterfaceCaller::functionCameraY(void) {
	// ...
}

void InterfaceCaller::functionChr(void) {
	// ...
}

void InterfaceCaller::functionCloneImage(void) {
	// ...
}

void InterfaceCaller::functionCloneObject(void) {
	// ...
}

void InterfaceCaller::functionCollisionAngle(void) {
	// ...
}

void InterfaceCaller::functionCollisionX(void) {
	// ...
}

void InterfaceCaller::functionCollisionY(void) {
	// ...
}

void InterfaceCaller::functionCommandLine(void) {
	// ...
}

/** Math: Cos() */
void InterfaceCaller::functionCos(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->cos(v));
}

void InterfaceCaller::functionCountCollisions(void) {
	// ...
}

void InterfaceCaller::functionCountWords(void) {
	// ...
}

void InterfaceCaller::functionCrc32(void) {
	// ...
}

void InterfaceCaller::functionCurrentDir(void) {
	// ...
}

/** Math: CurveAngle() */
void InterfaceCaller::functionCurveAngle(void) {
	float smoothness = stack->popValue().toFloat();
	float oldA = stack->popValue().toFloat();
	float newA = stack->popValue().toFloat();

	stack->pushValue(system->curveAngle(newA, oldA, smoothness));
}

/** Math: CurveValue() */
void InterfaceCaller::functionCurveValue(void) {
	float smoothness = cb->popValue().toFloat();
	float oldV = cb->popValue().toFloat();
	float newV = cb->popValue().toFloat();

	stack->pushValue(system->curveValue(newV, oldV, smoothness));
}

void InterfaceCaller::functionDate(void) {
	// ...
}

/** Math: Distance() */
void InterfaceCaller::functionDistance(void) {
	float y2 = stack->popValue().toFloat();
	float x2 = stack->popValue().toFloat();
	float y1 = stack->popValue().toFloat();
	float x1 = stack->popValue().toFloat();
	stack->pushValue(system->distance(x1, y1, x2, y2));
}

void InterfaceCaller::functionDistance2(void) {
	// ...
}

void InterfaceCaller::functionDownKey(void) {
	// ...
}

void InterfaceCaller::functionEOF(void) {
	// ...
}

void InterfaceCaller::functionEscapeKey(void) {
	// ...
}

void InterfaceCaller::functionFileExists(void) {
	// ...
}

void InterfaceCaller::functionFileOffset(void) {
	// ...
}

void InterfaceCaller::functionFileSize(void) {
	// ...
}

void InterfaceCaller::functionFindFile(void) {
	// ...
}

void InterfaceCaller::functionFlip(void) {
	// ...
}

/** Math: Float() */
void InterfaceCaller::functionFloat(void) {
	stack->pushValue(stack->popValue().toFloat());
}

void InterfaceCaller::functionFPS(void) {
	// ...
}

/** Math: GetAngle() */
void InterfaceCaller::functionGetAngle(void) {
	float y2 = stack->popValue().toFloat();
	float x2 = stack->popValue().toFloat();
	float y1 = stack->popValue().toFloat();
	float x1 = stack->popValue().toFloat();

	stack->pushValue(system->getAngle(x1, y1, x2, y2));
}

void InterfaceCaller::functionGetAngle2(void) {
	// ...
}

void InterfaceCaller::functionGetCollision(void) {
	// ...
}

void InterfaceCaller::functionGetEXEName(void) {
	// ...
}

void InterfaceCaller::functionGetKey(void) {
	// ...
}

void InterfaceCaller::functionGetMap(void) {
	// ...
}

void InterfaceCaller::functionGetMap2(void) {
	// ...
}

void InterfaceCaller::functionGetMouse(void) {
	// ...
}

void InterfaceCaller::functionGetPixel(void) {
	// ...
}

void InterfaceCaller::functionGetPixel2(void) {
	// ...
}

void InterfaceCaller::functionGetRGB(void) {
	// ...
}

void InterfaceCaller::functionGetWord(void) {
	// ...
}

void InterfaceCaller::functionGFXModeExists(void) {
	// ...
}

void InterfaceCaller::functionHex(void) {
	// ...
}

void InterfaceCaller::functionImage(void) {
	// ...
}

void InterfaceCaller::functionImageHeight(void) {
	// ...
}

void InterfaceCaller::functionImagesCollide(void) {
	// ...
}

void InterfaceCaller::functionImagesOverlap(void) {
	// ...
}

void InterfaceCaller::functionImageWidth(void) {
	// ...
}

void InterfaceCaller::functionInput(void) {
	// ...
}

void InterfaceCaller::functionInStr(void) {
	// ...
}

/** Math: Int() */
void InterfaceCaller::functionInt(void) {
	stack->pushValue(stack->popValue().toInt());
}

void InterfaceCaller::functionIsDirectory(void) {
	// ...
}

void InterfaceCaller::functionKeyDown(void) {
	// ...
}

void InterfaceCaller::functionKeyHit(void) {
	// ...
}

void InterfaceCaller::functionKeyUp(void) {
	// ...
}

void InterfaceCaller::functionLeft(void) {
	// ...
}

void InterfaceCaller::functionLeftKey(void) {
	// ...
}

void InterfaceCaller::functionLen(void) {
	// ...
}

void InterfaceCaller::functionLoadAnimImage(void) {
	// ...
}

void InterfaceCaller::functionLoadAnimObject(void) {
	// ...
}

void InterfaceCaller::functionLoadFont(void) {
	// ...
}

void InterfaceCaller::functionLoadImage(void) {
	// ...
}

void InterfaceCaller::functionLoadMap(void) {
	// ...
}

void InterfaceCaller::functionLoadObject(void) {
	// ...
}

void InterfaceCaller::functionLoadSound(void) {
	// ...
}

/** Math: Log() */
void InterfaceCaller::functionLog(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->log(v));
}

/** Math: Log10() */
void InterfaceCaller::functionLog10(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->log10(v));
}

void InterfaceCaller::functionLower(void) {
	// ...
}

void InterfaceCaller::functionLSet(void) {
	// ...
}

void InterfaceCaller::functionMakeEmitter(void) {
	// ...
}

void InterfaceCaller::functionMakeImage(void) {
	// ...
}

void InterfaceCaller::functionMakeMap(void) {
	// ...
}

void InterfaceCaller::functionMakeMEMBlock(void) {
	// ...
}

void InterfaceCaller::functionMakeObject(void) {
	// ...
}

void InterfaceCaller::functionMakeObjectFloor(void) {
	// ...
}

void InterfaceCaller::functionMapHeight(void) {
	// ...
}

void InterfaceCaller::functionMapWidth(void) {
	// ...
}

/** Math: Max() */
void InterfaceCaller::functionMax(void) {
	Any b = stack->popValue();
	Any a = stack->popValue();
	stack->pushValue((a > b) ? a : b);
}

void InterfaceCaller::functionMEMBlockSize(void) {
	// ...
}

void InterfaceCaller::functionMid(void) {
	// ...
}

/** Math: Min() */
void InterfaceCaller::functionMin(void) {
	Any b = stack->popValue();
	Any a = stack->popValue();
	stack->pushValue((a < b) ? a : b);
}

void InterfaceCaller::functionMouseDown(void) {
	// ...
}

void InterfaceCaller::functionMouseHit(void) {
	// ...
}

void InterfaceCaller::functionMouseMoveX(void) {
	// ...
}

void InterfaceCaller::functionMouseMoveY(void) {
	// ...
}

void InterfaceCaller::functionMouseMoveZ(void) {
	// ...
}

void InterfaceCaller::functionMouseUp(void) {
	// ...
}

void InterfaceCaller::functionMouseWX(void) {
	// ...
}

void InterfaceCaller::functionMouseWY(void) {
	// ...
}

void InterfaceCaller::functionMouseX(void) {
	// ...
}

void InterfaceCaller::functionMouseY(void) {
	// ...
}

void InterfaceCaller::functionMouseZ(void) {
	// ...
}

void InterfaceCaller::functionNextObject(void) {
	// ...
}

void InterfaceCaller::functionObjectAngle(void) {
	// ...
}

void InterfaceCaller::functionObjectFloat(void) {
	// ...
}

void InterfaceCaller::functionObjectFrame(void) {
	// ...
}

void InterfaceCaller::functionObjectInteger(void) {
	// ...
}

void InterfaceCaller::functionObjectLife(void) {
	// ...
}

void InterfaceCaller::functionObjectPlaying(void) {
	// ...
}

void InterfaceCaller::functionObjectSight(void) {
	// ...
}

void InterfaceCaller::functionObjectSizeX(void) {
	// ...
}

void InterfaceCaller::functionObjectSizeY(void) {
	// ...
}

void InterfaceCaller::functionObjectsOverlap(void) {
	// ...
}

void InterfaceCaller::functionObjectString(void) {
	// ...
}

void InterfaceCaller::functionObjectX(void) {
	// ...
}

void InterfaceCaller::functionObjectY(void) {
	// ...
}

void InterfaceCaller::functionOpenToEdit(void) {
	// ...
}

void InterfaceCaller::functionOpenToRead(void) {
	// ...
}

void InterfaceCaller::functionOpenToWrite(void) {
	// ...
}

void InterfaceCaller::functionPeekByte(void) {
	// ...
}

void InterfaceCaller::functionPeekFloat(void) {
	// ...
}

void InterfaceCaller::functionPeekInt(void) {
	// ...
}

void InterfaceCaller::functionPeekShort(void) {
	// ...
}

void InterfaceCaller::functionPickedAngle(void) {
	// ...
}

void InterfaceCaller::functionPickedObject(void) {
	// ...
}

void InterfaceCaller::functionPickedX(void) {
	// ...
}

void InterfaceCaller::functionPickedY(void) {
	// ...
}

void InterfaceCaller::functionPlayAnimation(void) {
	// ...
}

void InterfaceCaller::functionPlaySound(void) {
	// ...
}

/** Math: Rand() */
void InterfaceCaller::functionRand(void) {
	int32_t high = stack->popValue().toInt();
	int32_t low = stack->popValue().toInt();

	stack->pushValue(system->rand(low, high));
}

void InterfaceCaller::functionReadByte(void) {
	// ...
}

void InterfaceCaller::functionReadFloat(void) {
	// ...
}

void InterfaceCaller::functionReadInt(void) {
	// ...
}

void InterfaceCaller::functionReadLine(void) {
	// ...
}

void InterfaceCaller::functionReadShort(void) {
	// ...
}

void InterfaceCaller::functionReadString(void) {
	// ...
}

void InterfaceCaller::functionReplace(void) {
	// ...
}

void InterfaceCaller::functionRight(void) {
	// ...
}

void InterfaceCaller::functionRightKey(void) {
	// ...
}

/** Math: Rnd() */
void InterfaceCaller::functionRnd(void) {
	float high = stack->popValue().toFloat();
	float low = stack->popValue().toFloat();

	stack->pushValue(system->rand(low, high));
}

/** Math: RoundDown() */
void InterfaceCaller::functionRoundDown(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->roundDown(v));
}

/** Math: RoundUp() */
void InterfaceCaller::functionRoundUp(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->roundUp(v));
}

void InterfaceCaller::functionRSet(void) {
	// ...
}

void InterfaceCaller::functionSCREEN(void) {
	// ...
}

void InterfaceCaller::functionScreenDepth(void) {
	// ...
}

void InterfaceCaller::functionScreenHeight(void) {
	// ...
}

void InterfaceCaller::functionScreenWidth(void) {
	// ...
}

/** Math: Sin() */
void InterfaceCaller::functionSin(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->sin(v));
}

void InterfaceCaller::functionSoundPlaying(void) {
	// ...
}

/** Math: Sqrt() */
void InterfaceCaller::functionSqrt(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->sqrt(v));
}

void InterfaceCaller::functionStr(void) {
	// ...
}

void InterfaceCaller::functionString(void) {
	// ...
}

void InterfaceCaller::functionStrInsert(void) {
	// ...
}

void InterfaceCaller::functionStrMove(void) {
	// ...
}

void InterfaceCaller::functionStrRemove(void) {
	// ...
}

/** Math: Tan() */
void InterfaceCaller::functionTan(void) {
	float v = stack->popValue().toFloat();
	stack->pushValue(system->tan(v));
}

void InterfaceCaller::functionTextHeight(void) {
	// ...
}

void InterfaceCaller::functionTextWidth(void) {
	// ...
}

void InterfaceCaller::functionTime(void) {
	// ...
}

void InterfaceCaller::functionTimer(void) {
	// ...
}

void InterfaceCaller::functionTrim(void) {
	// ...
}

void InterfaceCaller::functionUpKey(void) {
	// ...
}

void InterfaceCaller::functionUpper(void) {
	// ...
}

void InterfaceCaller::functionWaitKey(void) {
	// ...
}

void InterfaceCaller::functionWaitMouse(void) {
	// ...
}

/** Math: WrapAngle() */
void InterfaceCaller::functionWrapAngle(void) {
	Any a = stack->popValue();
	if (a.type() == Any::Float) {
		stack->pushValue(system->wrapAngle(a.getFloat()));
	}
	else if (a.type() == Any::Int) {
		stack->pushValue(system->wrapAngle(a.getInt()));
	}
	stack->pushValue(a);
}

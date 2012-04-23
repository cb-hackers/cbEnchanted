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

void InterfaceCaller::commandArrayAssign(void) {
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

void InterfaceCaller::commandCase(void) {
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

void InterfaceCaller::commandClearArray(void) {
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

void InterfaceCaller::commandDelete(void) {
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

void InterfaceCaller::commandDim(void) {
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

void InterfaceCaller::commandFunction(void) {
	// ...
}

void InterfaceCaller::commandGhostObject(void) {
	// ...
}

void InterfaceCaller::commandGosub(void) {
	// ...
}

void InterfaceCaller::commandGoto(void) {
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

void InterfaceCaller::commandRandomize(void) {
	// ...
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

void InterfaceCaller::commandReDim(void) {
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

void InterfaceCaller::commandReturn(void) {
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

void InterfaceCaller::commandSelect(void) {
	// ...
}

void InterfaceCaller::commandSetArrayNumbers(void) {
	// ...
}

void InterfaceCaller::commandSetFont(void) {
	// ...
}

void InterfaceCaller::commandSetGlobalVariable(void) {
	// ...
}

void InterfaceCaller::commandSetGlobalVariableNumbers(void) {
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

void InterfaceCaller::commandSetTypeMemberField(void) {
	// ...
}

void InterfaceCaller::commandSetupCollision(void) {
	// ...
}

void InterfaceCaller::commandSetVariable(void) {
	// ...
}

void InterfaceCaller::commandSetVariableNumbers(void) {
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

void InterfaceCaller::commandType(void) {
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



void InterfaceCaller::functionAbs(void) {
	// ...
}

void InterfaceCaller::functionACos(void) {
	// ...
}

void InterfaceCaller::functionAfter(void) {
	// ...
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

void InterfaceCaller::functionASin(void) {
	// ...
}

void InterfaceCaller::functionATan(void) {
	// ...
}

void InterfaceCaller::functionBefore(void) {
	// ...
}

void InterfaceCaller::functionBin(void) {
	// ...
}

void InterfaceCaller::functionBoxOverlap(void) {
	// ...
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

void InterfaceCaller::functionConvertToInteger(void) {
	// ...
}

void InterfaceCaller::functionConvertToType(void) {
	// ...
}

void InterfaceCaller::functionCos(void) {
	// ...
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

void InterfaceCaller::functionCurveAngle(void) {
	// ...
}

void InterfaceCaller::functionCurveValue(void) {
	// ...
}

void InterfaceCaller::functionDate(void) {
	// ...
}

void InterfaceCaller::functionDistance(void) {
	// ...
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

void InterfaceCaller::functionFirst(void) {
	// ...
}

void InterfaceCaller::functionFlip(void) {
	// ...
}

void InterfaceCaller::functionFloat(void) {
	// ...
}

void InterfaceCaller::functionFPS(void) {
	// ...
}

void InterfaceCaller::functionGetAngle(void) {
	// ...
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

void InterfaceCaller::functionInt(void) {
	// ...
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

void InterfaceCaller::functionLast(void) {
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

void InterfaceCaller::functionLog(void) {
	// ...
}

void InterfaceCaller::functionLog10(void) {
	// ...
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

void InterfaceCaller::functionMax(void) {
	// ...
}

void InterfaceCaller::functionMEMBlockSize(void) {
	// ...
}

void InterfaceCaller::functionMid(void) {
	// ...
}

void InterfaceCaller::functionMin(void) {
	// ...
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

void InterfaceCaller::functionNew(void) {
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

void InterfaceCaller::functionRand(void) {
	// ...
}

void InterfaceCaller::functionRead(void) {
	// ...
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

void InterfaceCaller::functionRnd(void) {
	// ...
}

void InterfaceCaller::functionRoundDown(void) {
	// ...
}

void InterfaceCaller::functionRoundUp(void) {
	// ...
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

void InterfaceCaller::functionSin(void) {
	// ...
}

void InterfaceCaller::functionSoundPlaying(void) {
	// ...
}

void InterfaceCaller::functionSqrt(void) {
	// ...
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

void InterfaceCaller::functionTan(void) {
	// ...
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

void InterfaceCaller::functionWrapAngle(void) {
	// ...
}

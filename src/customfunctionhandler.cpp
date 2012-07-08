#include "customfunctionhandler.h"
#include "cbenchanted.h"
#include "customfunctiondefines.h"
#include "customfunctions.h"
CustomFunctionHandler::CustomFunctionHandler() {

#ifndef DISABLE_CUSTOMS
	importCustomFunctions();
#endif
}

int32_t CustomFunctionHandler::getHandle(CustomFunction &a) {
	for (vector<CustomFunction>::iterator i = functions.begin(); i != functions.end(); i++) {
		if (*i == a) {
			return (*i).getHandle();
		}
	}
	int32_t handle = functions.size();
	a.setHandle(handle);
	functions.push_back(a);
	return handle;
}

void CustomFunctionHandler::import(CustomFunction &a) {
	int32_t handleCounter = 0;

	//Check if already added by getHandle
	for (vector<CustomFunction>::iterator i = functions.begin(); i != functions.end(); i++) {
		if (*i == a) {
			*i = a;
			(*i).setHandle(handleCounter);
		}
		handleCounter++;
	}

	int32_t handle = functions.size();
	a.setHandle(handle);
	functions.push_back(a);
}

void CustomFunctionHandler::call(CBEnchanted *cb, int32_t handle) {
	assert(0 <= handle && size_t(handle) < functions.size());
	functions[handle].call(cb);
}

void CustomFunctionHandler::add(CustomFunction &a) {
	int32_t handle = functions.size();
	a.setHandle(handle);
	functions.push_back(a);
}

/** Adds custom functions to CustomFunctionHandler
  */
void CustomFunctionHandler::importCustomFunctions() {
	CustomFunction func;
	func.setGroupId(CBE_CUSTOM_FUNCTION_GROUP);

	func.setFuncPtr(&cbeTriangle);
	func.setFuncId(CBE_CUSTOM_DRAWTRIANGLE);
	this->add(func);

	func.setFuncPtr(&cbeColor);
	func.setFuncId(CBE_CUSTOM_COLOR);
	this->add(func);

	func.setFuncPtr(&cbeClsColor);
	func.setFuncId(CBE_CUSTOM_CLSCOLOR);
	this->add(func);

	func.setFuncPtr(&cbeSetBlendMode);
	func.setFuncId(CBE_CUSTOM_SET_BLEND_MODE);
	this->add(func);

	func.setFuncPtr(&cbeSetBlendModeAdvanced);
	func.setFuncId(CBE_CUSTOM_SET_BLEND_MODE_ADVANCED);
	this->add(func);

	func.setFuncPtr(&cbeDrawTintedImage);
	func.setFuncId(CBE_DRAW_TINTED_IMAGE);
	this->add(func);

	func.setFuncPtr(&cbeDrawImage);
	func.setFuncId(CBE_DRAW_IMAGE);
	this->add(func);

	func.setFuncPtr(&cbeBNot);
	func.setFuncId(CBE_BINARY_NOT);
	this->add(func);

	func.setFuncPtr(&cbeBAnd);
	func.setFuncId(CBE_BINARY_AND);
	this->add(func);

	func.setFuncPtr(&cbeBOr);
	func.setFuncId(CBE_BINARY_OR);
	this->add(func);

	func.setFuncPtr(&cbeBXor);
	func.setFuncId(CBE_BINARY_XOR);
	this->add(func);

	func.setFuncPtr(&cbeReadLineUTF8);
	func.setFuncId(CBE_READ_LINE_UTF8);
	this->add(func);

	func.setFuncPtr(&cbeShowConsole);
	func.setFuncId(CBE_SHOW_CONSOLE);
	this->add(func);

	func.setFuncPtr(&cbeSetSystemCursor);
	func.setFuncId(CBE_SET_SYSTEM_CURSOR);
	this->add(func);

	func.setFuncPtr(&cbeTransformBuild);
	func.setFuncId(CBE_TRANSFORM_BUILD);
	this->add(func);

	func.setFuncPtr(&cbeTransformRotate);
	func.setFuncId(CBE_TRANSFORM_ROTATE);
	this->add(func);

	func.setFuncPtr(&cbeTransformScale);
	func.setFuncId(CBE_TRANSFORM_SCALE);
	this->add(func);

	func.setFuncPtr(&cbeTransformTranslate);
	func.setFuncId(CBE_TRANSFORM_TRANSLATE);
	this->add(func);

	func.setFuncPtr(&cbeTransformReset);
	func.setFuncId(CBE_TRANSFORM_RESET);
	this->add(func);

	func.setFuncPtr(&cbeCameraAngle);
	func.setFuncId(CBE_CAMERA_ANGLE);
	this->add(func);

	func.setFuncPtr(&cbeCameraZoom);
	func.setFuncId(CBE_CAMERA_ZOOM);
	this->add(func);

	func.setFuncPtr(&cbeSetLineWidth);
	func.setFuncId(CBE_SET_LINE_WIDTH);
	this->add(func);

	func.setFuncPtr(&cbeFileDialog);
	func.setFuncId(CBE_FILEDIALOG);
	this->add(func);

	func.setFuncPtr(&cbeWToSX);
	func.setFuncId(CBE_W_TO_S_X);
	this->add(func);

	func.setFuncPtr(&cbeWToSY);
	func.setFuncId(CBE_W_TO_S_Y);
	this->add(func);

	func.setFuncPtr(&cbeSToWX);
	func.setFuncId(CBE_S_TO_W_X);
	this->add(func);

	func.setFuncPtr(&cbeSToWY);
	func.setFuncId(CBE_S_TO_W_Y);
	this->add(func);

	func.setFuncPtr(&cbeObjectSX);
	func.setFuncId(CBE_OBJECT_S_X);
	this->add(func);

	func.setFuncPtr(&cbeObjectSY);
	func.setFuncId(CBE_OBJECT_S_Y);
	this->add(func);
	
	func.setGroupId(CBE_STACK);
	
	func.setFuncPtr(&cbePushByte);
	func.setFuncId(CBE_PUSH_BYTE);
	this->add(func);
	
	func.setFuncPtr(&cbePushShort);
	func.setFuncId(CBE_PUSH_SHORT);
	this->add(func);
	
	func.setFuncPtr(&cbePushInteger);
	func.setFuncId(CBE_PUSH_INTEGER);
	this->add(func);
	
	func.setFuncPtr(&cbePushFloat);
	func.setFuncId(CBE_PUSH_FLOAT);
	this->add(func);
	
	func.setFuncPtr(&cbePushString);
	func.setFuncId(CBE_PUSH_STRING);
	this->add(func);
	
	func.setFuncPtr(&cbePopByte);
	func.setFuncId(CBE_POP_BYTE);
	this->add(func);
	
	func.setFuncPtr(&cbePopShort);
	func.setFuncId(CBE_POP_SHORT);
	this->add(func);
	
	func.setFuncPtr(&cbePopInteger);
	func.setFuncId(CBE_POP_INTEGER);
	this->add(func);
	
	func.setFuncPtr(&cbePopFloat);
	func.setFuncId(CBE_POP_FLOAT);
	this->add(func);
	
	func.setFuncPtr(&cbePopString);
	func.setFuncId(CBE_POP_STRING);
	this->add(func);
}


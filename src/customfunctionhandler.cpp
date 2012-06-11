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

	func.setFuncPtr(&cbeDrawTintedScaledRotatedImageRegion);
	func.setFuncId(CBE_DRAW_TINTED_SCALED_ROTATED_IMAGE_REGION);
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
}


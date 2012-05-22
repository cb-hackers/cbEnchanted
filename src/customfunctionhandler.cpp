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
}


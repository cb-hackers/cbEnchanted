#include "customfunctionhandler.h"
#include "cbenchanted.h"
#include "customfunctiondefines.h"
#include "customfunctions.h"
#ifndef CBE_LIB
/** Constructs CustomFunctionHandler and adds the custom functions included in the cbE. */
CustomFunctionHandler::CustomFunctionHandler() {

#ifndef DISABLE_CUSTOMS
	importCustomFunctions();
#endif
}

/** Imports the custom functions included in the cbE */
void CustomFunctionHandler::importCustomFunctions() {
	CustomFunction func;
	func.setGroupId(CBE_CUSTOM_FUNCTION_GROUP);

	func.setFuncPtr(&cbeTriangle);
	func.setFuncId(CBE_CUSTOM_DRAWTRIANGLE);
	this->addBody(func);

	func.setFuncPtr(&cbeColor);
	func.setFuncId(CBE_CUSTOM_COLOR);
	this->addBody(func);

	func.setFuncPtr(&cbeClsColor);
	func.setFuncId(CBE_CUSTOM_CLSCOLOR);
	this->addBody(func);

	func.setFuncPtr(&cbeSetBlendMode);
	func.setFuncId(CBE_CUSTOM_SET_BLEND_MODE);
	this->addBody(func);

	func.setFuncPtr(&cbeSetBlendModeAdvanced);
	func.setFuncId(CBE_CUSTOM_SET_BLEND_MODE_ADVANCED);
	this->addBody(func);

	func.setFuncPtr(&cbeMakeImage);
	func.setFuncId(CBE_MAKE_IMAGE);
	this->addBody(func);

	func.setFuncPtr(&cbeDrawImage);
	func.setFuncId(CBE_DRAW_IMAGE);
	this->addBody(func);

	func.setFuncPtr(&cbeDrawTintedImage);
	func.setFuncId(CBE_DRAW_TINTED_IMAGE);
	this->addBody(func);

	func.setFuncPtr(&cbeBNot);
	func.setFuncId(CBE_BINARY_NOT);
	this->addBody(func);

	func.setFuncPtr(&cbeBAnd);
	func.setFuncId(CBE_BINARY_AND);
	this->addBody(func);

	func.setFuncPtr(&cbeBOr);
	func.setFuncId(CBE_BINARY_OR);
	this->addBody(func);

	func.setFuncPtr(&cbeBXor);
	func.setFuncId(CBE_BINARY_XOR);
	this->addBody(func);

	func.setFuncPtr(&cbeReadLineUTF8);
	func.setFuncId(CBE_READ_LINE_UTF8);
	this->addBody(func);

	func.setFuncPtr(&cbeShowConsole);
	func.setFuncId(CBE_SHOW_CONSOLE);
	this->addBody(func);

	func.setFuncPtr(&cbeSetSystemCursor);
	func.setFuncId(CBE_SET_SYSTEM_CURSOR);
	this->addBody(func);

	func.setFuncPtr(&cbeTransformBuild);
	func.setFuncId(CBE_TRANSFORM_BUILD);
	this->addBody(func);

	func.setFuncPtr(&cbeTransformRotate);
	func.setFuncId(CBE_TRANSFORM_ROTATE);
	this->addBody(func);

	func.setFuncPtr(&cbeTransformScale);
	func.setFuncId(CBE_TRANSFORM_SCALE);
	this->addBody(func);

	func.setFuncPtr(&cbeTransformTranslate);
	func.setFuncId(CBE_TRANSFORM_TRANSLATE);
	this->addBody(func);

	func.setFuncPtr(&cbeTransformReset);
	func.setFuncId(CBE_TRANSFORM_RESET);
	this->addBody(func);

	func.setFuncPtr(&cbeCameraAngle);
	func.setFuncId(CBE_CAMERA_ANGLE);
	this->addBody(func);

	func.setFuncPtr(&cbeCameraZoom);
	func.setFuncId(CBE_CAMERA_ZOOM);
	this->addBody(func);

	func.setFuncPtr(&cbeSetLineWidth);
	func.setFuncId(CBE_SET_LINE_WIDTH);
	this->addBody(func);

	func.setFuncPtr(&cbeFileDialog);
	func.setFuncId(CBE_FILEDIALOG);
	this->addBody(func);

	func.setFuncPtr(&cbeWToSX);
	func.setFuncId(CBE_W_TO_S_X);
	this->addBody(func);

	func.setFuncPtr(&cbeWToSY);
	func.setFuncId(CBE_W_TO_S_Y);
	this->addBody(func);

	func.setFuncPtr(&cbeSToWX);
	func.setFuncId(CBE_S_TO_W_X);
	this->addBody(func);

	func.setFuncPtr(&cbeSToWY);
	func.setFuncId(CBE_S_TO_W_Y);
	this->addBody(func);

	func.setFuncPtr(&cbeObjectSX);
	func.setFuncId(CBE_OBJECT_S_X);
	this->addBody(func);

	func.setFuncPtr(&cbeObjectSY);
	func.setFuncId(CBE_OBJECT_S_Y);
	this->addBody(func);

	func.setFuncPtr(&cbeLinkCustomFunctions);
	func.setFuncId(CBE_LINK_CUSTOM_FUNCTIONS);
	this->addBody(func);

	func.setFuncPtr(&cbeLoadLibrary);
	func.setFuncId(CBE_LOAD_LIBRARY);
	this->addBody(func);

	func.setFuncPtr(&cbeGetGfxModesCount);
	func.setFuncId(CBE_GET_GFX_MODES_COUNT);
	this->addBody(func);

	func.setFuncPtr(&cbeGetGfxMode);
	func.setFuncId(CBE_GET_GFX_MODE);
	this->addBody(func);

	func.setFuncPtr(&cbeGetBestGfxMode);
	func.setFuncId(CBE_GET_BEST_GFX_MODE);
	this->addBody(func);

	func.setFuncPtr(&cbeMessageBox);
	func.setFuncId(CBE_MESSAGEBOX);
	this->addBody(func);

	func.setFuncPtr(&cbePutPixel);
	func.setFuncId(CBE_PUT_PIXEL);
	this->addBody(func);

	func.setFuncPtr(&cbeGetPixel);
	func.setFuncId(CBE_GET_PIXEL);
	this->addBody(func);

}
#endif

/** Calls custom function with given handle */
void CustomFunctionHandler::call(CBEnchanted *cb, int32_t handle) {
	assert(0 <= handle && size_t(handle) < functions.size());
	assert(functions[handle].getFuncPtr() && "Function hasn't been linked");
	functions[handle].call(cb);
}

/** Adds a definition of a function. */
void CustomFunctionHandler::addDefinition(CustomFunction &a) {
	int32_t handle = functions.size();
	a.setHandle(handle);
	functions.push_back(a);
}

/** Adds a body of a function */
void CustomFunctionHandler::addBody(const CustomFunction &a) {
	unlinkedBodies.insert(a);
}

/** Links definitions and bodies together. */
void CustomFunctionHandler::link() {
	for (vector<CustomFunction>::iterator i = functions.begin(); i != functions.end(); i++) {
		if (i->getFuncPtr() == 0) {
			set<CustomFunction>::iterator bodyI = unlinkedBodies.find(*i);
			if (bodyI != unlinkedBodies.end()) {
				i->setFuncPtr(bodyI->getFuncPtr());
				unlinkedBodies.erase(bodyI);
			}
		}
	}
}


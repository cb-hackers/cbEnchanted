#include <cbenchanted.h>

void testFunction(CBEnchanted *cb) {
	printf("Hello world");
	cb->pushValue(0);
}

extern "C" __declspec(dllexport) void cbeInitializeLibrary(CBEnchanted *cb) {
	cb->dllInit();
	CustomFunctionHandler *cfh = cb->getCustomFunctionHandler();

	CustomFunction func;
	func.setGroupId(1337);
	func.setFuncId(1);
	func.setFuncPtr(&testFunction);
	cfh->addBody(func);
}



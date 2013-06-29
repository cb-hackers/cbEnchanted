#include "cbdll.h"
#include "util.h"
#include "cbenchanted.h"
#include "errorsystem.h"

CBDLL::CBDLL() {
}

bool CBDLL::load(const ISString &f) {
	dll = loadDLL(f);
	return dll != 0;
}

void CBDLL::call(const string &name, const void *in, int32_t inSize, void *out, int32_t outSize) {
	if (dll == 0) return;
	CBDLLFunction func = functions[name];
	if (func == 0) {
		func = (CBDLLFunction) getDLLFunction(dll, name);
		if (func == 0) {
			CBEnchanted::instance()->errors->createError("Can't call dll function", "Can't call dll function \"" + name + "\"");
			return;
		}
		functions[name] = func;
	}
	func(in, inSize, out, outSize);
}

#include "customfunctionhandler.h"

CustomFunctionHandler::CustomFunctionHandler()
{
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

void CustomFunctionHandler::add(CustomFunction &a) {
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

void CustomFunctionHandler::call(int32_t handle) {
	assert(0 <= handle && handle < functions.size());
	functions[handle].call();
}

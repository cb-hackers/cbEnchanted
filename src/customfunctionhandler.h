#ifndef CUSTOMFUNCTIONHANDLER_H
#define CUSTOMFUNCTIONHANDLER_H
#include "customfunction.h"
class CustomFunctionHandler
{
	public:
		CustomFunctionHandler();
		void add(CustomFunction &a);
		int32_t getHandle(CustomFunction &a);
		void call(int32_t handle);
	private:
		vector<CustomFunction> functions;
};

#endif // CUSTOMFUNCTIONHANDLER_H

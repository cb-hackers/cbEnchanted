#ifndef CUSTOMFUNCTIONHANDLER_H
#define CUSTOMFUNCTIONHANDLER_H
#include "customfunction.h"
class CustomFunctionHandler
{
	public:
		CustomFunctionHandler();
		void import(CustomFunction &a);
		int32_t getHandle(CustomFunction &a);
		void call(CBEnchanted *cb,int32_t handle);
	private:
		vector<CustomFunction> functions;
		void importCustomFunctions();
		void add(CustomFunction &a);
};

#endif // CUSTOMFUNCTIONHANDLER_H

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
		void add(CustomFunction &a);
#ifndef CBE_LIB
		void importCustomFunctions();
#endif
};

#endif // CUSTOMFUNCTIONHANDLER_H

#ifndef CUSTOMFUNCTIONHANDLER_H
#define CUSTOMFUNCTIONHANDLER_H
#include "customfunction.h"
class CustomFunctionHandler {
	public:
		/** Constructs CustomFunctionHandler and adds the custom functions included in the cbE. */
		CustomFunctionHandler();

		/** Calls custom function with given handle */
		void call(CBEnchanted *cb,int32_t handle);

		/** Adds a definition of a function. */
		void addDefinition(CustomFunction &a);

		/** Adds a body of a function */
		void addBody(const CustomFunction &a);

		/** Links definitions and bodies together. */
		void link();
	private:
#ifndef CBE_LIB
		/** Imports the custom functions included in the cbE */
		void importCustomFunctions();
#endif
		/** Definitions of functions */
		vector<CustomFunction> functions;

		/** Bodies that haven't been linked yet */
		set<CustomFunction> unlinkedBodies;
};

#endif // CUSTOMFUNCTIONHANDLER_H

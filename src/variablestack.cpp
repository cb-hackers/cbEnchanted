#include "variablestack.h"
#include "any.h"
void VariableStack::resize(uint32_t newStackSize) {
	Any *newStack = new Any[newStackSize];
	memcpy(newStack, stackArray, sizeof(Any) * stackSize);
	memset((newStack + stackSize), 0, (newStackSize - stackSize) * sizeof(Any));
	delete [] stackArray;
	stackArray = newStack;
	stackSize = newStackSize;
	INFO("Stack resized to %i", stackSize);
#ifdef CBE_STACKDUMP
	dump();
#endif
}

void VariableStack::dump() {
	printf("\nSTACKDUMP\n------------------------------------\n");
	for (unsigned int i = 0; i < stackLevel; ++i) {
		printf("Stack[%u] ", i);
		stackArray[i].dump();
	}
	printf("------------------------------------\n");
}

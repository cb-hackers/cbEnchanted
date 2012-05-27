#ifndef VARIABLESTACK_H
#define VARIABLESTACK_H
#include "any.h"

class VariableStack {
		/** @brief
		 * To gain speed, mathematical expressions need to have a direct
		 * link to stack variables. Making Any friend with VariableHolder
		 * does exactly that.
		 * @see mathoperations.h */
		friend class Any;
	public:
		VariableStack() {
			stackSize = 100;
			stackArray = new Any[stackSize];
			memset(stackArray, 0, sizeof(Any) * stackSize);
			stackLevel = 0;
		}

		FORCEINLINE void push(const Any &a) {
			if (stackSize <= stackLevel) {
				resize(stackSize * 2);
			}
			stackArray[stackLevel] = a;
			++stackLevel;
		}

		inline const Any &pop() {
			return stackArray[--stackLevel];
		}

		inline bool empty() const {
			return stackLevel == 0;
		}

		void resize(uint32_t newStackSize);

	private:
		Any *stackArray;
		uint32_t stackLevel;
		uint32_t stackSize;
};

#endif

#ifndef VARIABLESTACK_H
#define VARIABLESTACK_H

class VariableStack {
	public:
		VariableStack() {
			stackSize = 100;
			stackArray = new Any[stackSize];
			memset(stackArray, 0, sizeof(Any) * stackSize);
			stackLevel = 0;
		}

		inline void push(const Any &a) {
			if (stackSize < stackLevel) {
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

		void resize(uint32_t newStackSize) {
			Any *newStack = new Any[newStackSize];
			memcpy(newStack,stackArray,sizeof(Any)*stackSize);
			memset(newStack+stackSize,0,(newStackSize-stackSize)*sizeof(Any));
			delete stackArray;
			stackArray = newStack;
			stackSize = newStackSize;
			INFO("Stack resized to %i",stackSize);
		}

	private:
		Any *stackArray;
		uint32_t stackLevel;
		uint32_t stackSize;
};

#endif

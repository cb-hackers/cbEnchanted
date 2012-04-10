#ifndef CBVARIABLEHOLDER_H
#define CBVARIABLEHOLDER_H

#include "precomp.h"
#include "any.h"
#include "scopeholder.h"
#include "array.h"
#include "type.h"
#include "variablecollection.h"
#include "variablestack.h"

class CBVariableHolder {
	public:
		CBVariableHolder() {}

		const Any & popValue(void) {
			assert(!internalStack.empty());
			const Any &value = internalStack.pop();
			assert(!value.empty());
			return value;
		}

		FORCEINLINE void pushValue(const Any &value) {
			internalStack.push(value);
		}

		void pushScope(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount,int32_t typePtrCount) {
			scopeHolder.pushScope(byteCount,shortCount,stringCount,floatCount,integerCount,typePtrCount);
		}

		void popScope(void) {
			scopeHolder.popScope();
		}

		bool inFunction(void) {
			return scopeHolder.depth() > 1;
		}

		uint8_t getByteVariable(uint32_t id) { return scopeHolder.getByteVar(id); }
		uint16_t getShortVariable(uint32_t id) { return scopeHolder.getShortVar(id); }
		int32_t getIntegerVariable(uint32_t id) { return scopeHolder.getIntVar(id); }
		float getFloatVariable(uint32_t id) { return scopeHolder.getFloatVar(id); }
		const ISString &getStringVariable(uint32_t id) { return scopeHolder.getStringVar(id); }
		void *getTypePointerVariable(uint32_t id) { return scopeHolder.getTypePtrVar(id);}

		uint8_t &getGlobalByteVariable(uint32_t id) { return globalByteVariables.get(id); }
		uint16_t &getGlobalShortVariable(uint32_t id) { return globalShortVariables.get(id); }
		int32_t &getGlobalIntegerVariable(uint32_t id) { return globalIntegerVariables.get(id); }
		float &getGlobalFloatVariable(uint32_t id) { return globalFloatVariables.get(id); }
		ISString &getGlobalStringVariable(uint32_t id) { return globalStringVariables.get(id); }

		Array<int32_t> &getIntegerArray(uint32_t id) { return integerArrays.get(id); }
		Array<float> &getFloatArray(uint32_t id) { return floatArrays.get(id); }
		Array<uint16_t> &getShortArray(uint32_t id) { return shortArrays.get(id); }
		Array<uint8_t> &getByteArray(uint32_t id) { return byteArrays.get(id); }
		Array<ISString> &getStringArray(uint32_t id) { return stringArrays.get(id); }

		ISString &getString(uint32_t id) { return strings.get(id); }

		void setByteVariable(uint32_t id, uint8_t value) { scopeHolder.setByteVar(id, value); }
		void setShortVariable(uint32_t id, uint16_t value) { scopeHolder.setShortVar(id, value); }
		void setIntegerVariable(uint32_t id, int32_t value) { scopeHolder.setIntVar(id, value); }
		void setFloatVariable(uint32_t id, float value) { scopeHolder.setFloatVar(id, value); }
		void setStringVariable(uint32_t id, const ISString &value) { scopeHolder.setStringVar(id, value); }
		void setTypePointerVariable(uint32_t id,void *value) { scopeHolder.setTypePtrVar(id,value);}

		void setGlobalByteVariable(uint32_t id, uint8_t value) { globalByteVariables.set(id, value); }
		void setGlobalShortVariable(uint32_t id, uint16_t value) { globalShortVariables.set(id, value); }
		void setGlobalIntegerVariable(uint32_t id, int32_t value) { globalIntegerVariables.set(id, value); }
		void setGlobalFloatVariable(uint32_t id, float value) { globalFloatVariables.set(id, value); }
		void setGlobalStringVariable(uint32_t id, const ISString &value) { globalStringVariables.set(id, value); }

		void setArray(uint32_t id, const Array<int32_t> &value) { integerArrays.set(id, value); }
		void setArray(uint32_t id, const Array<float> &value) { floatArrays.set(id, value); }
		void setArray(uint32_t id, const Array<uint16_t> &value) { shortArrays.set(id, value); }
		void setArray(uint32_t id, const Array<uint8_t> &value) { byteArrays.set(id, value); }
		void setArray(uint32_t id, const Array<ISString> &value) { stringArrays.set(id, value); }

		void setString(uint32_t id, ISString value) { strings.set(id, value); }

		void addType(int32_t dataSize) {types.push_back(new Type(dataSize));}

		void initStrings(uint32_t n) { strings.init(n);}
		void initArrays(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount) {
			byteArrays.init(byteCount);
			shortArrays.init(shortCount);
			stringArrays.init(stringCount);
			floatArrays.init(floatCount);
			integerArrays.init(integerCount);
		}

		void initGlobalVars(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount) {
			globalByteVariables.init(byteCount);
			globalShortVariables.init(shortCount);
			globalIntegerVariables.init(integerCount);
			globalFloatVariables.init(floatCount);
			globalStringVariables.init(stringCount);
		}
		Type *getType(int32_t id){return types[--id];}

	protected:
		VariableStack internalStack;
	private:
		ScopeHolder scopeHolder;

		VariableCollection <uint8_t> globalByteVariables;
		VariableCollection <uint16_t> globalShortVariables;
		VariableCollection <int32_t> globalIntegerVariables;
		VariableCollection <float> globalFloatVariables;
		VariableCollection <ISString> globalStringVariables;

		VariableCollection <Array<int32_t> > integerArrays;
		VariableCollection <Array<float> > floatArrays;
		VariableCollection <Array<uint16_t> > shortArrays;
		VariableCollection <Array<uint8_t> > byteArrays;
		VariableCollection <Array<ISString> > stringArrays;

		VariableCollection <ISString> strings;

		std::vector<Type*> types;
};

#endif

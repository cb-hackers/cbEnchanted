#ifndef CBVARIABLEHOLDER_H
#define CBVARIABLEHOLDER_H

#include "precomp.h"
#include "any.h"


class VariableStack {
	public:
		VariableStack() {
			stackSize = 100;
			stackArray = new Any[stackSize];
			memset(stackArray,0,sizeof(Any)*stackSize);
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

		inline bool empty()const{ return stackLevel == 0; }

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

template <class T>
class VariableCollection {
	public:
		VariableCollection():size(0),variables(0) {}
		~VariableCollection() {
			if (variables) delete [] variables;
		}
		VariableCollection(const VariableCollection<T> &vc) {
			this->size = vc.size;
			if (size == 0) {this->variables = 0;return;}
			this->variables = new T[size];
			memcpy(this->variables,vc.variables,this->size*sizeof(T));
		}
		VariableCollection &operator =(const VariableCollection<T> &o) {
			if (this->variables) delete[] this->variables;
			this->size = o.size;
			if (size == 0) {this->variables = 0;return;}
			this->variables = new T[size];
			memcpy(this->variables,vc.variables,this->size*sizeof(T));
		}

		void init(uint32_t s) {
			if (variables) {delete [] variables; variables = 0;}
			size = s;
			if (size == 0) return;
			variables = new T[size];
			memset(variables,0,sizeof(T)*size);
		}

		T &get(uint32_t id) {
			--id;
			assert(id < size);
			return variables[id];
		}

		inline void set(uint32_t id, const T &value) {
			--id;
			assert(id < size);
			variables[id] = value;
		}
	private:
		T *variables;
		uint32_t size;
};

class Type {
	public:
		Type(int32_t fields):sizeOfMember((fields+3)*sizeof(void*)),firstMember(0),lastMember(0) {dummyFirst[0] = dummyFirst[1] = 0;}
		inline void *newMember() {
			void * m = new char[sizeOfMember];
			memset(m,0,sizeOfMember);
			if (lastMember) setAfter(lastMember,m);
			setBefore(m,lastMember);
			setAfter(m,0);
			setType(m,this);
			lastMember = m;
			if (firstMember == 0) {firstMember = m;setAfter(dummyFirst,firstMember);}
			return m;
		}
		inline void *deleteMember(void *m) {
			void * before = getBefore(m);
			void * after = getAfter(m);
			if (before) { //Not first member
				setAfter(before,after);
				if (after == 0) {
					lastMember = before;
				}
				else {
					setBefore(after,before);
				}
				return before;
			}
			else {
				firstMember = after;
				setAfter(dummyFirst,firstMember);
				if (after) {
					setBefore(after,0);
				}
				else {
					lastMember = 0;
				}
				return dummyFirst;
			}
			delete[] (void**)m;
		}
		inline static Type *getMembersType(const void * m){return ((Type**)m)[2];}
		inline static int32_t &getIntField(const void *m,int32_t field) {
			return *(int32_t*)(((void**)m)+3+field);
		}
		inline static float getFloatField(const void *m,int32_t field) {
			return *(float*)(((void**)m)+3+field);
		}
		inline static ISString &getStringField(const void *m,int32_t field) {
			return *(ISString*)(((void**)m)+3+field);
		}
		inline static void setField(void *m,int32_t field,int32_t value) {
			*(int32_t*)(((void**)m)+3+field) = value;
		}
		inline static void setField(void *m,int32_t field,float value) {
			*(float*)(((void**)m)+3+field) = value;
		}
		inline static void setField(void *m,int32_t field,const ISString &value) {
			*(ISString*)(((void**)m)+3+field) = value;
		}
		inline static void *getBefore(void *m) {
			if (m == 0) return 0;
			return ((void**)m)[0];
		}
		inline static void *getAfter(void *m) {
			if (m == 0) return 0;
			return ((void**)m)[1];
		}
		inline void setBefore(void *m,void *v) {
			((void**)m)[0] = v;
		}
		inline void setAfter(void *m,void *v) {
			((void**)m)[1] = v;
		}

		void *getFirst(){return firstMember;}
		void *getLast(){return lastMember;}
	private:
		inline static void setType(void *m,Type *t) {
			((Type**)m)[2] = t;
		}
		void *dummyFirst[2];
		int32_t sizeOfMember;
		void *firstMember;
		void *lastMember;
};

class VariableScope {
	public:
		VariableScope(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount,int32_t typePtrCount) {
			byteVariables.init(byteCount);
			shortVariables.init(shortCount);
			integerVariables.init(integerCount);
			floatVariables.init(floatCount);
			stringVariables.init(stringCount);
			typePointerVariables.init(typePtrCount);
		}

		VariableCollection <uint8_t> byteVariables;
		VariableCollection <uint16_t> shortVariables;
		VariableCollection <int32_t> integerVariables;
		VariableCollection <float> floatVariables;
		VariableCollection <ISString> stringVariables;
		VariableCollection <void*> typePointerVariables;
};
template <class T>
class Array {
	public:
		Array() : data(0){}

		void init(uint32_t s) {
			if (data) delete [] data;
			size = s;
			data = new T[size];
			memset(data,0,size*sizeof(T));
		}
		T &get(uint32_t index)const{
			return data[index];
		}
		void set(uint32_t index,const T &t){
			assert(index < size);
			data[index] = t;
		}
		uint32_t dimensions[5];
		T *data;
		uint32_t size;
};

class CBVariableHolder {
	public:
		CBVariableHolder() {}

		const Any & popValue(void) {
			assert(!internalStack.empty());
			const Any &value = internalStack.pop();
			assert(!value.empty());
			return value;
		}

		void pushValue(const Any &value) {
			internalStack.push(value);
		}

		void pushScope(int32_t byteCount,int32_t shortCount,int32_t stringCount,int32_t floatCount,int32_t integerCount,int32_t typePtrCount) {
			VariableScope s(byteCount,shortCount,stringCount,floatCount,integerCount,typePtrCount);
			scopes.push(s);
		}

		void popScope(void) {
			assert(!scopes.empty());

			scopes.pop();
		}

		bool inFunction(void) {
			return scopes.size() > 1;
		}

		uint8_t getByteVariable(uint32_t id) { return scopes.top().byteVariables.get(id); }
		uint16_t getShortVariable(uint32_t id) { return scopes.top().shortVariables.get(id); }
		int32_t getIntegerVariable(uint32_t id) { return scopes.top().integerVariables.get(id); }
		float getFloatVariable(uint32_t id) { return scopes.top().floatVariables.get(id); }
		ISString getStringVariable(uint32_t id) { return scopes.top().stringVariables.get(id); }
		void *getTypePointerVariable(uint32_t id) { return scopes.top().typePointerVariables.get(id);}

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

		void setByteVariable(uint32_t id, uint8_t value) { scopes.top().byteVariables.set(id, value); }
		void setShortVariable(uint32_t id, uint16_t value) { scopes.top().shortVariables.set(id, value); }
		void setIntegerVariable(uint32_t id, int32_t value) { scopes.top().integerVariables.set(id, value); }
		void setFloatVariable(uint32_t id, float value) { scopes.top().floatVariables.set(id, value); }
		void setStringVariable(uint32_t id, const ISString &value) { scopes.top().stringVariables.set(id, value); }
		void setTypePointerVariable(uint32_t id,void *value) { scopes.top().typePointerVariables.set(id,value);}

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

		void addType(int32_t fields) {types.push_back(new Type(fields));}

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

	private:
		VariableStack internalStack;
		stack <VariableScope> scopes;

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

		//vector <sf::Texture> images;
};

#endif

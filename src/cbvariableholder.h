#ifndef CBVARIABLEHOLDER_H
#define CBVARIABLEHOLDER_H

#include "precomp.h"
#include "any.h"

template <class T>
class VariableCollection {
	public:
		void Init(uint32_t size) {
			variables.reserve(size);
		}

		T &get(uint32_t id) {
			if (variables.size() <= id) {
				variables.resize(id + 1);
			}
	
			return variables[id];
		}

		inline void set(uint32_t id, const T &value) {
			if (variables.size() <= id) {
				variables.resize(id + 1);
			}
			
			variables[id] = value;
		}
	private:
		vector <T> variables;
};

class VariableScope {
	public:
		VariableCollection <uint8_t> byteVariables;
		VariableCollection <uint16_t> shortVariables;
		VariableCollection <int32_t> integerVariables;
		VariableCollection <float> floatVariables;
		VariableCollection <ISString> stringVariables;
};

struct Array {
	void init(uint32_t size, int32_t t) {
		switch (t){
			case 3:
				type = 3;
				data = (char*)new int32_t[size];
				memset(data,0,sizeof(int32_t)*size);
				break;
			case 7:
				type = 7;
				data = (char*)new uint16_t[size];
				memset(data,0,sizeof(uint16_t)*size);
				break;
			case 8:
				type = 8;
				data = (char*)new uint8_t[size];
				memset(data,0,sizeof(uint8_t)*size);
				break;
			case 4: {
				type = 4;
				float *fdata = new float[size];
				for (int32_t i = 0; i != size;++i) {
					fdata[i] = 0.0f;
				}
				data = (char*)fdata;
				break;}
			case 6:{
				type = 6;
				data = (char*)new ISString[size];
				memset(data,0,sizeof(ISString)*size);
				break;
			}
		}
	}

	map<uint32_t, uint32_t> dimensions;
	uint32_t type;
	int32_t getInt(uint32_t index)const{return ((int32_t*)data)[index];}
	float getFloat(uint32_t index)const{return ((float*)data)[index];}
	uint16_t getShort(uint32_t index)const{return ((uint16_t*)data)[index];}
	uint8_t getByte(uint32_t index)const{return ((uint8_t*)data)[index];}
	ISString getString(uint32_t index)const{return ((ISString*)data)[index];}
	void setInt(uint32_t index,int32_t v){((int32_t*)data)[index] = v;}
	void setFloat(uint32_t index,float v){((float*)data)[index] = v;}
	void setShort(uint32_t index,uint16_t v){((uint16_t*)data)[index] = v;}
	void setByte(uint32_t index,uint8_t v){((uint8_t*)data)[index] = v;}
	void setString(uint32_t index,const ISString &v){((ISString*)data)[index] = v;}

	char *data;
};

class CBVariableHolder {
	public:
		CBVariableHolder() { pushScope(); }
	
		Any popValue(void) {
			assert(!internalStack.empty());
			Any value = internalStack.top();
			internalStack.pop();
			
			assert(!value.empty());
			return value;
		}
		
		void pushValue(const Any &value) {
			internalStack.push(value);
		}

		void pushScope(void) {
			VariableScope s;
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
		
		uint8_t getGlobalByteVariable(uint32_t id) { return globalByteVariables.get(id); }
		uint16_t getGlobalShortVariable(uint32_t id) { return globalShortVariables.get(id); }
		int32_t getGlobalIntegerVariable(uint32_t id) { return globalIntegerVariables.get(id); }
		float getGlobalFloatVariable(uint32_t id) { return globalFloatVariables.get(id); }
		ISString getGlobalStringVariable(uint32_t id) { return globalStringVariables.get(id); }
		
		Array getArray(uint32_t id) { return arrays.get(id); }
		
		ISString getString(uint32_t id) { return strings.get(id); }
		
		void setByteVariable(uint32_t id, uint8_t value) { scopes.top().byteVariables.set(id, value); }
		void setShortVariable(uint32_t id, uint16_t value) { scopes.top().shortVariables.set(id, value); }
		void setIntegerVariable(uint32_t id, int32_t value) { scopes.top().integerVariables.set(id, value); }
		void setFloatVariable(uint32_t id, float value) { scopes.top().floatVariables.set(id, value); }
		void setStringVariable(uint32_t id, const ISString &value) { scopes.top().stringVariables.set(id, value); }
		
		void setGlobalByteVariable(uint32_t id, uint8_t value) { globalByteVariables.set(id, value); }
		void setGlobalShortVariable(uint32_t id, uint16_t value) { globalShortVariables.set(id, value); }
		void setGlobalIntegerVariable(uint32_t id, int32_t value) { globalIntegerVariables.set(id, value); }
		void setGlobalFloatVariable(uint32_t id, float value) { globalFloatVariables.set(id, value); }
		void setGlobalStringVariable(uint32_t id, const ISString &value) { globalStringVariables.set(id, value); }

		void setArray(uint32_t id, Array value) { arrays.set(id, value); }
		
		void setString(uint32_t id, string value) { strings.set(id, value); }
	private:
		stack <Any> internalStack;
		stack <VariableScope> scopes;

		/*VariableCollection <uint8_t> byteVariables;
		VariableCollection <uint16_t> shortVariables;
		VariableCollection <int32_t> integerVariables;
		VariableCollection <float> floatVariables;
		VariableCollection <string> stringVariables;*/
		
		VariableCollection <uint8_t> globalByteVariables;
		VariableCollection <uint16_t> globalShortVariables;
		VariableCollection <int32_t> globalIntegerVariables;
		VariableCollection <float> globalFloatVariables;
		VariableCollection <ISString> globalStringVariables;
		
		VariableCollection <Array> arrays;
		
		VariableCollection <ISString> strings;
		
		//vector <sf::Texture> images;
};

#endif

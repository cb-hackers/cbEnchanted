#ifndef CBVARIABLEHOLDER_H
#define CBVARIABLEHOLDER_H

template <class T>
class VariableCollection {
	public:
		T get(uint32_t id) {
			if (variables.size() <= id) {
				variables.resize(id + 1);
			}
	
			return variables[id];
		}
		void set(uint32_t id, T value) {
			if (variables.size() <= id) {
				variables.resize(id + 1);
			}
			
			variables[id] = value;
		}
	private:
		vector <T> variables;
};

struct Array {
	map<uint32_t, uint32_t> dimensions;
	uint32_t type;
	
	boost::any *data;
};

class CBVariableHolder {
	public:
		CBVariableHolder() {}
		
		boost::any popValue(void) {
			assert(!internalStack.empty());
			boost::any value = internalStack.top();
			internalStack.pop();
			
			assert(!value.empty());
			return value;
		}
		
		void pushValue(boost::any value) {
			internalStack.push(value);
		}
		
		template <typename T>
		inline T popValue(void) {
			assert(!internalStack.empty());
			boost::any value;
			try {
				value = internalStack.top();
				internalStack.pop();
				assert(!value.empty());
				return any_cast<T>(value);
			}
			catch(const boost::bad_any_cast &error) {
				cout << "Failed cast from " << value.type().name() << " to " << typeid(T).name() << endl;
				exit(1);
			}
		}

		uint8_t getByteVariable(uint32_t id) { return byteVariables.get(id); }
		uint16_t getShortVariable(uint32_t id) { return shortVariables.get(id); }
		int32_t getIntegerVariable(uint32_t id) { return integerVariables.get(id); }
		float getFloatVariable(uint32_t id) { return floatVariables.get(id); }
		string getStringVariable(uint32_t id) { return stringVariables.get(id); }
		
		uint8_t getGlobalByteVariable(uint32_t id) { return globalByteVariables.get(id); }
		uint16_t getGlobalShortVariable(uint32_t id) { return globalShortVariables.get(id); }
		int32_t getGlobalIntegerVariable(uint32_t id) { return globalIntegerVariables.get(id); }
		float getGlobalFloatVariable(uint32_t id) { return globalFloatVariables.get(id); }
		string getGlobalStringVariable(uint32_t id) { return globalStringVariables.get(id); }
		
		Array getArray(uint32_t id) { return arrays.get(id); }
		
		string getString(uint32_t id) { return strings.get(id); }
		
		
		void setByteVariable(uint32_t id, uint8_t value) { byteVariables.set(id, value); }
		void setShortVariable(uint32_t id, uint16_t value) { shortVariables.set(id, value); }
		void setIntegerVariable(uint32_t id, int32_t value) { integerVariables.set(id, value); }
		void setFloatVariable(uint32_t id, float value) { floatVariables.set(id, value); }
		void setStringVariable(uint32_t id, string value) { stringVariables.set(id, value); }
		
		void setGlobalByteVariable(uint32_t id, uint8_t value) { globalByteVariables.set(id, value); }
		void setGlobalShortVariable(uint32_t id, uint16_t value) { globalShortVariables.set(id, value); }
		void setGlobalIntegerVariable(uint32_t id, int32_t value) { globalIntegerVariables.set(id, value); }
		void setGlobalFloatVariable(uint32_t id, float value) { globalFloatVariables.set(id, value); }
		
		void setArray(uint32_t id, Array value) { arrays.set(id, value); }
		
		void setString(uint32_t id, string value) { strings.set(id, value); }
		
	private:
		stack <boost::any> internalStack;
		
		VariableCollection <uint8_t> byteVariables;
		VariableCollection <uint16_t> shortVariables;
		VariableCollection <int32_t> integerVariables;
		VariableCollection <float> floatVariables;
		VariableCollection <string> stringVariables;
		
		VariableCollection <uint8_t> globalByteVariables;
		VariableCollection <uint16_t> globalShortVariables;
		VariableCollection <int32_t> globalIntegerVariables;
		VariableCollection <float> globalFloatVariables;
		VariableCollection <string> globalStringVariables;
		
		VariableCollection <Array> arrays;
		
		VariableCollection <string> strings;
		
		vector <sf::Texture> images;
};

#endif

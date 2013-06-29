#ifndef VARIABLECOLLECTION_H
#define VARIABLECOLLECTION_H

template <class T>
class VariableCollection {
	public:
		VariableCollection() : size(0), variables(0) { }

		~VariableCollection() {
			if (variables) delete [] variables;
		}

		VariableCollection(const VariableCollection<T> &vc) {
			this->size = vc.size;
			if (size == 0) {
				this->variables = 0;
				return;
			}
			this->variables = new T[size];
			memcpy(this->variables, vc.variables, this->size * sizeof(T));
		}

		VariableCollection &operator =(const VariableCollection<T> &o) {
			if (this->variables) {
				delete[] this->variables;
			}
			this->size = o.size;

			if (size == 0) {
				this->variables = 0;
				return;
			}

			this->variables = new T[size];
			memcpy(this->variables, o.variables, this->size * sizeof(T));
		}

		void init(uint32_t s) {
			if (variables) {
				delete [] variables;
				variables = 0;
			}
			size = s;
			if (size == 0) {
				return;
			}
			variables = new T[size];
			memset(variables, 0, sizeof(T) * size);
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

#endif

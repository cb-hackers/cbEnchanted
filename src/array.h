#ifndef ARRAY_H
#define ARRAY_H

template <class T>
class Array {
	public:
		Array() : data(0) {}

		void init(uint32_t s) {
			if (data) {
				delete [] data;
			}

			size = s;
			data = new T[size];
			memset(data, 0, size * sizeof(T));
		}

		T &get(uint32_t index)const{
			return data[index];
		}

		void set(uint32_t index, const T &t){
			assert(index < size);
			data[index] = t;
		}

		void setDimensions(uint32_t dim, uint32_t size) { this->dimensions[dim] = size; }
		uint32_t getDimensions(uint32_t dim) const { return this->dimensions[dim]; }
	private:
		uint32_t dimensions[5];
		T *data;
		uint32_t size;
};

#endif

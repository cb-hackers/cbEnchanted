#ifndef ARRAY_H
#define ARRAY_H
#include <stdint.h>
#include <cstring>
#include "debug.h"
class ISString;
template <class T>
class Array {
	public:
		Array() : data(0) {}
		void init(uint32_t *dims, uint32_t dimCount);
		void resize(uint32_t *dims, uint32_t dimCount, bool copy);
		T &get(uint32_t index)const { return data[index]; }
		void set(uint32_t index, const T &t){ assert(index < size); data[index] = t;}
		uint32_t getDimensionMultiplier(uint32_t dim) const { return this->dimensions[dim]; }
	private:
		static int32_t getCellIndex(uint32_t *sizes, uint32_t *indices, uint32_t dimCount);
		uint32_t dimensions[5];
		uint32_t dimensionSizes[5];
		T *data;
		uint32_t size;
};

template <class T>
void Array<T>::init(uint32_t *dims, uint32_t dimCount) {
	uint32_t s(1);
	for (int i = dimCount - 1; i >= 0; --i) {
		dimensions[i] = s;
		s *= dims[i];
		dimensionSizes[i] = dims[i];
	}
	size = s;
	data = new T[size];
	memset(data, 0, size * sizeof(T));
}

template <class T>
int32_t Array<T>::getCellIndex(uint32_t *sizes, uint32_t *indices, uint32_t dimCount) {
	uint32_t s(1);
	uint32_t index(0);
	for (int i = dimCount - 1; i >= 0; --i) {
		s *= sizes[i];
		index += s * indices[i];
	}
	return index;
}

template <class T>
void Array<T>::resize(uint32_t *dims, uint32_t dimCount, bool copy) {
	if (copy) {
		//TODO: Use memcpy.
		uint32_t s(1);
		for (int i = dimCount - 1; i >= 0; --i) {
			s *= dims[i];
		}
		s;
		T *newData = new T[s];
		memset(newData, 0, s * sizeof(T));
		uint32_t copySize[5] = {0};
		for (uint32_t i = 0; i != dimCount; i++) {
			copySize[i] = dims[i] < dimensionSizes[i] ? dims[i] : dimensionSizes[i];
		}

		{
			uint32_t tempDims[5];
			tempDims[4] = 0;
			uint32_t i1 = 0;
			do {
				tempDims[0] = i1;
				uint32_t i2 = 0;
				do {
					tempDims[1] = i2;
					uint32_t i3 = 0;
					do {
						tempDims[2] = i3;
						uint32_t i4 = 0;
						do {
							tempDims[3] = i4;
							uint32_t indexStart1 = getCellIndex(dims, tempDims, dimCount);
							uint32_t indexStart2 = getCellIndex(dimensionSizes, tempDims, dimCount);

							size_t count = sizeof(T) * copySize[dimCount - 1];

							DEBUG("Copying memory to newData position %i from data position %i, count %i", indexStart1, indexStart2, count);

							memcpy(newData + indexStart1, data + indexStart2, count);
							i4++;
						} while (i4 < copySize[4]);
						i3++;
					} while (i3 < copySize[3]);
					i2++;
				} while (i2 < copySize[2]);
				i1++;
			} while (i1 < copySize[1]);
		}

		if (typeid(T) == typeid(ISString)) {
			for (uint32_t i = 0; i != size; i++) {
				data[i].~T();
			}
		}
		delete[] data;
		data = newData;
		s = 1;
		for (int i = dimCount - 1; i >= 0; --i) {
			dimensions[i] = s;
			s *= dims[i];
			dimensionSizes[i] = dims[i];
		}
		size = s;
	}
	else {
		if (typeid(T) == typeid(ISString)) {
			for (uint32_t i = 0; i != size; i++) {
				data[i].~T();
			}
		}
		delete[] data;
		uint32_t s(1);
		for (int i = dimCount - 1; i >= 0; --i) {
			dimensions[i] = s;
			s *= dims[i];
			dimensionSizes[i] = dims[i];
		}
		size = s;
		data = new T[size];
		memset(data, 0, size * sizeof(T));
	}
}

#endif

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
		uint32_t getSize() const {return size;}
		uint32_t *getDimensionSizes() { return dimensionSizes;}
	private:
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
void Array<T>::resize(uint32_t *dims, uint32_t dimCount, bool copy) {
	if (copy) {
		uint32_t s(1);
		uint32_t dimMult[5] = {0};
		for (int i = dimCount - 1; i >= 0; --i) {
			dimMult[i] = s;
			s *= dims[i];
		}
		T *newData = new T[s];
		memset(newData, 0, s * sizeof(T));
		uint32_t copySize[5] = {0};
		for (uint32_t i = 0; i != dimCount; i++) {
			copySize[i] = dims[i] < dimensionSizes[i] ? dims[i] : dimensionSizes[i];
		}
		switch (dimCount) {
			case 1:
				if (typeid(T) == typeid(ISString)) {
					for (uint32_t i = 0; i < copySize[0]; i++) {
						newData[i] = data[i];
					}
				}
				else {
					memcpy(newData,data,sizeof(T) * copySize[0]);
				}
				break;
			case 2:
				if (typeid(T) == typeid(ISString)) {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						uint32_t place1 = dimMult[0] * i1;
						uint32_t place2 = dimensions[0] * i1;
						for (uint32_t i2 = 0; i2 < copySize[1]; i2++) {
							newData[place1 + i2] = newData[place2 + i2];
						}
					}
				}
				else {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						uint32_t place1 = dimMult[0] * i1;
						uint32_t place2 = dimensions[0] * i1;
						memcpy(newData + place1, data + place2, sizeof(T) * copySize[1]);
					}
				}
				break;
			case 3:
				if (typeid(T) == typeid(ISString)) {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2;
							uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2;
							for (uint32_t i3 = 0; i3 < copySize[2]; i3++) {
								newData[place1 + i3] = data[place2 + i3];
							}
						}
					}
				}
				else {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2;
							uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2;
							memcpy(newData + place1, data + place2, sizeof(T) * copySize[2]);
						}
					}
				}
				break;
			case 4:
				if (typeid(T) == typeid(ISString)) {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							for (uint32_t i3 = 0; i3 != copySize[2]; i3++) {
								uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2 + dimMult[2] * i3;
								uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2 + dimensions[3] * i3;
								for (uint32_t i4 = 0; i4 < copySize[3]; i4++) {
									newData[place1 + i4] = data[place2 + i4];
								}
							}
						}
					}
				}
				else {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							for (uint32_t i3 = 0; i3 != copySize[2]; i3++) {
								uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2 + dimMult[2] * i3;
								uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2 + dimensions[3] * i3;
								memcpy(newData + place1, data + place2, sizeof(T) * copySize[3]);
							}
						}
					}
				}
				break;
			case 5:
				if (typeid(T) == typeid(ISString)) {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							for (uint32_t i3 = 0; i3 != copySize[2]; i3++) {
								for (uint32_t i4 = 0; i4 != copySize[3]; i4++) {
									uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2 + dimMult[2] * i3 + dimMult[3] * i4;
									uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2 + dimensions[2] * i3 + dimensions[3] * i4;
									for (uint32_t i5 = 0; i5 < copySize[4]; i5++) {
										newData[place1 + i5] = data[place2];
									}
								}
							}
						}
					}
				}
				else {
					for (uint32_t i1 = 0; i1 != copySize[0]; i1++) {
						for (uint32_t i2 = 0; i2 != copySize[1]; i2++) {
							for (uint32_t i3 = 0; i3 != copySize[2]; i3++) {
								for (uint32_t i4 = 0; i4 != copySize[3]; i4++) {
									uint32_t place1 = dimMult[0] * i1 + dimMult[1] * i2 + dimMult[2] * i3 + dimMult[3] * i4;
									uint32_t place2 = dimensions[0] * i1 + dimensions[1] * i2 + dimensions[2] * i3 + dimensions[3] * i4;
									memcpy(newData + place1, data + place2, sizeof(T) * copySize[4]);
								}
							}
						}
					}
				}
				break;
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
		delete[] data;
		uint32_t s(1);
		for (uint32_t i = 0; i < dimCount; ++i) {
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

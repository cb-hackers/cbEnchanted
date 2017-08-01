#include "cbenchanted.h"
#include "precomp.h"
#include "meminterface.h"
#include <cstring>
#include <stdint.h>
#include "errorsystem.h"
#ifndef CBE_LIB
#ifdef CBE_MEMBLOCK_BOUNDS_CHECK
	#define MEMBLOCK_BOUNDS_CHECK(mem, funcName, index, sizeOf) \
		if (index < 0 || ((index) + (sizeOf)) > *((int32_t*)(mem))) \
		{ CBEnchanted::instance()->errors->createError((funcName) + string(": Out of bounds"),\
			"Index: " + std::to_string(index) + "\nmemblock size: " + std::to_string(*((int32_t*)(mem))));\
			CBEnchanted::instance()->pushValue(0); return; }
#else
	#define MEMBLOCK_BOUNDS_CHECK(mem, funcName, index, sizeOf)
#endif

MemInterface::MemInterface() {
	cb = CBEnchanted::instance();
}

MemInterface::~MemInterface() {

}

template<class T> static T &getMemBlockData(uint8_t *mem, uint32_t p) {
#ifdef CBE_MEMBLOCK_BOUNDS_CHECK
	if ((p + sizeof(T)) > *(int32_t*)mem) { //Out of bounds
		CBEnchanted::instance()->errors->createError("");
	}
#endif
	return *(T*)(mem + p + 4);
}



//DeleteMEMBlock
void MemInterface::commandDeleteMEMBlock(void) {
	int32_t id = cb->popValue().getInt();

	uint8_t* mem = getMemblock(id);
	if (mem == 0) return;
	delete mem;
}

//ResizeMEMblock
void MemInterface::commandResizeMEMBlock(void) {
	int32_t size = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	if (mem == 0) return;
	int32_t oldsize = mem[0];

	uint8_t *newBlock = new uint8_t[size + 4];
	*((int32_t*)newBlock) = size;

	if (oldsize < size) {
		memset(newBlock + 4 + oldsize, 0, size - oldsize);
		memcpy(newBlock + 4, mem + 4, (size_t)oldsize);
	}
	else {
		memcpy(newBlock + 4, mem + 4, (size_t)size);
	}

	delete mem;
	memblockMap[id] = newBlock;
}

//MemCopy
void MemInterface::commandMemCopy(void) {
	int32_t length = cb->popValue().getInt();
	int32_t dest = cb->popValue().getInt();
	int32_t destId = cb->popValue().getInt();

	uint8_t *destmem = getMemblock(destId);
	if (destmem == 0) return;
	int32_t src = cb->popValue().getInt();
	int32_t srcId = cb->popValue().getInt();

	uint8_t *srcmem = getMemblock(srcId);
	if (srcmem == 0) return;

#ifdef CBE_MEMBLOCK_BOUNDS_CHECK
	int32_t destMemSize = *(int32_t*)destmem;
	int32_t srcMemSize = *(int32_t*)srcmem;
	if ((dest + length > destMemSize) || (src + length > srcMemSize)) {
		cb->errors->createError("MemCopy out of bounds", "Destination index: " + std::to_string(dest)
								+ "\n Source index: " + std::to_string(src)
								+ "\n Trying to copy " + std::to_string(length) + " bytes");
		return;
	}
#endif

	memcpy(destmem + dest + 4, srcmem + src + 4, length);
}

//PokeByte
void MemInterface::commandPokeByte(void) {
	uint8_t value = cb->popValue().toByte();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t * mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PokeByte", position, 1);
	*((uint8_t *)(mem + position + 4)) = value;
}

//PokeShort
void MemInterface::commandPokeShort(void) {
	uint16_t value = cb->popValue().toShort();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t * mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PokeShort", position, 2);
	*((uint16_t *)(mem + position + 4)) = value;
}

//PokeInt
void MemInterface::commandPokeInt(void){
	int32_t value = cb->popValue().getInt();
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t * mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PokeInt", position, 4);
	*((int32_t *)(mem + position + 4)) = value;
}

//PokeFloat
void MemInterface::commandPokeFloat(void){
	float value = cb->popValue().toFloat();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t * mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PokeFloat", position, 4);
	*((float *)(mem + position + 4)) = value;
}

//MakeMemblock
void MemInterface::functionMakeMEMBlock(void){
	int32_t size = cb->popValue().getInt();
	uint8_t *mem = new uint8_t[size + 4];
	*((int32_t *)mem) = size;
	memset(mem  + 4, 0, size);

	int32_t id = nextId();
	memblockMap[id] = mem;
	cb->pushValue(id);
}

//MemblockSize
void MemInterface::functionMEMBlockSize(void){
	int32_t id = cb->popValue().getInt();

	int32_t *mem = (int32_t *)getMemblock(id);
	if (mem == 0) return;
	cb->pushValue(*mem);
}

//PeekByte
void MemInterface::functionPeekByte(void) {
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t *mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PeekByte", position, 1);
	cb->pushValue(mem[position + 4]);
}

//PeekShort
void MemInterface::functionPeekShort(void) {
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t *mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PeekShort", position, 2);
	cb->pushValue(*(uint16_t*)(&mem[position + 4]));
}

//PeekInt
void MemInterface::functionPeekInt(void){
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t *mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PeekInt", position, 4);
	cb->pushValue(*(int32_t*)(&mem[position + 4]));
}

//PeekFloat
void MemInterface::functionPeekFloat(void){
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();

	uint8_t *mem = getMemblock(id);
	if (mem == 0) return;
	MEMBLOCK_BOUNDS_CHECK(mem, "PeekFloat", position, 4);
	cb->pushValue(*(float*)(&mem[position + 4]));
}

#endif

uint8_t *MemInterface::getMemblock(int32_t id) {
	map<int32_t, uint8_t*>::iterator i = memblockMap.find(id);
	if (i == memblockMap.end()) {
		cb->errors->createError("Memory Access Violation", "Could not find memblock with ID " + std::to_string(id));
		return 0;
	}
	return i->second;
}

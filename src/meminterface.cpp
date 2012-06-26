#include "cbenchanted.h"
#include "precomp.h"
#include "meminterface.h"
#include <cstring>
#include <stdint.h>

MemInterface::MemInterface() {
	cb = static_cast<CBEnchanted*>(this);
}

MemInterface::~MemInterface() {

}

//DeleteMEMBlock
void MemInterface::commandDeleteMEMBlock(void) {
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t*mem = getMemblock(id);
	delete mem;
}

//ResizeMEMblock
void MemInterface::commandResizeMEMBlock(void) {
	int32_t size = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	int32_t oldsize = mem[0];
	uint8_t *newBlock = new uint8_t[size + 4];
	*((int32_t*)newBlock) = size;
	memcpy(newBlock+4, mem+4, (size_t)oldsize);
	delete mem;
	memblockMap[id] = newBlock;
}

//MemCopy
void MemInterface::commandMemCopy(void) {
	int32_t lenght = cb->popValue().getInt();
	int32_t dest = cb->popValue().getInt();
	int32_t destId = cb->popValue().getInt();
	assert(destId);
	uint8_t *destmem = getMemblock(destId);
	int32_t src = cb->popValue().getInt();
	int32_t srcId = cb->popValue().getInt();
	assert(srcId);
	uint8_t *srcmem = getMemblock(srcId);
	memcpy(destmem + dest + 4, srcmem + src + 4, lenght);
}

//PokeByte
void MemInterface::commandPokeByte(void) {
	uint8_t value = cb->popValue().toByte();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t * mem = getMemblock(id);
	*((uint8_t *)(mem + position + 4)) = value;
}

//PokeShort
void MemInterface::commandPokeShort(void) {
	uint16_t value = cb->popValue().toShort();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t * mem = getMemblock(id);
	*((uint16_t *)(mem + position + 4)) = value;
}

//PokeInt
void MemInterface::commandPokeInt(void){
	int32_t value = cb->popValue().getInt();
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t * mem = getMemblock(id);
	*((int32_t *)(mem + position + 4)) = value;
}

//PokeFloat
void MemInterface::commandPokeFloat(void){
	float value = cb->popValue().toFloat();
	int32_t position  = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t * mem = getMemblock(id);
	*((float *)(mem + position + 4)) = value;
}

//MakeMemblock
void MemInterface::functionMakeMEMBlock(void){
	int32_t size = cb->popValue().getInt();
	uint8_t *mem = new uint8_t[size + 4];
	*((int32_t *)mem) = size;
	int32_t id = nextId();
	memblockMap[id] = mem;
	cb->pushValue(id);
}

//MemblockSize
void MemInterface::functionMEMBlockSize(void){
	int32_t id = cb->popValue().getInt();
	assert(id);
	int32_t *mem = (int32_t *)getMemblock(id);
	cb->pushValue(*mem);
}

//PeekByte
void MemInterface::functionPeekByte(void) {
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	cb->pushValue(mem[position + 4]);
}

//PeekShort
void MemInterface::functionPeekShort(void) {
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	cb->pushValue(*(uint16_t*)(&mem[position+4]));
}

//PeekInt
void MemInterface::functionPeekInt(void){
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	cb->pushValue(*(int32_t*)(&mem[position+4]));
}

//PeekFloat
void MemInterface::functionPeekFloat(void){
	int32_t position = cb->popValue().getInt();
	int32_t id = cb->popValue().getInt();
	assert(id);
	uint8_t *mem = getMemblock(id);
	cb->pushValue(*(float*)(&mem[position+4]));
}

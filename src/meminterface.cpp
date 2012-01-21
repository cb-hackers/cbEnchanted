#include "cbenchanted.h"
#include "precomp.h"
#include "meminterface.h"
#include <cstring>
#include <stdint.h>

MemInterface::MemInterface() {
	
}

MemInterface::~MemInterface() {
	
}

//DeleteMEMBlock
void MemInterface::commandDeleteMEMBlock(void) {
	int32_t *mem = (int32_t *)cb->popValue().getInt();
	delete mem;
}

//ResizeMEMblock
void MemInterface::commandResizeMEMBlock(void) {
	int32_t size = cb->popValue().getInt();
	int32_t *mem = (int32_t *)cb->popValue().getInt();
	int32_t memsize = mem[0];
	int8_t *newblock = new int8_t[size + 4];
	memcpy(newblock, mem, (size_t)memsize);
	delete mem;
	cb->pushValue((int32_t)newblock);
}

//MemCopy
void MemInterface::commandMemCopy(void) {
	int32_t lenght = cb->popValue().getInt();
	int32_t dest = cb->popValue().getInt();
	int32_t *destmem = (int32_t *)cb->popValue().getInt();
	int32_t src = cb->popValue().getInt();
	int32_t *srcmem = (int32_t *)cb->popValue().getInt();
	memcpy(destmem + dest + 4, srcmem + src + 4, lenght);
}

//PokeByte
void MemInterface::commandPokeByte(void) {
	uint8_t value = cb->popValue().toByte();
	int32_t position  = cb->popValue().getInt();
	unsigned char * mem = (uint8_t *)cb->popValue().getInt();
	*((uint8_t *)(mem + position + 4)) = value;
}

//PokeShort
void MemInterface::commandPokeShort(void) {
	uint16_t value = cb->popValue().toShort();
	int32_t position  = cb->popValue().getInt();
	uint8_t *mem = (uint8_t *)cb->popValue().getInt();
	*((uint16_t *)(mem + position + 4)) = value;
}

//PokeInt
void MemInterface::commandPokeInt(void){
	int32_t value = cb->popValue().getInt();
	int32_t position = cb->popValue().getInt();
	uint8_t *mem = (uint8_t *)cb->popValue().getInt();
	*((int32_t *)(mem + position + 4)) = value;
}

//PokeFloat
void MemInterface::commandPokeFloat(void){
	float value = cb->popValue().toFloat();
	int32_t position  = cb->popValue().getInt();
	uint8_t *mem = (uint8_t *)cb->popValue().getInt();
	*((float *)(mem + position + 4)) = value;
}

//MakeMemblock
void MemInterface::functionMakeMEMBlock(void){
	int32_t size = cb->popValue().getInt();
	uint8_t *mem = new uint8_t[size + 4];
	*((uint32_t *)mem) = size;
	cb->pushValue((int32_t) mem);
}

//MemblockSize
void MemInterface::functionMEMBlockSize(void){
	int32_t *mem = (int32_t *)cb->popValue().getInt();
	int32_t data = mem[0];
	cb->pushValue(data);
}

//PeekByte
void MemInterface::functionPeekByte(void) {
	int32_t position = cb->popValue().getInt();
	uint8_t *mem = (uint8_t *)cb->popValue().getInt();
	uint8_t data = mem[position + 4];
	cb->pushValue(data);
}

//PeekShort
void MemInterface::functionPeekShort(void) {
	int32_t position = cb->popValue().getInt();
	uint16_t *mem = (uint16_t *)cb->popValue().getInt();
	uint16_t data = mem[position + 4];
	cb->pushValue(data);
}

//PeekInt
void MemInterface::functionPeekInt(void){
	int32_t position = cb->popValue().getInt();
	int32_t *mem = (int32_t*)cb->popValue().getInt();
	int32_t data = mem[position + 4];
	cb->pushValue(data);
}

//PeekFloat
void MemInterface::functionPeekFloat(void){
	int32_t position = cb->popValue().getInt();
	float *mem = (float *)cb->popValue().getInt();
	float data = mem[position + 4];
	cb->pushValue(data);
}

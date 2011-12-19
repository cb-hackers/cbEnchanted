#include "cbenchanted.h"
#include "precomp.h"
#include "meminterface.h"
#include <cstring>

MemInterface::MemInterface() {
	
}

MemInterface::~MemInterface() {
	
}

//DeleteMEMBlock
void MemInterface::commandDeleteMEMBlock(void) {
    int32_t* mem = (int32_t*)cb->popValue<int32_t>();
    delete mem;
}

//ResizeMEMblock
void MemInterface::commandResizeMEMBlock(void) {
    int32_t size = cb->popValue<int32_t>();
    int32_t* mem = (int32_t*)cb->popValue<int32_t>();
    int32_t memsize = mem[0];
    char* newblock = new char[size+4];
    memcpy(newblock, mem, (size_t)memsize);
    delete mem;
    cb->pushValue((int32_t)newblock);
}

//MemCopy
void MemInterface::commandMemCopy(void) {
    int32_t lenght = cb->popValue<int32_t>();
    int32_t dest = cb->popValue<int32_t>();
    int32_t* destmem = (int32_t*)cb->popValue<int32_t>();
    int32_t src = cb->popValue<int32_t>();
    int32_t* srcmem = (int32_t*)cb->popValue<int32_t>();
    memcpy(destmem+dest+4, srcmem+src+4, lenght);
}

//PokeByte
void MemInterface::commandPokeByte(void) {
    uint8_t value = cb->popValue<uint8_t>();
    int32_t position  = cb->popValue<int32_t>();
    unsigned char* mem = (unsigned char*)cb->popValue<int32_t>();
    *((uint8_t*)(mem + position + 4)) = value;
}

//PokeShort
void MemInterface::commandPokeShort(void) {
    uint16_t value = cb->popValue<uint16_t>();
    int32_t position  = cb->popValue<int32_t>();
    unsigned char* mem = (unsigned char*)cb->popValue<int32_t>();
    *((uint16_t*)(mem + position + 4)) = value;
}

//PokeInt
void MemInterface::commandPokeInt(void){
    int32_t value = cb->popValue<int32_t>();
    int32_t position = cb->popValue<int32_t>();
    unsigned char* mem = (unsigned char*)cb->popValue<int32_t>();
    *((int32_t*)(mem + position + 4))=value;
}

//PokeFloat
void MemInterface::commandPokeFloat(void){
    float value = cb->popValue<float>();
    int32_t position  = cb->popValue<int32_t>();
    unsigned char* mem = (unsigned char*)cb->popValue<int32_t>();
    *((float*)(mem + position + 4)) = value;
}

//MakeMemblock
void MemInterface::functionMakeMEMBlock(void){
    int32_t size = cb->popValue<int32_t>();
    char *mem = new char[size + 4];
    *((uint32_t*)mem) = size;
    cb->pushValue((int32_t)mem);
}

//MemblockSize
void MemInterface::functionMEMBlockSize(void){
    int32_t* mem = (int32_t*)cb->popValue<int32_t>();
    int32_t data = mem[0];
    cb->pushValue(data);
}

//PeekByte
void MemInterface::functionPeekByte(void) {
    int32_t position = cb->popValue<int32_t>();
    uint8_t* mem = (uint8_t*)cb->popValue<int32_t>();
    uint8_t data = mem[position+4];
    cb->pushValue(data);
}

//PeekShort
void MemInterface::functionPeekShort(void) {
    int32_t position = cb->popValue<int32_t>();
    uint16_t* mem = (uint16_t*)cb->popValue<int32_t>();
    uint16_t data = mem[position+4];
    cb->pushValue(data);
}

//PeekInt
void MemInterface::functionPeekInt(void){
    int32_t position = cb->popValue<int32_t>();
    int32_t* mem = (int32_t*)cb->popValue<int32_t>();
    int32_t data = mem[position+4];
    cb->pushValue(data);
}

//PeekFloat
void MemInterface::functionPeekFloat(void){
    int32_t position = cb->popValue<int32_t>();
    float* mem = (float*)cb->popValue<int32_t>();
    float data = mem[position+4];
    cb->pushValue(data);
}

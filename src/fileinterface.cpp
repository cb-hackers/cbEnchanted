#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"

FileInterface::FileInterface() : cb(static_cast <CBEnchanted *> (this)) {
	idC=0;
}

FileInterface::~FileInterface() {
	
}

void FileInterface::commandCloseFile(void) {
	fstream *filestr2;
	filestr2 = filestrs[cb->popValue().getInt()];
	filestr2->close();
}

void FileInterface::commandSeekFile(void) {
	
}

void FileInterface::commandStartSearch(void) {
	
}

void FileInterface::commandEndSearch(void) {
	
}

void FileInterface::commandChDir(void) {
	
}

void FileInterface::commandMakeDir(void) {
	
}

void FileInterface::commandCopyFile(void) {
	
}

void FileInterface::commandDeleteFile(void) {
	
}

void FileInterface::commandExecute(void) {
	
}

void FileInterface::commandWriteByte(void) {
	
}

void FileInterface::commandWriteShort(void) {
	
}

void FileInterface::commandWriteInt(void) {
	
}

void FileInterface::commandWriteFloat(void) {
	
}

void FileInterface::commandWriteString(void) {
	
}

void FileInterface::commandWriteLine(void) {
	
}

void FileInterface::commandReadByte(void) {
	
}

void FileInterface::commandReadShort(void) {
	
}

void FileInterface::commandReadInt(void) {
	
}

void FileInterface::commandReadFloat(void) {
	
}

void FileInterface::commandReadString(void) {
	
}

void FileInterface::commandReadLine(void) {

}

void FileInterface::functionOpenToRead(void) {
	string file = cb->popValue().toString();
	filestr.open(file, fstream::in);
	int32_t id = ++idC;
	fstream *fptr = &filestr;
	filestrs[id] = fptr;

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {
	
}

void FileInterface::functionOpenToEdit(void) {
	
}

void FileInterface::functionFileOffset(void) {
	
}

void FileInterface::functionFindFile(void) {
	
}

void FileInterface::functionCurrentDir(void) {
	
}

void FileInterface::functionFileExists(void) {
	
}

void FileInterface::functionIsDirectory(void) {
	
}

void FileInterface::functionFileSize(void) {
	
}

void FileInterface::functionEOF(void) {
	
}

void FileInterface::functionReadByte(void) {
	
}

void FileInterface::functionReadShort(void) {
	
}

void FileInterface::functionReadInt(void) {
	
}

void FileInterface::functionReadFloat(void) {
	
}

void FileInterface::functionReadString(void) {
	
}

void FileInterface::functionReadLine(void) {
	fstream *filestr2;
	filestr2 = filestrs[cb->popValue().getInt()];

	string line;
	getline(*filestr2,line);
	cb->pushValue(line);
}

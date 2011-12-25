#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"
#include <cstdio>

FileInterface::FileInterface() : cb(static_cast <CBEnchanted *> (this)) {

}

FileInterface::~FileInterface() {
	
}

void FileInterface::commandCloseFile(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];
	fclose(file2);
}

void FileInterface::commandSeekFile(void) {
	fpos_t pos = cb->popValue().getInt();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	fsetpos(file2,&pos);
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
	char byte = cb->popValue().getByte();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	fputc((int) byte,file2);
}

void FileInterface::commandWriteShort(void) {
	short sh = cb->popValue().getShort();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	fwrite(&sh,sizeof(short),1,file2);

}

void FileInterface::commandWriteInt(void) {
	int32_t i = cb->popValue().getInt();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	fwrite(&i,sizeof(int32_t),1,file2);
}

void FileInterface::commandWriteFloat(void) {
	float fl = cb->popValue().getFloat();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	fwrite(&fl,sizeof(float),1,file2);
}

void FileInterface::commandWriteString(void) {
	string sstring = cb->popValue().toString();

	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	int l = int(sstring.length());
	char * str;
	str = new char [sstring.length()+1];
	strcpy(str,sstring.c_str());

	fwrite(&l,sizeof(int32_t),1,file2);
	fwrite(str,l,1,file2);
	delete[] str;
}

void FileInterface::commandWriteLine(void) {
	string line = cb->popValue().toString();
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	char * cLine;
	cLine = new char [line.length()+1];
	strcpy(cLine,line.c_str());

	fputs(cLine,file2);

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

	string sfile = cb->popValue().toString();
	char * cFile;
	cFile = new char [sfile.length()+1];
	strcpy(cFile,sfile.c_str());

	int32_t id = ++idC;

	filestrs[id] = fopen(cFile, "r");;

	delete[] cFile;

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {

	string sfile = cb->popValue().toString();
	char * cFile;
	cFile = new char [sfile.length()+1];
	strcpy(cFile,sfile.c_str());
	int32_t id = ++idC;

	filestrs[id] = fopen(cFile, "w");

	delete[] cFile;

	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {

	string sfile = cb->popValue().toString();
	char * cFile;
	cFile = new char [sfile.length()+1];
	strcpy(cFile,sfile.c_str());
	int32_t id = ++idC;

	filestrs[id] = fopen(cFile, "r+");

	delete[] cFile;

	cb->pushValue(id);
}

void FileInterface::functionFileOffset(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	cb->pushValue(ftell(file2));
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
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	bool eof = 0;
	if (feof(file2)!=0) {eof=1;}

	cb->pushValue(eof);

}

void FileInterface::functionReadByte(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	cb->pushValue(char(fgetc(file2)));
}

void FileInterface::functionReadShort(void) {

	short sh;

	fread(&sh, sizeof(short), 1, filestrs[cb->popValue().getInt()]);

	cb->pushValue(sh);

}

void FileInterface::functionReadInt(void) {
	int32_t i;

	fread(&i, sizeof(int32_t), 1, filestrs[cb->popValue().getInt()]);

	cb->pushValue(i);
}

void FileInterface::functionReadFloat(void) {
	float fl=0.0f;

	fread(&fl, sizeof(float), 1, filestrs[cb->popValue().getInt()]);

	cb->pushValue(fl);
}

void FileInterface::functionReadString(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	int32_t i;

	fread(&i, sizeof(int32_t), 1, file2);

	char * str;
	str = new char [i+1];

	fread(str, i, 1, file2);

	string line(str);

	line=line.substr(0, line.length()-1);

	delete[] str;

	cb->pushValue(line);
}

void FileInterface::functionReadLine(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	string line = "";
	int c;
	while(1) {

		c = fgetc(file2);
		if (c != '\n' && c != EOF)
		{
			line = line + char(c);
		} else {
			break;
		}

	}
	cb->pushValue(line);
}

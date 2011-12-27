#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"
#include <cstdio>
#include <boost/filesystem.hpp>

FileInterface::FileInterface() : cb(static_cast <CBEnchanted *> (this)) {

}

FileInterface::~FileInterface() {
	
}

void FileInterface::commandCloseFile(void) {
	fclose(filestrs[cb->popValue().getInt()]);
}

void FileInterface::commandSeekFile(void) {
	fpos_t pos = cb->popValue().getInt();
	fsetpos(filestrs[cb->popValue().getInt()],&pos);
}

void FileInterface::commandStartSearch(void) {

}

void FileInterface::commandEndSearch(void) {
	
}

void FileInterface::commandChDir(void) {
	current_path(path(cb->popValue().toString()));
}

void FileInterface::commandMakeDir(void) {
	create_directory(path(cb->popValue().toString()));
}

void FileInterface::commandCopyFile(void) {
	string file_s2 = cb->popValue().toString();
	string file_s1 = cb->popValue().toString();
	copy_file(path(file_s1),path(file_s2));
}

void FileInterface::commandDeleteFile(void) {
	remove(path(cb->popValue().toString()));
}

void FileInterface::commandExecute(void) {
	string file_s = cb->popValue().toString();
	system(file_s.c_str());
}

void FileInterface::commandWriteByte(void) {
	char byte = cb->popValue().getByte();
	fputc((int) byte,filestrs[cb->popValue().getInt()]);
}

void FileInterface::commandWriteShort(void) {
	short sh = cb->popValue().getShort();
	fwrite(&sh,sizeof(short),1,filestrs[cb->popValue().getInt()]);

}

void FileInterface::commandWriteInt(void) {
	int32_t i = cb->popValue().getInt();
	fwrite(&i,sizeof(int32_t),1,filestrs[cb->popValue().getInt()]);
}

void FileInterface::commandWriteFloat(void) {
	float fl = cb->popValue().getFloat();
	fwrite(&fl,sizeof(float),1,filestrs[cb->popValue().getInt()]);
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

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "r");;

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "w");

	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "r+");

	cb->pushValue(id);
}

void FileInterface::functionFileOffset(void) {

	cb->pushValue(int(ftell(filestrs[cb->popValue().getInt()])));
}

void FileInterface::functionFindFile(void) {

}

void FileInterface::functionCurrentDir(void) {
	cb->pushValue(current_path().string());
}

void FileInterface::functionFileExists(void) {
	cb->pushValue(exists(cb->popValue().toString()));
}

void FileInterface::functionIsDirectory(void) {
	cb->pushValue(is_directory(cb->popValue().toString()));
}

void FileInterface::functionFileSize(void) {
	cb->pushValue(int32_t(file_size(cb->popValue().toString())));
}

void FileInterface::functionEOF(void) {
	FILE *file2;
	file2 = filestrs[cb->popValue().getInt()];

	int32_t eof = 0;
	if (feof(file2)!=0) {eof=1;}

	cb->pushValue(eof);

}

void FileInterface::functionReadByte(void) {
	cb->pushValue(char(fgetc(filestrs[cb->popValue().getInt()])));
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

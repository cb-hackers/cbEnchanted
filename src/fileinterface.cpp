#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"

FileInterface::FileInterface() : cb(static_cast <CBEnchanted *> (this)) {

}

FileInterface::~FileInterface() {
	
}

void FileInterface::commandCloseFile(void) {
	int32_t ID;
	ID = cb->popValue().getInt();
	if (filestrs[ID]==0)
	{
		FIXME("CloseFile failed.");
		cb->pushValue(0);
	}

	if(fclose(filestrs[ID])!=0)
	{
		FIXME("CloseFile failed.");
		cb->pushValue(0);
	}
}

void FileInterface::commandSeekFile(void) {
	int32_t pos = cb->popValue().getInt();
	fseek(filestrs[cb->popValue().getInt()], pos, SEEK_SET);
}

void FileInterface::commandStartSearch(void) {
	directory_iterator new_dir_iter(current_path());
	directory_iterator new_dir_end;

	dir_iter = new_dir_iter;
	dir_end = new_dir_end;
}

void FileInterface::commandEndSearch(void) {
	directory_iterator new_dir_end;

	dir_iter = new_dir_end;
	dir_end = new_dir_end;
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
	string cmd = cb->popValue().toString();
	string scmd;

	#ifdef WIN32
		scmd = "start " + cmd;
	#else
		scmd = "xdg-open " + cmd;
	#endif

	system(scmd.c_str());

}

void FileInterface::commandWriteByte(void) {
    uint8_t byte = cb->popValue().getByte();
	fputc((int) byte,filestrs[cb->popValue().getInt()]);
}

void FileInterface::commandWriteShort(void) {
    uint16_t sh = cb->popValue().getShort();
    fwrite(&sh,sizeof(uint16_t),1,filestrs[cb->popValue().getInt()]);

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

	line += "\n";

	fputs(line.c_str(),file2);

}

void FileInterface::commandReadByte(void) {
	STUB;
}

void FileInterface::commandReadShort(void) {
	STUB;
}

void FileInterface::commandReadInt(void) {
	STUB;
}

void FileInterface::commandReadFloat(void) {
	STUB;
}

void FileInterface::commandReadString(void) {
	STUB;
}

void FileInterface::commandReadLine(void) {
	STUB;
}

void FileInterface::functionOpenToRead(void) {

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "r");;
	if (filestrs[id]==NULL) {
		FIXME("OpenToRead failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "w");
	if (filestrs[id]==NULL) {
		FIXME("OpenToWrite failed.");
		cb->pushValue(0);
	}


	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {

	string file_s = cb->popValue().toString();
	int32_t id = ++idC;

	filestrs[id] = fopen(file_s.c_str(), "r+");
	if (filestrs[id]==NULL) {
		FIXME("OpenToEdit failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionFileOffset(void) {

	cb->pushValue(int32_t(ftell(filestrs[cb->popValue().getInt()])));
}

void FileInterface::functionFindFile(void) {
	if (dir_iter != dir_end) {
		path filepath(dir_iter->path());
		cb->pushValue(filepath.filename().string());
		++dir_iter;
	} else {
		cb->pushValue(string(""));
	}
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
    cb->pushValue(uint8_t(fgetc(filestrs[cb->popValue().getInt()])));
}

void FileInterface::functionReadShort(void) {

    uint16_t sh;

    fread(&sh, sizeof(uint16_t), 1, filestrs[cb->popValue().getInt()]);

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

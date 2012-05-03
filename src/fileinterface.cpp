#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"

FileInterface::FileInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

FileInterface::~FileInterface() {
}

void FileInterface::commandCloseFile(void) {
	int32_t ID;
	ID = cb->popValue().getInt();
	if (filestrs[ID] == NULL)
	{
		FIXME("CloseFile failed.");
		cb->pushValue(0);
	}

	if(fclose(filestrs[ID]) != 0)
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
	fs::directory_iterator new_dir_iter(fs::current_path());
	fs::directory_iterator new_dir_end;

	rcount = 0;

	dir_iter = new_dir_iter;
	dir_end = new_dir_end;
}

void FileInterface::commandEndSearch(void) {
	fs::directory_iterator new_dir_end;

	rcount = 0;

	dir_iter = new_dir_end;
	dir_end = new_dir_end;
}

void FileInterface::commandChDir(void) {
	fs::current_path(fs::path(cb->popValue().toString().getRef()));
}

void FileInterface::commandMakeDir(void) {
	fs::create_directory(fs::path(cb->popValue().toString().getRef()));
}

void FileInterface::commandCopyFile(void) {
	string file_s2 = cb->popValue().toString().getRef();
	string file_s1 = cb->popValue().toString().getRef();
	//fs::copy_file(fs::path(file_s1), fs::path(file_s2));
}

void FileInterface::commandDeleteFile(void) {
	fs::remove(fs::path(cb->popValue().toString().getRef()));
}

void FileInterface::commandExecute(void) {
	string cmd = cb->popValue().toString().getRef();
	string scmd;

	#ifdef WIN32
		scmd = "start " + cmd;
	#else
		scmd = "xdg-open " + cmd;
	#endif

	system(scmd.c_str());
}

void FileInterface::commandWriteByte(void) {
	uint8_t byte = cb->popValue().toByte();
	fputc((int) byte, filestrs[cb->popValue().toInt()]);
}

void FileInterface::commandWriteShort(void) {
	uint16_t sh = cb->popValue().toShort();
	fwrite(&sh, sizeof(uint16_t), 1, filestrs[cb->popValue().getInt()]);

}

void FileInterface::commandWriteInt(void) {
	int32_t i = cb->popValue().toInt();
	fwrite(&i, sizeof(int32_t), 1, filestrs[cb->popValue().toInt()]);
}

void FileInterface::commandWriteFloat(void) {
	float fl = cb->popValue().toFloat();
	fwrite(&fl, sizeof(float), 1, filestrs[cb->popValue().toInt()]);
}

void FileInterface::commandWriteString(void) {
	string sstring = cb->popValue().toString().getRef();

	FILE *file = filestrs[cb->popValue().toInt()];

	int l = int(sstring.length());

	fwrite(&l, sizeof(int32_t), 1, file);
	fwrite(sstring.c_str(), l, 1, file);

}

void FileInterface::commandWriteLine(void) {
	string line = cb->popValue().toString().getRef();
	FILE *file = filestrs[cb->popValue().getInt()];

	line += "\n";

	fputs(line.c_str(), file);
}

void FileInterface::commandReadByte(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	uint8_t byte;
	fread((char*)&byte, 1, 1, file);
	cb->pushValue(byte);
}

void FileInterface::commandReadShort(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	uint16_t shortint;
	fread((char*)&shortint, 2, 1, file);
	cb->pushValue(shortint);
}

void FileInterface::commandReadInt(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	int32_t integer;
	fread((char*)&integer, 4, 1, file);
	cb->pushValue(integer);
}

void FileInterface::commandReadFloat(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	float decimal;
	fread((char*)&decimal, 4, 1, file);
	cb->pushValue(decimal);
}

void FileInterface::commandReadString(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	int32_t lenght;
	fread((char*)&lenght, 4, 1, file);
	char *txt = new char[lenght];
	fread((char*)txt, 1, lenght, file);
	string text = txt;
	cb->pushValue(text);
	delete [] txt;
}

void FileInterface::commandReadLine(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	string str;
	char txt[10000];
	fgets(txt, 10000, file);
	str = txt;
	cb->pushValue(str);
}

void FileInterface::functionOpenToRead(void) {
	int32_t id = ++idC;

	filestrs[id] = fopen(cb->popValue().toString().getRef().c_str(), "r");;
	if (filestrs[id] == NULL) {
		FIXME("OpenToRead failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {
	int32_t id = ++idC;

	filestrs[id] = fopen(cb->popValue().getString().getRef().c_str(), "w");
	if (filestrs[id] == NULL) {
		FIXME("OpenToWrite failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {
	string file_s = cb->popValue().toString().getRef();
	int32_t id = ++idC;

	if(fs::exists(file_s))
	{
		filestrs[id] = fopen(file_s.c_str(), "r+");
	} else {
		filestrs[id] = fopen(file_s.c_str(), "w+");
	}
	if (filestrs[id] == NULL) {
		FIXME("OpenToEdit failed.");
		cb->pushValue(0);
	}
	cb->pushValue(id);
}

void FileInterface::functionFileOffset(void) {
	cb->pushValue(int32_t(ftell(filestrs[cb->popValue().getInt()])));
}

void FileInterface::functionFindFile(void) {
	++rcount;
	if (rcount == 1) {
		if(fs::path(fs::current_path()).has_relative_path()) {
			cb->pushValue(string("."));
		}
		else {
			++rcount;
			functionFindFile();
		}
	}
	else if (rcount == 2) {
		if(fs::path(fs::current_path()).has_parent_path()) {
			cb->pushValue(string(".."));
		}
		else {
			functionFindFile();
		}
	}
	else if (dir_iter != dir_end) {
		cb->pushValue(fs::path(dir_iter->path()).filename().string());
		++dir_iter;
	}
	else {
		cb->pushValue(string(""));
	}
}

void FileInterface::functionCurrentDir(void) {
	cb->pushValue(fs::current_path().string());
}

void FileInterface::functionFileExists(void) {
	cb->pushValue(fs::exists(cb->popValue().toString().getRef()));
}

void FileInterface::functionIsDirectory(void) {
	cb->pushValue(fs::is_directory(cb->popValue().toString().getRef()));
}

void FileInterface::functionFileSize(void) {
	cb->pushValue(int32_t(fs::file_size(cb->popValue().toString().getRef())));
}

void FileInterface::functionEOF(void) {
	FILE *file = filestrs[cb->popValue().getInt()];

	cb->pushValue(feof(file) != 0);
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
	float fl = 0.0f;

	fread(&fl, sizeof(float), 1, filestrs[cb->popValue().getInt()]);

	cb->pushValue(fl);
}

void FileInterface::functionReadString(void) {
	FILE *file = filestrs[cb->popValue().getInt()];

	int32_t i;
	fread(&i, sizeof(int32_t), 1, file);

	char * str;
	str = new char [i + 1];
	fread(str, i, 1, file);

	string line(str);

	delete[] str;

	cb->pushValue(line);
}

void FileInterface::functionReadLine(void) {
	FILE *file = filestrs[cb->popValue().getInt()];

	string line = "";
	while(1) {
		int c = fgetc(file);
		if (c != '\n' && c != EOF) {
			line = line + char(c);
		}
		else {
			break;
		}
	}
	cb->pushValue(line);
}

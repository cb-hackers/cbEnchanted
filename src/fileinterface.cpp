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
	fs::current_path(fs::path(cb->popValue().toString().toStdString()));
}

void FileInterface::commandMakeDir(void) {
	fs::create_directory(fs::path(cb->popValue().toString().toStdString()));
}

void FileInterface::commandCopyFile(void) {
	ISString file_s2 = cb->popValue().toString();
	ISString file_s1 = cb->popValue().toString();
#ifdef WIN32 // Doesn't work on linux for some reason
	fs::copy_file(fs::path(file_s1.toStdString()), fs::path(file_s2.toStdString()));
#endif
}

void FileInterface::commandDeleteFile(void) {
	fs::remove(fs::path(cb->popValue().toString().toStdString()));
}

void FileInterface::commandExecute(void) {
	ISString cmd = cb->popValue().toString();
	string scmd;

	#ifdef WIN32
		scmd = "start " + cmd.toStdString();
	#else
		scmd = "xdg-open " + cmd.toStdString();
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

	ISString sstring = cb->popValue().toString();

	FILE *file = filestrs[cb->popValue().toInt()];

	int32_t l = int32_t(sstring.size());

	fwrite(&l, sizeof(int32_t), 1, file);
	fwrite(sstring.cString(), l, 1, file);

}

void FileInterface::commandWriteLine(void) {
	ISString line = cb->popValue().toString();
	FILE *file = filestrs[cb->popValue().getInt()];

	line += "\n";

	fputs(line.cString(), file);
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
	int32_t size;
	fread((char*)&size, 4, 1, file);
	char *txt = new char[size];
	fread((char*)txt, 1, size, file);
	cb->pushValue(ISString::fromBuffer(txt,size));
	delete [] txt;
}

void FileInterface::commandReadLine(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	char txt[10000];
	fgets(txt, 10000, file);
	cb->pushValue(ISString(txt));
}

void FileInterface::functionOpenToRead(void) {
	int32_t id = ++idC;

	filestrs[id] = fopen(cb->popValue().toString().cString(), "r");
	if (filestrs[id] == NULL) {
		FIXME("OpenToRead failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {
	int32_t id = ++idC;

	filestrs[id] = fopen(cb->popValue().getString().cString(), "w");
	if (filestrs[id] == NULL) {
		FIXME("OpenToWrite failed.");
		cb->pushValue(0);
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {
	string file_s = cb->popValue().toString().toStdString();
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
			cb->pushValue(ISString(".."));
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
		cb->pushValue(ISString());
	}
}

void FileInterface::functionCurrentDir(void) {
	cb->pushValue(fs::current_path().string());
}

void FileInterface::functionFileExists(void) {
	cb->pushValue(fs::exists(cb->popValue().toString().toStdString()));
}

void FileInterface::functionIsDirectory(void) {
	cb->pushValue(fs::is_directory(cb->popValue().toString().toStdString()));
}

void FileInterface::functionFileSize(void) {
	cb->pushValue(int32_t(fs::file_size(cb->popValue().toString().toStdString())));
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

	cb->pushValue(ISString::fromBuffer(str,i));
	delete[] str;
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

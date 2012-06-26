#include "precomp.h"
#include "cbenchanted.h"
#include "fileinterface.h"
#include "errorsystem.h"

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
		cb->errors->createFatalError("CloseFile failed.");
		return;
	}

	if(fclose(filestrs[ID]) != 0)
	{
		cb->errors->createError("CloseFile failed.");
	}
}

void FileInterface::commandSeekFile(void) {
	int32_t pos = cb->popValue().getInt();
	fseek(filestrs[cb->popValue().getInt()], pos, SEEK_SET);
}

void FileInterface::commandStartSearch(void) {
	string dir_str = string(al_get_current_directory()) + ".";
	cur_dir = al_create_fs_entry(dir_str.c_str());
	if(!al_open_directory(cur_dir))
		cb->errors->createError("StartSearch failed! Path: \"" + dir_str + "\"");

	rcount = 0;
}

void FileInterface::commandEndSearch(void) {
	al_close_directory(cur_dir);
	al_destroy_fs_entry(cur_dir);
}

void FileInterface::commandChDir(void) {
	string path_s = cb->popValue().toString().getRef();
	if(!al_change_directory(path_s.c_str()))
		cb->errors->createError("ChDir failed! Path: \"" + path_s + "\"");
}

void FileInterface::commandMakeDir(void) {
	string dir_s = cb->popValue().toString().getRef();
	if(!al_make_directory(dir_s.c_str()))
		cb->errors->createError("MakeDir failed! Directory: \"" + dir_s + "\"");
}

void FileInterface::commandCopyFile(void) {
	string file_s2 = cb->popValue().toString().getRef();
	string file_s1 = cb->popValue().toString().getRef();

	if(al_filename_exists(file_s2.c_str())) {
		cb->errors->createFatalError("CopyFile failed! File \"" + file_s2 + "\" already exists!");
		return;
	}

	uint32_t size = 0;
	char * buffer;

	FILE * file1 = fopen(file_s1.c_str(), "rb");
	if(file1 == NULL) {
		cb->errors->createFatalError("CopyFile failed! Can't open file \"" + file_s1 + "\"");
		return;
	}

	fseek(file1, 0, SEEK_END);
	size = ftell(file1) * sizeof(char);
	rewind(file1);

	buffer = new (std::nothrow)char[size];
	if(buffer == NULL) {
		cb->errors->createFatalError("CopyFile failed! Memory error!");
		return;
	}

	if(fread(buffer, 1, size, file1) != size)
	{
		cb->errors->createFatalError("CopyFile failed!");
		return;
	}

	fclose(file1);

	FILE * file2 = fopen(file_s2.c_str(), "wb");
	if(file2 == NULL) {
		cb->errors->createFatalError("CopyFile failed! Can't open file \"" + file_s2 + "\"");
		return;
	}
	fwrite(buffer, 1, size, file2);
	fclose(file2);

	delete[] buffer;
}

void FileInterface::commandDeleteFile(void) {
	string file_s = cb->popValue().toString().getRef();
	if(!al_remove_filename(file_s.c_str()))
		cb->errors->createError("DeleteFile failed! File: \"" + file_s + "\"");
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

	#ifdef WIN32
		line += "\r\n";
	#else
		line += "\n";
	#endif

	fputs(line.c_str(), file);
}

void FileInterface::commandReadByte(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	fseek(file, ftell(file) + 1, SEEK_SET);
}

void FileInterface::commandReadShort(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	fseek(file, ftell(file) + 2, SEEK_SET);
}

void FileInterface::commandReadInt(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	fseek(file, ftell(file) + 4, SEEK_SET);
}

void FileInterface::commandReadFloat(void) {
	FILE *file = filestrs[cb->popValue().getInt()];
	fseek(file, ftell(file) + 4, SEEK_SET);
}

void FileInterface::commandReadString(void) {
	FILE *file = filestrs[cb->popValue().getInt()];

	int32_t i;
	fread(&i, sizeof(int32_t), 1, file);

	fseek(file, ftell(file) + i, SEEK_SET);
}

void FileInterface::commandReadLine(void) {
	FILE *file = filestrs[cb->popValue().getInt()];

	while(1) {
		int c = fgetc(file);
		if (c == '\n' || c == EOF) {
			break;
		}
	}
}

void FileInterface::functionOpenToRead(void) {
	int32_t id = ++idC;
	string file = cb->popValue().toString().getRef();

	filestrs[id] = fopen(file.c_str(), "rb");
	if (filestrs[id] == NULL) {
		cb->errors->createFatalError("OpenToRead failed! File: \"" + file + "\"");
		cb->pushValue(0);
		return;
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToWrite(void) {
	int32_t id = ++idC;
	string file = cb->popValue().toString().getRef();

	filestrs[id] = fopen(file.c_str(), "wb");
	if (filestrs[id] == NULL) {
		cb->errors->createFatalError("OpenToWrite failed! File: \"" + file + "\"");
		cb->pushValue(0);
		return;
	}

	cb->pushValue(id);
}

void FileInterface::functionOpenToEdit(void) {
	string file = cb->popValue().toString().getRef();
	int32_t id = ++idC;

	if(al_filename_exists(file.c_str()))
	{
		filestrs[id] = fopen(file.c_str(), "rb+");
	} else {
		filestrs[id] = fopen(file.c_str(), "wb+");
	}
	if (filestrs[id] == NULL) {
		cb->errors->createFatalError("OpenToEdit failed! File: \"" + file + "\"");
		cb->pushValue(0);
		return;
	}
	cb->pushValue(id);
}

void FileInterface::functionFileOffset(void) {
	cb->pushValue(int32_t(ftell(filestrs[cb->popValue().getInt()])));
}

void FileInterface::functionFindFile(void) {

	++rcount;
	if (rcount == 1) {
		ALLEGRO_PATH * path;
		path = al_create_path(al_get_fs_entry_name(cur_dir));
		if(al_get_path_num_components(path) >= 2) {
			cb->pushValue(string("."));
			al_destroy_path(path);
			return;
		}
		else {
			al_destroy_path(path);
			functionFindFile();
		}
	}
	else if (rcount == 2) {
		ALLEGRO_PATH * path;
		path = al_create_path(al_get_fs_entry_name(cur_dir));
		if(al_get_path_num_components(path) >= 2) {
			cb->pushValue(string(".."));
			al_destroy_path(path);
			return;
		}
		else {
			al_destroy_path(path);
			functionFindFile();
		}
	}

	ALLEGRO_FS_ENTRY * file = al_read_directory(cur_dir);

	if(file == NULL) {
		cb->pushValue(string(""));
		return;
	}

	string file_s;
	ALLEGRO_PATH * path = al_create_path(al_get_fs_entry_name(file));

	if(al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR) {
		file_s = string(al_get_path_tail(path));
	}
	else {
		file_s = string(al_get_path_filename(path));
	}
	cb->pushValue(file_s);

	al_destroy_path(path);
	al_destroy_fs_entry(file);

}

void FileInterface::functionCurrentDir(void) {
	char * dir = al_get_current_directory();

	string dir_s = string(dir) + "\\";
	al_free(dir);

	cb->pushValue(dir_s);
}

void FileInterface::functionFileExists(void) {
	ALLEGRO_PATH *filePath = al_create_path(cb->popValue().toString().getRef().c_str());
	cb->pushValue(al_filename_exists(al_path_cstr(filePath, ALLEGRO_NATIVE_PATH_SEP)));
	al_destroy_path(filePath);
}

void FileInterface::functionIsDirectory(void) {
	ALLEGRO_PATH *filePath = al_create_path(cb->popValue().toString().getRef().c_str());
	ALLEGRO_FS_ENTRY * file = al_create_fs_entry(al_path_cstr(filePath, ALLEGRO_NATIVE_PATH_SEP));
	cb->pushValue(bool(al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR));
	al_destroy_fs_entry(file);
	al_destroy_path(filePath);
}

void FileInterface::functionFileSize(void) {
	ALLEGRO_FS_ENTRY * file = al_create_fs_entry(cb->popValue().toString().getRef().c_str());
	cb->pushValue(int32_t(al_get_fs_entry_size(file)));
	al_destroy_fs_entry(file);
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
		if (c != '\r' && c != EOF) {
			if (c != '\n') {
				line = line + char(c);
			}
		}
		else {
			break;
		}
	}
	cb->pushValue(line);
}

/** Gets a FILE pointer from the given ID. */
FILE* FileInterface::getFile(int32_t id) {
	if (filestrs.count(id) == 0) {
		cb->errors->createFatalError("Could not find open file with ID " + boost::lexical_cast<string>(id) + "!");
		return 0;
	}

	return filestrs[id];
}

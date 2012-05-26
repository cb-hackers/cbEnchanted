#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

#include "precomp.h"

class FileInterface {
	public:
		FileInterface();
		~FileInterface();

		void commandCloseFile(void);
		void commandSeekFile(void);
		void commandStartSearch(void);
		void commandEndSearch(void);
		void commandChDir(void);
		void commandMakeDir(void);
		void commandCopyFile(void);
		void commandDeleteFile(void);
		void commandExecute(void);
		void commandWriteByte(void);
		void commandWriteShort(void);
		void commandWriteInt(void);
		void commandWriteFloat(void);
		void commandWriteString(void);
		void commandWriteLine(void);
		void commandReadByte(void);
		void commandReadShort(void);
		void commandReadInt(void);
		void commandReadFloat(void);
		void commandReadString(void);
		void commandReadLine(void);

		void functionOpenToRead(void);
		void functionOpenToWrite(void);
		void functionOpenToEdit(void);
		void functionFileOffset(void);
		void functionFindFile(void);
		void functionCurrentDir(void);
		void functionFileExists(void);
		void functionIsDirectory(void);
		void functionFileSize(void);
		void functionEOF(void);
		void functionReadByte(void);
		void functionReadShort(void);
		void functionReadInt(void);
		void functionReadFloat(void);
		void functionReadString(void);
		void functionReadLine(void);
	private:
		CBEnchanted *cb;

		int32_t rcount;
		int32_t idC;
		map <int32_t, FILE*> filestrs;

		ALLEGRO_FS_ENTRY * cur_dir;
};

#endif

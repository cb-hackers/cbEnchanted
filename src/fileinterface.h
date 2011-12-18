#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

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
	private:
	
};

#endif

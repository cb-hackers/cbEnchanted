#ifndef CBSOUND_H
#define CBSOUND_H
#include "precomp.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

class CBSound {
	public:
		CBSound();
		~CBSound();
		bool loadSound(string file);
		ALLEGRO_SAMPLE* getSample();
		void freeSound();
		string getSourceFile() const { return sourceFile; }
		int32_t getFrequency() const{ return frequency;}
	private:
		int32_t frequency;
		string sourceFile;
		ALLEGRO_SAMPLE* sample;
};

#endif // CBSOUND_H

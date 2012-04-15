#ifndef CBSOUND_H
#define CBSOUND_H
#include "precomp.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

class CBSound
{
	public:
		CBSound();
		void loadSound(string file);
		void setSound(bool loop, uint8_t gain, int8_t pan, int32_t freq);
		uint8_t getVolume();
		int8_t getBalance();
		uint32_t getFreq();
		bool isLooping();
		float getFreqScale();

	private:

		ALLEGRO_SAMPLE* sample;
		ALLEGRO_SAMPLE* getSample();
		uint8_t volume;
		int8_t balance;
		uint32_t frequency;
		bool looping;

};

#endif // CBSOUND_H

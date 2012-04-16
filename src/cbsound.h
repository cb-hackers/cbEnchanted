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
		void setSound(bool loop, uint8_t vol, int8_t pan, int32_t freq=-1);
		float getGain();
		float getBalance();
		uint32_t getFreq();
		bool isLooping();
		float getFreqScale();

	private:

		ALLEGRO_SAMPLE* sample;
		ALLEGRO_SAMPLE* getSample();
		float gain;
		float balance;
		uint32_t frequency;
		bool looping;

};

#endif // CBSOUND_H

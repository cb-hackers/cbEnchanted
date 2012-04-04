#ifndef CBAUDIO_H
#define CBAUDIO_H
#include "precomp.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>


class CBAudio
{
	public:
		CBAudio();
		virtual void playSound(uint8_t volume, int8_t balance, int32_t freq) = 0;
		virtual bool loadSound(std::string file)=0;
		virtual void setSound(bool looping, uint8_t volume, int8_t balance, int32_t freq)=0;
		virtual void stopSound()=0;
		virtual bool soundPlaying()=0;

};

#endif // CBAUDIO_H

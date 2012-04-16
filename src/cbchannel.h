#ifndef CBCHANNEL_H
#define CBCHANNEL_H
#include "cbsound.h"
#include "cbenchanted.h"
class CBChannel
{
	public:
		CBChannel();
		void playSound(string str, uint8_t volume=100, int8_t pan=0, int32_t freq=-1);
		void playSound(CBSound& sound, uint8_t volume=100, int8_t pan=0, int32_t freq=-1);
		bool isPlaying();
		void stopSound();
	private:
		enum playTypes{
			soundType = 1,
			streamType = 2
		};

		ALLEGRO_AUDIO_STREAM* flow;
		ALLEGRO_SAMPLE_INSTANCE* instance;
		uint32_t bufferCount;
		uint32_t sampleCount;
		playTypes playtype;
		CBEnchanted *cb;
};

#endif // CBCHANNEL_H

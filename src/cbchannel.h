#ifndef CBCHANNEL_H
#define CBCHANNEL_H
#include "cbsound.h"

class CBChannel
{
	public:
		CBChannel();
		~CBChannel();
		inline void setMixer(ALLEGRO_MIXER* mix) {mixer = mix;}
		void playSound(string str, float volume=100, float pan=0, int32_t freq=-1);
		void playSound(CBSound& sound, float volume=100, float pan=0, int32_t freq=-1);
		bool isPlaying();
		void stopSound();

	private:
		enum playTypes{
			soundType = 1,
			streamType = 2
		};

		ALLEGRO_AUDIO_STREAM* flow;
		ALLEGRO_SAMPLE_INSTANCE* instance;
		ALLEGRO_MIXER* mixer;
		uint32_t bufferCount;
		uint32_t sampleCount;
		playTypes playtype;

};

#endif // CBCHANNEL_H

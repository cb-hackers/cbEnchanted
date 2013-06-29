#ifndef CBCHANNEL_H
#define CBCHANNEL_H

#include "cbsound.h"

class CBChannel {
	public:
		CBChannel();
		~CBChannel();
		inline void setMixer(ALLEGRO_MIXER *mix) { mixer = mix; }
		bool playSound(string file, float volume = 100, float pan = 0, int32_t freq = -1);
		bool playSound(CBSound& sound, float volume = 100, float pan = 0, int32_t freq = -1);
		void setSound(bool loop, float vol, float pan, int32_t freq = -1);
		void freeChannel();
		bool isPlaying();
		void stop();
	private:
		enum ChannelType{
			Sound,
			Stream
		};
		union {
			ALLEGRO_AUDIO_STREAM *stream;
			ALLEGRO_SAMPLE_INSTANCE *instance;
		};
		ALLEGRO_MIXER *mixer;
		uint32_t bufferCount;
		uint32_t sampleCount;
		ChannelType playType;
};

#endif // CBCHANNEL_H


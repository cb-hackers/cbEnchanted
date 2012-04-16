#ifndef SOUNDINTERFACE_H
#define SOUNDINTERFACE_H

#include "precomp.h"
#include <set>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

class CBEnchanted;

class SoundInterface {
	public:
		SoundInterface();
		~SoundInterface();

		void commandPlaySound(void);
		void commandSetSound(void);
		void commandStopSound(void);
		void commandDeleteSound(void);

		void functionLoadSound(void);
		void functionPlaySound(void);
		void functionSoundPlaying(void);

		bool initializeSounds();
		void updateAudio(void);
		void cleanupSoundInterface(void);
		inline ALLEGRO_MIXER* getMixer() {return cbMixer;}
	private:
		int32_t nextId(void);
		int32_t idCounter;
		CBEnchanted *cb;
		ALLEGRO_MIXER* cbMixer;
};

#endif

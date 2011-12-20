#ifndef SOUNDINTERFACE_H
#define SOUNDINTERFACE_H

#include <SFML/Audio.hpp>
#include "precomp.h"
#include <set>

class CBEnchanted;
struct CBSound {
	union{
		sf::Sound *sound;
		sf::Music *music;
	};
	sf::SoundBuffer *soundBuffer;
	//Is sound played directly from file
	bool isMusic;
	string file;
};

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


		void updateAudio();
	private:
		CBEnchanted *cb;

		//List of music what should be deleted when it stops.
		set<CBSound*> deleteWhenStop;
};

#endif

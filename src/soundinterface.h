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
	int32_t id;
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


		void updateAudio(void);
		void cleanupSoundInterface(void);
	private:
		int32_t nextId(void);
		int32_t idCounter;
		CBEnchanted *cb;

		//List of music what should be deleted when it stops.
		set<CBSound*> deleteWhenStop;
		map<int32_t,CBSound*> cbSounds;
};

#endif

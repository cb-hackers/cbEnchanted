#include "precomp.h"
#include "soundinterface.h"
#include "cbenchanted.h"
#include "any.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

SoundInterface::SoundInterface() : cb(static_cast <CBEnchanted *> (this)),idCounter(0) {

}

SoundInterface::~SoundInterface() {

}

void SoundInterface::commandPlaySound(void) {
	/*int32_t freg = */cb->popValue().toInt();
	float balance = cb->popValue().toFloat()/100.0;
	float volume = cb->popValue().toFloat();
	Any any = cb->popValue();
	if (any.type() == Any::Int) {//Loaded sound


	}
	else {

	}
}

void SoundInterface::commandSetSound(void) {
	/*int32_t freg = */cb->popValue().toInt();
	float balance = cb->popValue().toFloat() / 100.0;
	float volume = cb->popValue().toFloat();
	bool looping = cb->popValue().toInt();
}

void SoundInterface::commandStopSound(void) {
	int32_t id = cb->popValue().getInt();

}

void SoundInterface::commandDeleteSound(void) {
	int32_t id = cb->popValue().getInt();
}

void SoundInterface::functionLoadSound(void) {
	string filepath = cb->popValue().toString().getRef();
}

void SoundInterface::functionPlaySound(void) {
	/*int32_t freg = */cb->popValue().toInt();
	float balance = cb->popValue().toFloat();
	float volume = cb->popValue().toFloat();
	Any any = cb->popValue();
	if (any.type() == Any::Int) {//Loaded sound

	}
	else {

	}

}

void SoundInterface::functionSoundPlaying(void) {
	int32_t id = cb->popValue().getInt();
}

bool SoundInterface::initializeSounds()
{
	if (!al_install_audio()) return false;
	if (!al_init_acodec_addon()) return false;
	return true;
}

void SoundInterface::updateAudio(void) {

}

//Deletes all sounds.
void SoundInterface::cleanupSoundInterface() {
/*	for (map<int32_t, CBSound*>::iterator i = cbSounds.begin(); i != cbSounds.end(); i++) {
		delete i->second;
	}*/
}

int32_t SoundInterface::nextId() {
	return ++idCounter;
}

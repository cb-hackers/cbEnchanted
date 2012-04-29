#include "precomp.h"
#include "soundinterface.h"
#include "cbenchanted.h"
#include "any.h"
#include "errorsystem.h"


SoundInterface::SoundInterface() {
	cb = static_cast <CBEnchanted *> (this);
}

SoundInterface::~SoundInterface() {

}

void SoundInterface::functionPlaySound(void) {
	int32_t freg = cb->popValue().toInt();
	float balance = cb->popValue().toFloat();
	float volume = cb->popValue().toFloat();
	Any any = cb->popValue();
	if (any.type() == Any::Int) {//Loaded sound
		CBChannel* channel = new CBChannel;
		int32_t id = any.toInt();
		channel->setMixer(al_get_default_mixer());
		channel->playSound((*sounds[id]), volume, balance, freg);
		int32_t nextChannel = nextChannelId();
		channels[nextChannel] = channel;
		cb->pushValue(nextChannel);
	}
	else {
		CBChannel* channel = new CBChannel;
		channel->setMixer(al_get_default_mixer());
		string file = any.toString().getRef();
		INFO(file.c_str())
		channel->playSound(file, volume, balance, freg);
		int32_t nextChannel = nextChannelId();
		channels[nextChannel] = channel;
		cb->pushValue(nextChannel);
	}
}

void SoundInterface::commandPlaySound(void) {
	int32_t freg = cb->popValue().toInt();
	float balance = cb->popValue().toFloat();
	float volume = cb->popValue().toFloat();
	Any any = cb->popValue();
	if (any.type() == Any::Int) {//Loaded sound
		CBChannel* channel = new CBChannel;
		channel->setMixer(al_get_default_mixer());
		int32_t id = any.toInt();
		channel->playSound((*sounds[id]), volume, balance, freg);
		int32_t nextChannel = nextChannelId();
		channels[nextChannel] = channel;
	}
	else {
		CBChannel* channel = new CBChannel;
		channel->setMixer(al_get_default_mixer());
		string file = any.toString().getRef();
		channel->playSound(file, volume, balance, freg);
		int32_t nextChannel = nextChannelId();
		channels[nextChannel] = channel;
	}
}

void SoundInterface::commandSetSound(void) {
	int32_t freg = cb->popValue().toInt();
	float balance = cb->popValue().toFloat();
	float volume = cb->popValue().toFloat();
	bool looping = cb->popValue().toInt();
	int32_t id = cb->popValue().toInt();
	CBSound* sound = sounds[id];
	sound->setSound(looping, volume, balance, freg);
}

void SoundInterface::commandStopSound(void) {
	int32_t id = cb->popValue().getInt();
	CBChannel* sound = channels[id];
	sound->stopSound();
}

void SoundInterface::commandDeleteSound(void) {
	int32_t id = cb->popValue().getInt();
	CBSound* sound = sounds[id];
	delete sound;
	sounds.erase(id);
}

void SoundInterface::functionLoadSound(void) {
	ALLEGRO_PATH *path = cb->popValue().getString().getPath();
	const char *cpath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	CBSound *snd = new CBSound;
	if(!snd->loadSound(cpath)){
		cb->errors->createError("LoadSound() failed!", "Failed to load file \"" + string(cpath) + "\"");
		cb->pushValue(0);
		al_destroy_path(path);
		return;
	}
	al_destroy_path(path);

	int32_t id = nextSampleId();
	sounds[id] = snd;
	cb->pushValue(id);
}


void SoundInterface::functionSoundPlaying(void) {
	int32_t id = cb->popValue().getInt();
	CBChannel *channel = channels[id];
	if (channel == NULL){
		cb->pushValue(0);
		return;
	}
	uint8_t playing = channel->isPlaying();
	cb->pushValue(playing);
}

bool SoundInterface::initializeSounds()
{
	if (!al_init_acodec_addon()) return false;
	if (!al_install_audio()) return false;
	if (!al_reserve_samples(512)) return false;
	return true;
}

void SoundInterface::updateAudio(void) {
	map<int32_t, CBChannel*>::iterator i = channels.begin();
	map<int32_t, CBChannel*>::iterator i2;
	while (i != channels.end()){
		if ((*i).second->isPlaying() == false){
			delete (*i).second;
			i2 = i;
			++i2;
			channels.erase(i);
			i = i2;
		}
		else {
			++i;
		}
	}
}

//Deletes all sounds.
void SoundInterface::cleanupSoundInterface() {
	for (map<int32_t, CBChannel*>::iterator channel = channels.begin(); channel != channels.end(); ++channel) {
		delete channel->second;
	}
	for (map<int32_t, CBSound*>::iterator sound = sounds.begin(); sound != sounds.end(); sound++) {
		delete sound->second;
	}
}


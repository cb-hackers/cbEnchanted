#include "precomp.h"
#include "soundinterface.h"
#include "cbenchanted.h"
#include "any.h"


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
	string filepath = cb->popValue().toString().getRef();
	CBSound *snd = new CBSound;
	snd->loadSound(filepath);
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

	map<int32_t, CBChannel*>::iterator i;
	i = channels.begin();
	bool last = false;
	while (i != channels.end()){
		if ((*i).second->isPlaying() == false){
			(*i).second -> freeChannel();
			if(i == channels.end())
				last = true;
			delete (*i).second;
			channels.erase(i);
		}
		if(last == true)
			break;
		i++;
	}


}

//Deletes all sounds.
void SoundInterface::cleanupSoundInterface() {
	for (map<int32_t, CBChannel*>::iterator i = channels.begin(); i != channels.end(); i++) {
		if ((*i).second->isPlaying() == false){
			(*i).second->freeChannel();
			delete (*i).second;
			INFO("Olio tuhottu")
			channels.erase(i);
			INFO("Listasta tyhjennetty.")
		}
	}
	for (map<int32_t, CBSound*>::iterator sound = sounds.begin(); sound != sounds.end(); sound++) {
		sound->second->freeSound();
		delete sound->second;
		sounds.erase(sound);
	}
}


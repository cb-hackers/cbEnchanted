#include "cbchannel.h"
#include "debug.h"
#include <boost/lexical_cast.hpp>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <string>
#include "cbenchanted.h"
#include "errorsystem.h"

CBChannel::CBChannel(): bufferCount(3), sampleCount(4000), flow(NULL), instance(NULL)
{

}

CBChannel::~CBChannel() {
	if (playtype == soundType) {
		al_destroy_sample_instance(instance);
	}
	else {
		al_destroy_audio_stream(flow);
	}
}

bool CBChannel::playSound(CBSound &sound, float volume, float pan, int32_t freq) {

	instance = al_create_sample_instance(sound.getSample());
	if (!al_attach_sample_instance_to_mixer(instance, mixer)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not attach sample instance to mixer.\nSource file: " + sound.getSourceFile());
		return false;
	}
	playtype = soundType;

	if (volume > 99.9999f) {
		if (!al_set_sample_instance_gain(instance, sound.getGain())) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance gain.\nSource file: " + sound.getSourceFile());
			return false;
		}
	}
	else {
		float newGain = float(volume) / 100.0f;
		if (!al_set_sample_instance_gain(instance, newGain)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance gain to " + boost::lexical_cast<string>(newGain) + ".\nSource file: " + sound.getSourceFile());
			return false;
		}
	}

	if (pan > -1e-5f && pan < 1e-5f) {
		if (!al_set_sample_instance_pan(instance, sound.getBalance())) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance pan to " + boost::lexical_cast<string>(pan) + ".\nSource file: " + sound.getSourceFile());
			return false;
		}
	}
	else {
		float newPan = float(pan) / 100.0f;
		if (!al_set_sample_instance_pan(instance, newPan)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance pan to " + boost::lexical_cast<string>(newPan) + ".\nSource file: " + sound.getSourceFile());
			return false;
		}
	}


	if (freq > 0.0f) {
		float newFreq = freq / float(sound.getFreq());
		if (!al_set_sample_instance_speed(instance, newFreq)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance frequency to " + boost::lexical_cast<string>(freq) + ".\nSource file: " + sound.getSourceFile());
			return false;
		}
	}


	if (sound.isLooping()) {
		if(!al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance frequency.\nSource file: " + sound.getSourceFile());
			return false;
		}
	}

	if (!al_play_sample_instance(instance)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not play sample instance.\nSource file: " + sound.getSourceFile());
		return false;
	}

	return true;
}

bool CBChannel::playSound(string file, float volume, float pan, int32_t freq) {
	flow = al_load_audio_stream(file.c_str(), bufferCount, sampleCount);
	if (!flow) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not load audio stream.\nSource file: " + file);
		return false;
	}
	if (!al_attach_audio_stream_to_mixer(flow, mixer)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not attach audio stream to mixer.\nSource file: " + file);
		return false;
	}
	playtype = streamType;

	float streamGain = al_get_audio_stream_gain(flow);
	uint32_t frequency = al_get_audio_stream_frequency(flow);

	if (volume > 99.999f) {
		if(!al_set_audio_stream_gain(flow, streamGain)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream gain.\nSource file: " + file);
			return false;
		}
	}
	else {
		if(!al_set_audio_stream_gain(flow, volume / 100.0 * streamGain)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream gain.\nSource file: " + file);
			return false;
		}
	}

	if (!al_set_audio_stream_pan(flow, pan / 100.0f)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream pan.\nSource file: " + file);
		return false;
	}

	if (freq != frequency && freq > 0) {
		float speed = float(freq) / float(frequency);
		if (!al_set_audio_stream_speed(flow, speed)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream frequency to " + boost::lexical_cast<string>(freq) + ".\nSource file: " + file);
			return false;
		}
	}
	else {
		if(!al_set_audio_stream_speed(flow, 1.0)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream frequency.\nSource file: " + file);
			return false;
		}
	}

	if(!al_set_audio_stream_playing(flow, true)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not start playing audio stream.\nSource file: " + file);
		return false;
	}

	return true;
}

bool CBChannel::isPlaying() {
	switch (playtype) {
		case soundType:
			return al_get_sample_instance_playing(instance);
		break;
		case streamType:
			return al_get_audio_stream_playing(flow);
		break;
	}
	return 0;
}

void CBChannel::freeChannel() {
	if (instance != NULL) {
		//al_detach_sample_instance(instance);
		al_destroy_sample_instance(instance);
	}
	if (flow != NULL) {
		//al_detach_audio_stream(flow);
		al_destroy_audio_stream(flow);
	}
}

void CBChannel::stopSound() {
	switch (playtype) {
		case soundType:
			al_set_sample_instance_playing(instance, false);
		break;
		case streamType:
			al_set_audio_stream_playing(flow, false);
		break;
	}
}

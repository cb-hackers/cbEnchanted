#include "cbchannel.h"
#include "debug.h"
#include <boost/lexical_cast.hpp>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <string>
#include "cbenchanted.h"
#include "errorsystem.h"

CBChannel::CBChannel(): bufferCount(3), sampleCount(8192), stream(NULL) {

}

CBChannel::~CBChannel() {
	if (playType == Sound) {
		if (instance) {
			al_destroy_sample_instance(instance);
		}
	}
	else {
		if (stream) {
			al_destroy_audio_stream(stream);
		}
	}
}

bool CBChannel::playSound(CBSound &sound, float volume, float pan, int32_t freq) {
	instance = al_create_sample_instance(sound.getSample());
	if (!al_attach_sample_instance_to_mixer(instance, mixer)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not attach sample instance to mixer.\nSource file: " + sound.getSourceFile());
		return false;
	}
	playType = Sound;

	float newGain = volume / 100.0f;
	if (!al_set_sample_instance_gain(instance, newGain)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance gain to " + boost::lexical_cast<string>(newGain) + ".\nSource file: " + sound.getSourceFile());
		return false;
	}

	float newPan = pan / 100.0f;
	if (!al_set_sample_instance_pan(instance, newPan)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance pan to " + boost::lexical_cast<string>(newPan) + ".\nSource file: " + sound.getSourceFile());
		return false;
	}


	if (freq > 0) {
		float newFreq = freq / float(sound.getFrequency());
		if (!al_set_sample_instance_speed(instance, newFreq)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set sample instance frequency to " + boost::lexical_cast<string>(freq) + ".\nSource file: " + sound.getSourceFile());
			return false;
		}
	}

	if (!al_play_sample_instance(instance)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not play sample instance.\nSource file: " + sound.getSourceFile());
		return false;
	}

	return true;
}

void CBChannel::setSound(bool loop, float vol, float pan, int32_t freq) {
	if (playType == Sound) {
		if (freq > 0) {
			float speed = double(freq) / double(al_get_sample_instance_frequency(instance));
			if (!al_set_sample_instance_speed(instance, speed)) {
				CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance frequency to " + boost::lexical_cast<string>(freq));
				return;
			}
		}
		float newGain = vol / 100.0f;
		if (!al_set_sample_instance_gain(instance, newGain)) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance gain to " + boost::lexical_cast<string>(newGain));
			return;
		}

		float newPan = pan / 100.0f;
		if (!al_set_sample_instance_pan(instance, newPan)) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance pan to " + boost::lexical_cast<string>(newPan));
			return;
		}
		bool success = false;
		if (loop) {
			success = al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
		}
		else {
			success = al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_ONCE);
		}
		if (!success) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", string("Could not set sample looping to '") + (loop ? "'true'" : "'false'"));
			return;
		}
	}
	else {
		if (freq > 0) {
			float speed = double(freq) / double(al_get_sample_instance_frequency(instance));
			if (!al_set_audio_stream_speed(stream, speed)) {
				CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance frequency to " + boost::lexical_cast<string>(freq));
				return;
			}
		}
		float newGain = vol / 100.0f;
		if (!al_set_audio_stream_gain(stream, newGain)) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance gain to " + boost::lexical_cast<string>(newGain));
			return;
		}

		float newPan = pan / 100.0f;
		if (!al_set_audio_stream_pan(stream, newPan)) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", "Could not set sample instance pan to " + boost::lexical_cast<string>(newPan));
			return;
		}
		bool success = false;
		if (loop) {
			success = al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
		}
		else {
			success = al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_ONCE);
		}
		if (!success) {
			CBEnchanted::instance()->errors->createError("SetSound() failed!", string("Could not set sample looping to '") + (loop ? "'true'" : "'false'"));
			return;
		}
	}
}

bool CBChannel::playSound(string file, float volume, float pan, int32_t freq) {
	stream = al_load_audio_stream(file.c_str(), bufferCount, sampleCount);
	if (!stream) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not load audio stream.\nSource file: " + file);
		return false;
	}
	if (!al_attach_audio_stream_to_mixer(stream, mixer)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not attach audio stream to mixer.\nSource file: " + file);
		return false;
	}
	playType = Stream;

	float streamGain = al_get_audio_stream_gain(stream);
	uint32_t oldFreq = al_get_audio_stream_frequency(stream);

	if(!al_set_audio_stream_gain(stream, volume / 100.0 * streamGain)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream gain.\nSource file: " + file);
		return false;
	}


	if (!al_set_audio_stream_pan(stream, pan / 100.0f)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream pan.\nSource file: " + file);
		return false;
	}
	if (freq > 0) {
		float speed = double(freq) / double(oldFreq);
		if (!al_set_audio_stream_speed(stream, speed)) {
			CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not set audio stream frequency to " + boost::lexical_cast<string>(freq) + ".\nSource file: " + file);
			return false;
		}
	}

	if(!al_set_audio_stream_playing(stream, true)) {
		CBEnchanted::instance()->errors->createError("PlaySound() failed!", "Could not start playing audio stream.\nSource file: " + file);
		return false;
	}

	return true;
}

bool CBChannel::isPlaying() {
	switch (playType) {
		case Sound:
			return al_get_sample_instance_playing(instance);
			break;
		case Stream:
			return al_get_audio_stream_playing(stream);
			break;
	}
	return 0;
}


void CBChannel::stop() {
	switch (playType) {
		case Sound:
			al_set_sample_instance_playing(instance, false);
			break;
		case Stream:
			al_set_audio_stream_playing(stream, false);
			break;
	}
}

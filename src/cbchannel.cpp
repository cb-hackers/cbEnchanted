#include "cbchannel.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

CBChannel::CBChannel(): playing(false), bufferCount(32), sampleCount(4)
{

}

void CBChannel::playSound(CBSound &sound,  uint8_t volume, int8_t pan, uint32_t freq) {

	instance = al_create_sample_instance(sound.getSample());
	al_attach_sample_instance_to_mixer(instance, mixer);
	playtype = soundType;

	if (volume==100) {
		al_set_sample_instance_gain(instance,  sound.getGain());
	}
	else {
		float newGain = volume / 100.0 * sound.getGain();
		al_set_sample_instance_gain(instance, newGain);
	}


	if (pan==100) {
		al_set_sample_instance_pan(instance, sound.getBalance());
	}
	else {
		float newPan = pan / 100.0;
		al_set_sample_instance_pan(instance, newPan);
	}


	if (freq>=0) {
		float newFreq = 1.0 / freq;
		al_set_sample_instance_pan(instance, newFreq);
	}
	else {
		al_set_sample_instance_pan(instance, sound.getFreqScale());
	}


	if (sound.isLooping())
		al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);

	al_play_sample_instance(instance);
}

void CBChannel::playSound(string str,  uint8_t volume, int8_t pan, uint32_t freq) {

	flow = al_load_audio_stream(str, bufferCount, sampleCount);
	al_attach_audio_stream_to_mixer(flow, mixer);
	playtype = streamType;

	float streamGain = al_get_audio_stream_gain(flow);
	uint32_t frequency = al_get_audio_stream_frequency(flow);

	if (volume==100) {
		al_set_audio_stream_gain(flow, streamGain);
	}
	else {
		al_set_audio_stream_gain(flow, volume / 100.0 * streamGain);
	}

	if (pan != 0){
		al_set_audio_stream_pan(flow, 0.0);
	}
	else {
		al_set_audio_stream_pan(flow, pan / 100.0);
	}

	if (freq >= 0) {
		float speed = float(freq) / frequency;
		al_set_audio_stream_speed(flow, speed);
	}
	else {
		al_set_audio_stream_speed(flow, 1.0);
	}
	al_set_audio_stream_playing(flow, true);
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
}

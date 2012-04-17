#include "cbsound.h"

CBSound::CBSound(): looping(false), gain(1.0), balance(0.0)
{
}


void CBSound::loadSound(string file) {
	sample = al_load_sample(file.c_str());
	frequency = al_get_sample_frequency(sample);
}


void CBSound::setSound(bool loop, uint8_t vol, int8_t pan, int32_t freq) {
	looping = loop;
	gain = vol / 100.0 * getGain();
	balance = pan / 100.0;
	if(freq>=0)
		frequency = freq;
}

float CBSound::getGain() {
	return gain;
}

float CBSound::getBalance() {
	return balance;
}

uint32_t CBSound::getFreq() {
	return frequency;
}

float CBSound::getFreqScale() {
	return 1.0 / frequency;
}

bool CBSound::isLooping() {
	return looping;
}

ALLEGRO_SAMPLE* CBSound::getSample() {
	return sample;
}

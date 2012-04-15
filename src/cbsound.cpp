#include "cbsound.h"

CBSound::CBSound(): looping(false), volume(100), balance(0)
{
}


void CBSound::loadSound(string file) {
	sample = al_load_sample(file.c_str());
	frequency = al_get_sample_frequency(sample);
}


void CBSound::setSound(bool loop, uint8_t gain, int8_t pan, int32_t freq) {
	looping = loop;
	volume = gain;
	balance = pan;
	frequency = freq;
}

uint8_t CBSound::getVolume() {
	return volume;
}

int8_t CBSound::getBalance() {
	return balance;
}

uint32_t CBSound::getFreq() {
	return frequency;
}

float CBSound::getFreqScale() {
	return 1.0 / frequency;
}

ALLEGRO_SAMPLE* CBSound::getSample() {
	return sample;
}

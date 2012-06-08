#include "cbsound.h"

CBSound::CBSound(): sample(0) {
}

CBSound::~CBSound() {
	if (sample) {
		al_destroy_sample(sample);
	}
}


bool CBSound::loadSound(string file) {
	sourceFile = file;
	sample = al_load_sample(file.c_str());
	if (sample == 0) {
		return false;
	}
	frequency = al_get_sample_frequency(sample);
	return true;
}

ALLEGRO_SAMPLE* CBSound::getSample() {
	return sample;
}

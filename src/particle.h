#ifndef PARTICLE_H
#define PARTICLE_H
#include <stdint.h>

struct Particle {
	int32_t lifeTime;
	float x,y;
	float velX,velY;
};

#endif // PARTICLE_H

#ifndef PARTICLE_H
#define PARTICLE_H
#include <stdint.h>

/** Represents values connected to a single particle */
struct Particle {
	/** How many frames does the particle remain undeleted */
	int32_t lifeTime;
	/** Coordinates in world coordinates */
	float x,y;
	/** Velocity */
	float velX,velY;
};

#endif // PARTICLE_H

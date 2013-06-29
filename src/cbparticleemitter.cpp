#include "cbparticleemitter.h"
#include "util.h"

CBParticleEmitter::CBParticleEmitter():
	CBObject(),
	particleLifeTime(0),
	particleSpeed(0),
	particleGravity(0),
	particleAcceleration(1.0f),
	particleDensity(1),
	particleSpread(0),
	particleCount(1),
	particleSpawnCounter(0),
	frameCount(0),
	stop(false)
{
}

CBParticleEmitter::~CBParticleEmitter() {

}

void CBParticleEmitter::create(CBImage *image, int32_t lifeTime) {
	particleTexture = image;
	particleLifeTime = lifeTime;
}

void CBParticleEmitter::setParticleMovement(float speed, float gravity, float accel) {
	particleSpeed = speed;
	particleGravity = gravity;
	particleAcceleration = accel;
}

void CBParticleEmitter::setParticleEmission(float density, int count, float spread) {
	particleDensity = density;
	particleCount = count;
	particleSpread = spread;
}

bool CBParticleEmitter::updateObject(float timestep) {
	particleSpawnCounter += 1.0f;
	for (vector<Particle>::iterator i = particles.begin(); i != particles.end();) {
		i->x += i->velX;
		i->y += i->velY;
		i->velX = i->velX * particleAcceleration;
		i->velY = i->velY * particleAcceleration - particleGravity;
		i->lifeTime--;
		if (i->lifeTime < 0) {
			i = particles.erase(i);
		}
		else {
			++i;
		}
	}
	if (stop) {
		if (particles.size() == 0) return true;
	}
	else {
		if (usingLife) {
			--life;
			if (life <= 0) {
				life = 0;
				return true; //Delete object
			}
		}
		while (particleSpawnCounter > particleDensity) { //Spawn particles
			particleSpawnCounter -= particleDensity;
			for (int32_t i = 0; i < particleCount; ++i) {
				Particle newParticle;
				newParticle.lifeTime = particleLifeTime;
				float pa = (angle + particleSpread - randf() * particleSpread * 2.0f) * M_PI / 180.0f;
				newParticle.velX = cos(pa) * particleSpeed;
				newParticle.velY = sin(pa) * particleSpeed;
				newParticle.x = posX + newParticle.velX * particleSpawnCounter;
				newParticle.y = posY + newParticle.velY * particleSpawnCounter;
				particles.push_back(newParticle);
			}
		}
	}
	return false;
}

void CBParticleEmitter::render(RenderTarget &target) {
	target.drawParticles(particleTexture, particles, particleLifeTime, frameCount);
}

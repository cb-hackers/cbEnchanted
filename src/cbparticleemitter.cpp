#include "cbparticleemitter.h"
#include "util.h"

CBParticleEmitter::CBParticleEmitter():
	particleLifeTime(0),
	particleSpeed(0),
	particleGravity(0),
	particleAcceleration(1.0f),
	particleDensity(1),
	particleSpread(0),
	particleCount(1),
	particleSpawnCounter(0)
{
}

CBParticleEmitter::~CBParticleEmitter() {

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
	for (vector<Particle>::iterator i = particles.begin();i != particles.end();++i)
	{
		i->x += i->velX;
		i->y += i->velY;
		i->velX *= particleAcceleration;
		i->velY = i->velY *particleAcceleration + particleGravity;
	}
	if (usingLife) {
		--life;
		if (life <= 0) {
			life = 0;
			return true; //Delete object
		}
	}
	while (particleSpawnCounter > particleDensity) {//Spawn particles
		particleSpawnCounter -= particleDensity;
		Particle newParticle;
		newParticle.lifeTime = particleLifeTime;
		float pa = angle + particleSpread*0.5f-randf()*particleSpread;
		newParticle.velX = randf()*cos(pa)*particleSpeed;
		newParticle.velY = randf()*sin(pa)*particleSpeed;
		newParticle.x = posX + newParticle.velX*particleSpawnCounter;
		newParticle.y = posY + newParticle.velY*particleSpawnCounter;
		particles.push_back(newParticle);
	}
	return false;
}

void CBParticleEmitter::render(RenderTarget &target) {
	target.drawParticles(particleTexture,particles,particleLifeTime);
}

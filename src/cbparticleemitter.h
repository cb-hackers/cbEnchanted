#ifndef CBPARTICLEEMITTER_H
#define CBPARTICLEEMITTER_H
#include "cbobject.h"
#include "cbimage.h"

class CBParticleEmitter : public CBObject
{
	public:
		CBParticleEmitter();
		~CBParticleEmitter();
		void create(CBImage *image,int32_t lifeTime);
		void setParticleMovement(float speed, float gravity,float accel);
		void setParticleEmission(float density,int count,float spread);
		void setParticleAnimation(int32_t frames){frameCount = frames;}
		void render(RenderTarget &target);
		bool updateObject(float timestep);
		Type type()const{return ParticleEmitter;}
	private:
		int32_t particleLifeTime;
		float particleSpeed, particleGravity, particleAcceleration;
		float particleDensity, particleSpread;
		int32_t particleCount;
		float particleSpawnCounter;
		CBImage *particleTexture;
		vector<Particle> particles;
		int32_t frameCount;
};

#endif // CBPARTICLEEMITTER_H

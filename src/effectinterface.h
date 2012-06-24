#ifndef EFFECTINTERFACE_H
#define EFFECTINTERFACE_H
#include "precomp.h"
#include "cbobject.h"
#include "cbparticleemitter.h"

class CBEnchanted;

class EffectInterface {
	public:
#ifndef CBE_LIB
		EffectInterface();
		~EffectInterface();
		void commandParticleMovement(void);
		void commandParticleAnimation(void);
		void commandParticleEmission(void);
		void functionMakeEmitter(void);
#endif
		void updateRogueParticles(void);
		void deleteParticleEmitter(CBParticleEmitter *p);
	private:
		CBEnchanted *cb;
		vector<CBParticleEmitter*> rogueEmitters;
};

#endif

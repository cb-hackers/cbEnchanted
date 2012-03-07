#ifndef EFFECTINTERFACE_H
#define EFFECTINTERFACE_H
#include "precomp.h"
#include "cbobject.h"



class EffectInterface {
	public:
		EffectInterface();
		~EffectInterface();
		
		void commandParticleMovement(void);
		void commandParticleAnimation(void);
		void commandParticleEmission(void);
		
		void functionMakeEmitter(void);
	private:

};

#endif

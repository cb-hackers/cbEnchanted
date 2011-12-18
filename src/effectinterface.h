#ifndef EFFECTINTERFACE_H
#define EFFECTINTERFACE_H

class EffectInterface {
	public:
		EffectInterface();
		~EffectInterface();
		
		void commandParticleMovement(void);
		void commandParticleAnimation(void);
		void commandParticleEmission(void);
	private:
	
};

#endif

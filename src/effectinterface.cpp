#include "precomp.h"
#include "effectinterface.h"
#include "debug.h"
#include "cbenchanted.h"
#include "cbimage.h"
#include "cbparticleemitter.h"
#include "objectinterface.h"
#include "imageinterface.h"

#ifndef CBE_LIB
EffectInterface::EffectInterface() {
	cb = CBEnchanted::instance();
}

EffectInterface::~EffectInterface() {

}

void EffectInterface::commandParticleMovement(void) {
	float acc = cb->popValue().toFloat();
	float gravity = cb->popValue().toFloat();
	float speed = cb->popValue().toFloat();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->objectInterface->getObject(emitterId));
	e->setParticleMovement(speed, gravity, acc);
}

void EffectInterface::commandParticleAnimation(void) {
	int32_t frames = cb->popValue().toInt();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->objectInterface->getObject(emitterId));
	e->setParticleAnimation(frames);
}

void EffectInterface::commandParticleEmission(void) {
	float spread = cb->popValue().toFloat();
	float count = cb->popValue().toFloat();
	float density = cb->popValue().toFloat();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->objectInterface->getObject(emitterId));
	e->setParticleEmission(density, count, spread);
}

void EffectInterface::functionMakeEmitter(void) {
	int32_t lifeTime = cb->popValue().toInt();
	int32_t imageId = cb->popValue().toInt();
	CBImage *image = cb->imageInterface->getImage(imageId);
	CBParticleEmitter *emitter = new CBParticleEmitter;
	emitter->create(image, lifeTime);
	int32_t id = cb->objectInterface->addObject(emitter);
	cb->objectInterface->addToDrawOrder(emitter);
	cb->pushValue(id);
}
#endif

void EffectInterface::deleteParticleEmitter(CBParticleEmitter *p)
{
	p->stopEmitting();
	rogueEmitters.push_back(p);
}

void EffectInterface::updateRogueParticles(void)
{
	for (vector<CBParticleEmitter*>::iterator i = rogueEmitters.begin(); i != rogueEmitters.end();) {
		if ((*i)->updateObject(0)) { //updateObject returns true if object should be deleted
			cb->objectInterface->removeFromDrawOrder(*i);
			delete (*i);
			i = rogueEmitters.erase(i);
		}
		else {
			++i;
		}
	}
}

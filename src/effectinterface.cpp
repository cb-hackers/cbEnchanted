#include "precomp.h"
#include "effectinterface.h"
#include "debug.h"
#include "cbenchanted.h"
#include "cbimage.h"
#include "cbparticleemitter.h"


EffectInterface::EffectInterface():
	cb(static_cast <CBEnchanted *> (this)){
}

EffectInterface::~EffectInterface() {
	
}

void EffectInterface::commandParticleMovement(void) {
	float acc = cb->popValue().toFloat();
	float gravity = cb->popValue().toFloat();
	float speed = cb->popValue().toFloat();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->getObject(emitterId));
	e->setParticleMovement(speed,gravity,acc);
}

void EffectInterface::commandParticleAnimation(void) {
	int32_t frames = cb->popValue().toInt();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->getObject(emitterId));
	e->setParticleAnimation(frames);
}

void EffectInterface::commandParticleEmission(void) {
	float spread = cb->popValue().toFloat();
	float count = cb->popValue().toFloat();
	float density = cb->popValue().toFloat();
	int32_t emitterId = cb->popValue().toInt();
	CBParticleEmitter *e = static_cast<CBParticleEmitter*>(cb->getObject(emitterId));
	e->setParticleEmission(density,count,spread);
}

void EffectInterface::functionMakeEmitter(void) {
	int32_t lifeTime = cb->popValue().toInt();
	int32_t imageId = cb->popValue().toInt();
	CBImage *image = cb->getImage(imageId);
	CBParticleEmitter *emitter = new CBParticleEmitter;
	emitter->create(image,lifeTime);
	int32_t id = cb->addObject(emitter);
	cb->addToDrawOrder(emitter);
	cb->pushValue(id);
}

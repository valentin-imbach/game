
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

#define POISON_PERIOD 2000
#define BURNING_PERIOD 800

class EffectSystem : public System {

	ROSTER(EFFECT)

	void update(uint ticks) {
		for (Entity entity : entities) {
			EffectComponent& effectComponent = ecs->getComponent<EffectComponent>(entity);

			for (int effect = 1; effect < Effect::count; effect++) {
				if (effectComponent.effects[effect].end < ticks) effectComponent.effects[effect] = {};
			}

			if (effectComponent.effects[Effect::SPEED].end && ecs->hasComponent<MovementComponent>(entity)) {
				MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
				movementComponent.speedBonus *= 1.5f;
			}

			if (effectComponent.effects[Effect::POISON].end && ecs->hasComponent<HealthComponent>(entity)) {
				if (effectComponent.effects[Effect::POISON].lastEvent + POISON_PERIOD < ticks) {
					HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
					if (healthComponent.health > 1) {	
						healthComponent.health -= 1;
						healthComponent.lastDamage = ticks;
						effectComponent.effects[Effect::POISON].lastEvent = ticks;
					}
				}
			}

			if (effectComponent.effects[Effect::BURNING].end) {
				if (effectComponent.effects[Effect::BURNING].lastEvent + BURNING_PERIOD < ticks) {
					if (ecs->hasComponent<HealthComponent>(entity)) {
						HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
						if (healthComponent.health > 0) {	
							healthComponent.health -= 1;
							healthComponent.lastDamage = ticks;
							effectComponent.effects[Effect::BURNING].lastEvent = ticks;
						}
					}
				}
				if (ecs->hasComponent<ParticleComponent>(entity)) {
					ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
					if (!particleComponent.emitters[ParticleId::FIRE].lastEmit) {
						particleComponent.emitters[ParticleId::FIRE] = {ticks, 100};
					}
				}
			} else {
				if (ecs->hasComponent<ParticleComponent>(entity)) {
					ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
					particleComponent.emitters[ParticleId::FIRE] = {};
				}
			}

		}
	}
};
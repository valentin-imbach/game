
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

#define POISON_PERIOD 1500

class EffectSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			EffectComponent& effectComponent = ecs->getComponent<EffectComponent>(entity);

			for (int effect = 1; effect < Effect::count; effect++) {
				if (effectComponent.effects[effect].end < ticks) effectComponent.effects[effect] = {};
			}

			if (effectComponent.effects[Effect::SPEED].end && ecs->hasComponent<MovementComponent>(entity)) {
				MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
				movementComponent.speedBonus = 2;
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
		}
	}
};
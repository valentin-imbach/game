
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class HealthSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			if (ecs->hasComponent<CreatureStateComponent>(entity)) {
				SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);
				if (ticks < healthComponent.lastDamage + 200) {
					spriteComponent.effects[SpriteEffectId::HURT] = {true, ticks};
				} else {
					spriteComponent.effects[SpriteEffectId::HURT] = {false, 0};
				}
			}
			if (healthComponent.health <= 0) ecs->addComponent(DeathComponent(), entity);
		}
	}
};
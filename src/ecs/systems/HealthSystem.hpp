
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class HealthSystem : public System {
public:
	void update(uint ticks, EntitySet& set) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			if (ecs->hasComponent<ActionComponent>(entity)) {
				SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);
				if (healthComponent.lastDamage && ticks < healthComponent.lastDamage + 200) {
					spriteComponent.effects[SpriteEffectId::HURT] = {true, ticks};
				} else {
					spriteComponent.effects[SpriteEffectId::HURT] = {false, 0};
				}
			}
			if (healthComponent.health <= 0) ecs->addComponent(DeathComponent(), entity);
		}
	}
};
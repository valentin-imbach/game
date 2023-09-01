
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class DamageAreaSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& collisions, uint ticks) {
		for (Entity damageArea : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(damageArea);
			DamageAreaComponent& damageAreaComponent = ecs->getComponent<DamageAreaComponent>(damageArea);
			for (Entity entity : collisions[damageArea]) {
				if (!ecs->hasComponent<HealthComponent>(entity)) continue;
				HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
				if (ticks - healthComponent.lastDamage < 500) continue;

				healthComponent.health = std::max(0, healthComponent.health - damageAreaComponent.damage);
				healthComponent.lastDamage = ticks;

				if (ecs->hasComponent<ForceComponent>(entity)) {
					ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
					forceComponent.force += damageAreaComponent.force;
				}
			}
			if (damageAreaComponent.duration != 0 && damageAreaComponent.startTime + damageAreaComponent.duration < ticks) {
				ecs->addComponent<DeathComponent>({}, damageArea);
			}
		}
	}
};
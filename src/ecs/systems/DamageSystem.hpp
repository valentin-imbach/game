
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class DamageSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& hits, uint ticks) {
		for (Entity damageArea : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(damageArea);
			DamageComponent& damageComponent = ecs->getComponent<DamageComponent>(damageArea);
			for (Entity entity : hits[damageArea]) {
				if (entity == damageComponent.imune) continue;
				if (ecs->hasComponent<HealthComponent>(entity)) {
					HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
					if (ticks - healthComponent.lastDamage < 500) continue;

					healthComponent.health = std::max(0, healthComponent.health - damageComponent.damage);
					healthComponent.lastDamage = ticks;
				}

				if (ecs->hasComponent<ForceComponent>(entity)) {
					ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
					forceComponent.force += damageComponent.force;
				}

				break;
			}
			if (damageComponent.duration != 0 && damageComponent.startTime + damageComponent.duration < ticks) {
				ecs->addComponent<DeathComponent>({}, damageArea);
			}
		}
	}
};
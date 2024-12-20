
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class DamageSystem : public System {

	ROSTER(POSITION, DAMAGE)

	void update(std::unordered_map<Entity,std::vector<Entity>>& hits, uint ticks, ParticleSystem& particleSystem) {
		for (Entity damageArea : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(damageArea);
			DamageComponent& damageComponent = ecs->getComponent<DamageComponent>(damageArea);
			for (Entity entity : hits[damageArea]) {
				if (entity == damageComponent.imune) continue;
				if (ecs->hasComponent<HealthComponent>(entity)) {
					HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
					if (ticks - healthComponent.lastDamage < 500) continue;

					uint damage = damageComponent.damage;
					if (damage > healthComponent.health) damage = healthComponent.health;
					healthComponent.health -= damage;
					healthComponent.lastDamage = ticks;
					ParticleStyle style = ParticleStyle::templates[ParticleId::HEALTH];
					style.text = std::to_string(-damage);
					particleSystem.emit(style, positionComponent.position, positionComponent.realmId);
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
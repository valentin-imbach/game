
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class DamageSystem : public System {
public:
	bool update(Entity actor, vec position, Entity item, uint ticks, EntitySet& set) {
		if (!ecs->hasComponent<DamageComponent>(item)) return false;
		vec actorPosition = ecs->getComponent<PositionComponent>(actor).position;
		DamageComponent& damageComponent = ecs->getComponent<DamageComponent>(item);
		
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			if (entity == actor) continue;
			HitboxComponent& hitboxComponent = ecs->getComponent<HitboxComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (!Shape::inside(position, hitboxComponent.hitbox, positionComponent.position)) continue;

			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			healthComponent.health -= damageComponent.damage;
			healthComponent.lastDamage = ticks;
			if (ecs->hasComponent<ForceComponent>(entity)) {
				vec force = vec::normalise(positionComponent.position - actorPosition) / 10;
				ecs->getComponent<ForceComponent>(entity).force = force;
			}
			return true;
			
		}
		return false;
	}
};
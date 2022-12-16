
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class DamageSystem : public System {
public:
	void update(Entity actor, vec position, Item& item) {
		if (!ecs->hasComponent<DamageComponent>(item.entity)) return;
		vec actorPosition = ecs->getComponent<PositionComponent>(actor).position;
		DamageComponent& damageComponent = ecs->getComponent<DamageComponent>(item.entity);
		for (Entity entity : entities) {
			ColliderComponent& colliderComponent = ecs->getComponent<ColliderComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			if (isInside(position, positionComponent.position + colliderComponent.collider.offset, colliderComponent.collider.size)) {
				healthComponent.health -= damageComponent.damage;
				healthComponent.damaged = true;
				if (ecs->hasComponent<ForceComponent>(entity)) {
					vec force = normalise(positionComponent.position - actorPosition) / 10;
					ecs->getComponent<ForceComponent>(entity).force = force;
				}
			}
		}
	}
};
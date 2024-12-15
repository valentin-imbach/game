
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ShovingSystem : public System {

	ROSTER(FORCE, POSITION)

	void update(EntityMap& collisions) {
		for (Entity entity : entities) {
			ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			for (Entity other : collisions[entity]) {
				if (!ecs->hasComponent<ForceComponent>(other)) continue;
				PositionComponent& otherPositionComponent = ecs->getComponent<PositionComponent>(other);
				vec dir = vec(1, 0);
				if (vec::dist(positionComponent.position, otherPositionComponent.position) > 0.001f) {
					dir = vec::normalise(positionComponent.position - otherPositionComponent.position);
				}
				forceComponent.force += dir / 100;
			}
		}
	}
};
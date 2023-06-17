
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ProjectileSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			ProjectileComponent& projectileComponent = ecs->getComponent<ProjectileComponent>(entity);
			if (!projectileComponent.grounded) {
				positionComponent.position += projectileComponent.speed;
				projectileComponent.speed *= 0.95f;
				if (vec::norm(projectileComponent.speed) < 0.1) projectileComponent.grounded = true;
			}
			
		}
	}
};
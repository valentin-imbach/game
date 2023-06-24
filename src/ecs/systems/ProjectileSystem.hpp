
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ProjectileSystem : public System {
public:
	void update(uint ticks, uint dt) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			ProjectileComponent& projectileComponent = ecs->getComponent<ProjectileComponent>(entity);
			if (!projectileComponent.grounded) {
				positionComponent.position += projectileComponent.speed * dt / 1000;
				projectileComponent.speed *= std::pow(0.995f, dt);
				if (vec::norm(projectileComponent.speed) < 1) projectileComponent.grounded = true;
			}
			
		}
	}
};
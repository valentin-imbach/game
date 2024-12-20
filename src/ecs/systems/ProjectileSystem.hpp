
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ProjectileSystem : public System {

	ROSTER(PROJECTILE, POSITION, DAMAGE)

	void update(uint ticks, uint dt) {
		EntitySet copy(entities);
		for (Entity entity : copy) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			ProjectileComponent& projectileComponent = ecs->getComponent<ProjectileComponent>(entity);

			if (!projectileComponent.grounded) {
				DamageComponent& damageComponent = ecs->getComponent<DamageComponent>(entity);

				positionComponent.position += projectileComponent.speed * dt / 1000;
				projectileComponent.speed *= std::pow(0.995f, dt);

				damageComponent.force = projectileComponent.speed / 100;
				damageComponent.damage = std::max(1, std::min(int(vec::norm(projectileComponent.speed / 10)), 5));

				if (vec::norm(projectileComponent.speed) < 1) {
					projectileComponent.grounded = true;
					ecs->removeComponent<DamageComponent>(entity);
					ecs->removeComponent<HitboxComponent>(entity);
				}
			}
			
		}
	}
};
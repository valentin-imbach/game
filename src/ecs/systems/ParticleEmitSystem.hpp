
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ParticleEmitSystem : public System {
public:
	void update(ParticleSystem& particleSystem, uint ticks) {
		for (Entity entity : entities) {
			ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			if (!particleComponent.active) continue;
			if (particleComponent.lastEmit + particleComponent.cooldown < ticks) {
				particleSystem.emit(particleComponent.style, positionComponent.position);
				particleComponent.lastEmit = ticks;
			}
			
		}
	}
};
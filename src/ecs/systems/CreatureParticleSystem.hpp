
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class CreatureParticleSystem : public System {

	ROSTER(PARTICLE, MOVEMENT)

	void update(uint ticks) {
		for (Entity entity : entities) {
			ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);

			if (movementComponent.movementState == MovementState::WALK || movementComponent.movementState == MovementState::RUN) {
				if (!particleComponent.emitters[ParticleId::DIRT].lastEmit) {
					particleComponent.emitters[ParticleId::DIRT] = {ticks, 300};
				}
			} else {
				particleComponent.emitters[ParticleId::DIRT] = {};
			}
		}
	}
};

#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class CreatureParticleSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);

			if (movementComponent.movementState == MovementState::WALK || movementComponent.movementState == MovementState::RUN) {
				particleComponent.active = true;
			} else {
				particleComponent.active = false;
			}
		}
	}
};
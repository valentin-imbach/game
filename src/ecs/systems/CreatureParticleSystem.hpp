
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class CreatureParticleSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);

			if (creatureStateComponent.movementState == MovementState::WALK || creatureStateComponent.movementState == MovementState::RUN) {
				particleComponent.active = true;
			} else {
				particleComponent.active = false;
			}
		}
	}
};
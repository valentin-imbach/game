
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

			if (creatureStateComponent.state == CreatureState::WALKING) {
				particleComponent.active = true;
				particleComponent.style = ParticleSystem::DIRT;
			} else {
				particleComponent.active = false;
			}
		}
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "SDL2/SDL_timer.h"
#include "ECS.hpp"

class AnimalAiSystem : public System {
public:
	void update(uint ticks) {
		uint seed = ticks;
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			AnimalAiComponent& animalAiComponent = ecs->getComponent<AnimalAiComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);

			CreatureState::value oldState = creatureStateComponent.state;
			Direction::value oldFacing = creatureStateComponent.facing;

			if (ticks >= animalAiComponent.nextChange) {
				creatureStateComponent.state = CreatureState::IDLE;
				if (noise::bernoulli(seed++, 0.3f)) creatureStateComponent.state = CreatureState::WALKING;

				if (noise::bernoulli(seed++, 0.3f)) {
					directionComponent.direction = Direction::random(seed++);
					pair taxi = Direction::taxi[directionComponent.direction];
					if (taxi.x == 1) {
						creatureStateComponent.facing = Direction::EAST;
					} else if (taxi.x == -1) {
						creatureStateComponent.facing = Direction::WEST;
					}
				}

				animalAiComponent.nextChange = ticks + 2000 + noise::Int(seed++, 0, 1000);
			}

			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.state != oldState) {
				creatureStateComponent.lastChange = ticks;
			}
		}
	}
};
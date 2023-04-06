
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

			if (healthComponent.damaged) {
				animalAiComponent.panic = 20;
				animalAiComponent.nextChange = ticks;
			}

			if (ticks >= animalAiComponent.nextChange) {
				creatureStateComponent.state = CreatureState::IDLE;
				if (bernoulli(ticks, 0.3f) || animalAiComponent.panic) creatureStateComponent.state = CreatureState::WALKING;

				if (bernoulli(ticks + 1, 0.3)) {
					directionComponent.direction = Direction::from_int(rand_int(seed++, 1, 9));
					if (taxiSteps[directionComponent.direction].x == 1) {
						creatureStateComponent.facing = Direction::EAST;
					} else if (taxiSteps[directionComponent.direction].x == -1) {
						creatureStateComponent.facing = Direction::WEST;
					}
				}

				animalAiComponent.nextChange = ticks + 2000 + rand_int(seed++, 0, 1000);
				if (animalAiComponent.panic > 0) {
					animalAiComponent.nextChange = ticks + 100 + rand_int(seed++, 0, 100);
					animalAiComponent.panic -= 1;
				}
			}

			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.state != oldState) {
				creatureStateComponent.lastChange = ticks;
			}
		}
	}
};
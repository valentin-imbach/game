
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "SDL2/SDL_timer.h"
#include "ECS.hpp"

class AnimalAiSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			CreatureState::value& state = ecs->getComponent<CreatureStateComponent>(entity).state;
			Direction::value& facing = ecs->getComponent<CreatureStateComponent>(entity).facing;
			Direction::value& direction = ecs->getComponent<DirectionComponent>(entity).direction;
			AnimalAiComponent& animalAiComponent = ecs->getComponent<AnimalAiComponent>(entity);
			bool& stateChanged = ecs->getComponent<CreatureStateComponent>(entity).stateChanged;

			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);

			uint ticks = SDL_GetTicks();
			CreatureState::value oldState = state;
			Direction::value oldFacing = facing;

			uint seed = ticks + entity;

			if (healthComponent.damaged) {
				animalAiComponent.panic = 20;
				animalAiComponent.nextChange = ticks;
			}

			if (ticks >= animalAiComponent.nextChange) {
				state = CreatureState::IDLE;
				if (bernoulli(ticks, 0.3f) || animalAiComponent.panic) state = CreatureState::WALKING;

				if (bernoulli(ticks + 1, 0.3)) {
					direction = Direction::from_int(rand_int(seed, 1, 9));
					if (taxiSteps[direction].x == 1) {
						facing = Direction::EAST;
					} else if (taxiSteps[direction].x == -1) {
						facing = Direction::WEST;
					}
				}

				animalAiComponent.nextChange = ticks + 2000 + rand_int(seed, 0, 1000);
				if (animalAiComponent.panic > 0) {
					animalAiComponent.nextChange = ticks + 100 + rand_int(seed, 0, 100);
					animalAiComponent.panic -= 1;
				}
			}

			stateChanged = (facing != oldFacing || state != oldState);
		}
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "SDL2/SDL_timer.h"
#include "ECS.hpp"

class AnimalAiSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			CreatureState& state = ecs -> getComponent<CreatureStateComponent>(entity).state;
			Direction& facing = ecs -> getComponent<CreatureStateComponent>(entity).facing;
			Direction& direction = ecs -> getComponent<DirectionComponent>(entity).direction;
			uint& nextChange = ecs -> getComponent<AnimalAiComponent>(entity).nextChange;
			bool& stateChanged = ecs -> getComponent<CreatureStateComponent>(entity).stateChanged;

			uint ticks = SDL_GetTicks();
			stateChanged = false;

			if (ticks >= nextChange) {
				CreatureState oldState = state;
				Direction oldFacing = facing;

				state = CreatureState::IDLE;
				if (bernoulli(ticks, 0.3f)) state = CreatureState::WALKING;

				if (bernoulli(ticks + 1, 0.3)) {
					uint rand = rand_int(ticks, 1, 9);
					direction = Direction(rand);
					if (taxiSteps[rand].x == 1) {
						facing = Direction::EAST;
					} else if (taxiSteps[rand].x == -1) {
						facing = Direction::WEST;
					}
				}
				
				stateChanged = (facing != oldFacing || state != oldState);
				nextChange = ticks + 2000 + rand_int(ticks, 0, 1000);
				
			}
		}
	}
};
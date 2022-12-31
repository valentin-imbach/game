
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class MonsterAiSystem : public System {
public:
	void update(Entity player) {
		for (Entity entity : entities) {
			CreatureState::value& state = ecs->getComponent<CreatureStateComponent>(entity).state;
			Direction::value& facing = ecs->getComponent<CreatureStateComponent>(entity).facing;
			Direction::value& direction = ecs->getComponent<DirectionComponent>(entity).direction;
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			bool& stateChanged = ecs->getComponent<CreatureStateComponent>(entity).stateChanged;

			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);

			CreatureState::value oldState = state;
			Direction::value oldFacing = facing;

			if (!player) {
				state = CreatureState::IDLE;
				continue;
			}

			vec targetPosition = ecs->getComponent<PositionComponent>(player).position;
			if (dist(targetPosition, positionComponent.position) > 1) {
				float ang = angle(targetPosition - positionComponent.position);
				int quad = int(ang / M_PI_4 + 8.5f) % 8;
				direction = Direction::from_int(quad + 1);
				if (taxiSteps[quad].x == 1) {
					facing = Direction::EAST;
				} else if (taxiSteps[quad].x == -1) {
					facing = Direction::WEST;
				}
				state = CreatureState::WALKING;
			} else {
				state = CreatureState::IDLE;
			}

			stateChanged = (facing != oldFacing || state != oldState);
		}
	}
};
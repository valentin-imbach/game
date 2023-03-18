
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "pathfinding.hpp"

class MonsterAiSystem : public System {
public:
	void update(Entity player, std::unordered_set<pair>& solidMap) {
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
			if (dist(targetPosition, positionComponent.position) < 1) state = CreatureState::IDLE;
			else {
				pair start = round(positionComponent.position);
				pair end = round(targetPosition);
				vec offset = positionComponent.position - start;

				Direction::value dir = PathFinder::find_direction(start, end, solidMap);
				if (dir) {
					pair step = start + taxiSteps[dir];
					pair left = start + taxiSteps[rotate_dir(dir, 1)];
					pair right = start + taxiSteps[rotate_dir(dir, 7)];
					if (solidMap.find(left) != solidMap.end() && (left-step)*offset > 0.1) {
						direction = rotate_dir(dir, 7);
					} else if (solidMap.find(right) != solidMap.end() && (right-step)*offset > 0.1) {
						direction = rotate_dir(dir, 1);
					} else {
						direction = dir;
					}
				}

				if (taxiSteps[direction].x == 1) {
					facing = Direction::EAST;
				} else if (taxiSteps[direction].x == -1) {
					facing = Direction::WEST;
				}
				state = dir ? CreatureState::WALKING : CreatureState::IDLE;
			}

			stateChanged = (facing != oldFacing || state != oldState);
		}
	}
};
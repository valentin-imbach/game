
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "pathfinding.hpp"

class MonsterAiSystem : public System {
public:
	void update(Entity player, std::unordered_set<pair>& solidMap, uint ticks) {
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);

			CreatureState::value oldState = creatureStateComponent.state;
			Direction::value oldFacing = creatureStateComponent.facing;

			if (!player) {
				creatureStateComponent.state = CreatureState::IDLE;
				continue;
			}

			vec targetPosition = ecs->getComponent<PositionComponent>(player).position;
			if (dist(targetPosition, positionComponent.position) < 1) {
				creatureStateComponent.state = CreatureState::IDLE;
			} else {
				pair start = round(positionComponent.position);
				pair end = round(targetPosition);
				vec offset = positionComponent.position - start;

				Direction::value dir = PathFinder::find_direction(start, end, solidMap, true);
				if (dir) {
					pair step = start + taxiSteps[dir];
					pair left = start + taxiSteps[rotate_dir(dir, 1)];
					pair right = start + taxiSteps[rotate_dir(dir, 7)];
					if (solidMap.find(left) != solidMap.end() && (left-step)*offset > 0.1) {
						directionComponent.direction = rotate_dir(dir, 7);
					} else if (solidMap.find(right) != solidMap.end() && (right-step)*offset > 0.1) {
						directionComponent.direction = rotate_dir(dir, 1);
					} else {
						directionComponent.direction = dir;
					}
				}

				if (taxiSteps[directionComponent.direction].x == 1) {
					creatureStateComponent.facing = Direction::EAST;
				} else if (taxiSteps[directionComponent.direction].x == -1) {
					creatureStateComponent.facing = Direction::WEST;
				}
				creatureStateComponent.state = dir ? CreatureState::WALKING : CreatureState::IDLE;
			}
			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.state != oldState) {
				creatureStateComponent.lastChange = ticks;
			}	
		}
	}
};
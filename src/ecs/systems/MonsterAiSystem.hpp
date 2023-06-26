
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "pathfinding.hpp"

class MonsterAiSystem : public System {
public:
	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
		
			CreatureState::value oldState = creatureStateComponent.state;
			Direction::value oldFacing = creatureStateComponent.facing;

			if (!sensorComponent.engaged || ticks > sensorComponent.lastSeen + 5000) {
				creatureStateComponent.state = CreatureState::IDLE;
				continue;
			}

			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			vec targetPosition = sensorComponent.position;
			if (vec::dist(targetPosition, positionComponent.position) < 1) {
				creatureStateComponent.state = CreatureState::IDLE;
			} else {
				pair start = vec::round(positionComponent.position);
				pair end = vec::round(targetPosition);
				vec offset = positionComponent.position - start;

				Direction::value dir = ai::find_direction(start, end, realm->solidMap, true);
				if (dir) {
					pair step = start + Direction::taxi[dir];
					pair left = start + Direction::taxi[Direction::rotate(dir, 1)];
					pair right = start + Direction::taxi[Direction::rotate(dir, 7)];
					if (realm->solidMap.find(left) != realm->solidMap.end() && vec::dot(left-step, offset) > 0.1) {
						directionComponent.direction = Direction::rotate(dir, 7);
					} else if (realm->solidMap.find(right) != realm->solidMap.end() && vec::dot(right - step, offset) > 0.1) {
						directionComponent.direction = Direction::rotate(dir, 1);
					} else {
						directionComponent.direction = dir;
					}
				}

				if (Direction::taxi[directionComponent.direction].x == 1) {
					creatureStateComponent.facing = Direction::EAST;
				} else if (Direction::taxi[directionComponent.direction].x == -1) {
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
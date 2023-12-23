
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"
#include "ECS.hpp"

class AiFleeSystem : public System {
public:
	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			AiFleeComponent& aiFleeComponent = ecs->getComponent<AiFleeComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			
			if (sensorComponent.lastSeen - ticks < 3000) {
				aiComponent.scores[AiState::FLEE] = 80;
				aiFleeComponent.avoid = sensorComponent.position;
			} else {
				aiComponent.scores[AiState::FLEE] = 0;
			}
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::FLEE) continue;
			
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiFleeComponent& aiFleeComponent = ecs->getComponent<AiFleeComponent>(entity);

			MovementState::value oldState = creatureStateComponent.movementState;
			Direction::value oldFacing = creatureStateComponent.facing;

			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			
			if (aiComponent.change || vec::dist(aiFleeComponent.target, positionComponent.position) < 1 || ticks - aiFleeComponent.lastChange > 500) {
				vec pos = positionComponent.position + (positionComponent.position - aiFleeComponent.avoid) * 7;
				aiFleeComponent.target = realm->findFree(vec::round(pos), 5, false);
				aiFleeComponent.lastChange = ticks;
			}

			pair start = vec::round(positionComponent.position);
			pair end = vec::round(aiFleeComponent.target);
			vec offset = positionComponent.position - start;
			//LOG(start, end);
			auto lambda = [realm](pair pos){ return realm->walkable(pos); };
			Direction::value dir = ai::find_direction(start, end, lambda, true);
	
			if (dir) {
				pair step = start + Direction::taxi[dir];
				pair left = start + Direction::taxi[Direction::rotate(dir, 1)];
				pair right = start + Direction::taxi[Direction::rotate(dir, 7)];
				if (!realm->walkable(left) && vec::dot(left-step, offset) > 0.1) {
					directionComponent.direction = Direction::rotate(dir, 7);
				} else if (!realm->walkable(right) && vec::dot(right - step, offset) > 0.1) {
					directionComponent.direction = Direction::rotate(dir, 1);
				} else {
					directionComponent.direction = dir;
				}
			} else {
				vec pos = positionComponent.position + (positionComponent.position - aiFleeComponent.avoid) * 7;
				aiFleeComponent.target = realm->findFree(vec::round(pos), 5, false);
				aiFleeComponent.lastChange = ticks;
				continue;
			}

			if (Direction::taxi[directionComponent.direction].x == 1) {
				creatureStateComponent.facing = Direction::EAST;
			} else if (Direction::taxi[directionComponent.direction].x == -1) {
				creatureStateComponent.facing = Direction::WEST;
			}
			creatureStateComponent.movementState = dir ? MovementState::RUN : MovementState::IDLE;
			
			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.movementState != oldState) {
				creatureStateComponent.lastChange = ticks;
			}	

		}
	}
};
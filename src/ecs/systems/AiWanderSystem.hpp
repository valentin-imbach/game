
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"
#include "ECS.hpp"

class AiWanderSystem : public System {
public:
	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			aiComponent.scores[AiState::WANDER] = 10;
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::WANDER) continue;
			
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiWanderComponent& aiWanderComponent = ecs->getComponent<AiWanderComponent>(entity);

			MovementState::value oldState = movementComponent.movementState;
			Direction::value oldFacing = movementComponent.facing;

			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			
			if (aiComponent.change || vec::dist(aiWanderComponent.position, positionComponent.position) < 0.1f) {
				aiWanderComponent.position = realm->findFree(vec::round(positionComponent.position), 5, false);
			}

			pair start = vec::round(positionComponent.position);
			pair end = vec::round(aiWanderComponent.position);
			vec offset = positionComponent.position - start;
			//LOG(start, end);
			auto lambda = [realm](pair pos){ return realm->walkable(pos); };
			Direction::value dir = ai::seek(start, end, lambda, true);
	
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
				aiWanderComponent.position = realm->findFree(vec::round(positionComponent.position), 5, false);
				continue;
			}

			if (Direction::taxi[directionComponent.direction].x == 1) {
				movementComponent.facing = Direction::EAST;
			} else if (Direction::taxi[directionComponent.direction].x == -1) {
				movementComponent.facing = Direction::WEST;
			}
			
			movementComponent.movementState = dir ? MovementState::WALK : MovementState::IDLE;
			
			if (movementComponent.facing != oldFacing || movementComponent.movementState != oldState) {
				movementComponent.movementStart = ticks;
			}

		}
	}
};
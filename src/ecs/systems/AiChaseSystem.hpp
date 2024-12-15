
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"
#include "ECS.hpp"

class AiChaseSystem : public System {

	ROSTER(AI, AI_CHASE, SENSOR, POSITION, MOVEMENT, DIRECTION, FACING)

	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			AiChaseComponent& aiChaseComponent = ecs->getComponent<AiChaseComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			
			if (ticks - sensorComponent.lastSeen < 3000) {
				float dist = vec::dist(positionComponent.position, sensorComponent.position);
				
				aiComponent.scores[AiState::CHASE] = 90 * std::exp(-dist/10);
				aiChaseComponent.target = sensorComponent.position;
			} else {
				aiComponent.scores[AiState::CHASE] = 0;
			}
			
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::CHASE) continue;
			
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiChaseComponent& aiChaseComponent = ecs->getComponent<AiChaseComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);

			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			pair start = vec::round(positionComponent.position);
			pair end = vec::round(aiChaseComponent.target);
			vec offset = positionComponent.position - start;

			vec v = aiChaseComponent.target - positionComponent.position;

			Direction::value dir;
			if (start == end) {
				if (vec::norm(v) > 0.1f) {
					dir = Direction::from_vec(v);
				}
			} else {
				auto lambda = [realm](pair pos){ return realm->walkable(pos); };
				dir = ai::seek(start, end, lambda, true);
		
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
				}
			}

			facingComponent.facing = vec::normalise(v);
			movementComponent.movementState = dir ? MovementState::RUN : MovementState::IDLE;
		}
	}
};
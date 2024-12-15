
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"
#include "ECS.hpp"

class AiPostSystem : public System {

	ROSTER(AI, AI_POST, POSITION, MOVEMENT, DIRECTION, FACING)

	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			AiPostComponent& aiPostComponent = ecs->getComponent<AiPostComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			
			float dist = vec::dist(positionComponent.position, aiPostComponent.position);
			if (dist > aiPostComponent.range) {
				aiComponent.scores[AiState::POST] = 90;
			} else {
				aiComponent.scores[AiState::POST] = lerp::linear(dist / aiPostComponent.range, 0, 90);
			}
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::POST) continue;
			
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiPostComponent& aiPostComponent = ecs->getComponent<AiPostComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);

			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			pair start = vec::round(positionComponent.position);
			pair end = vec::round(aiPostComponent.position);
			vec offset = positionComponent.position - start;

			Direction::value dir;
			if (start == end) {
				vec v = aiPostComponent.position - positionComponent.position;
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

			vec to = vec::normalise(aiPostComponent.position - positionComponent.position);
			facingComponent.facing = to;
			movementComponent.movementState = dir ? MovementState::RUN : MovementState::IDLE;
			
		
		}
	}
};
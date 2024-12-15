
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"
#include "ECS.hpp"

class AiLureSystem : public System {

	ROSTER(AI, AI_LURE, SENSOR, MOVEMENT, DIRECTION, POSITION, FACING)

	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			AiLureComponent& aiLureComponent = ecs->getComponent<AiLureComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);

			// HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			// float relHealth = float(healthComponent.health)/healthComponent.maxHealth;

			aiComponent.scores[AiState::LURE] = 0;
			if (sensorComponent.lastSeen - ticks < 3000) {
				Entity item = sensorComponent.item;
				if (!item) return;
				ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
				if (itemComponent.itemId == aiLureComponent.itemId) {
					aiComponent.scores[AiState::LURE] = 70;
					aiLureComponent.target = sensorComponent.position;
				}
			}
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::LURE) continue;
			
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiLureComponent& aiLureComponent = ecs->getComponent<AiLureComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);

			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			pair start = vec::round(positionComponent.position);
			pair end = vec::round(aiLureComponent.target);
			vec offset = positionComponent.position - start;

			Direction::value dir;
			if (start == end) {
				vec v = aiLureComponent.target - positionComponent.position;
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

			vec to = vec::normalise(aiLureComponent.target - positionComponent.position);
			facingComponent.facing = to;
			movementComponent.movementState = dir ? MovementState::RUN : MovementState::IDLE;

		}
	}
};
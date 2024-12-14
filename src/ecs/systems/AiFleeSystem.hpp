
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
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			float relHealth = float(healthComponent.health)/healthComponent.maxHealth;

			if (sensorComponent.lastSeen - ticks < 3000) {
				aiComponent.scores[AiState::FLEE] = lerp::linear(relHealth, 100, aiFleeComponent.baseScore);
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
			
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiFleeComponent& aiFleeComponent = ecs->getComponent<AiFleeComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);

			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			pair start = vec::round(positionComponent.position);
			pair avoid = vec::round(aiFleeComponent.avoid);
			
			auto lambda = [realm](pair pos){ return realm->walkable(pos); };
			Direction::value dir = ai::avoid(start, avoid, lambda, true);
	
			if (dir) {
				vec offset = positionComponent.position - start;
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
				continue;
			}

			movementComponent.movementState = dir ? MovementState::RUN : MovementState::IDLE;
			vec away = vec::normalise(positionComponent.position - aiFleeComponent.avoid);
			facingComponent.facing = away;
		}
	}
};
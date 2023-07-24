
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class AiMoveSystem : public System {
public:
	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			AiMoveComponent& aiMoveComponent = ecs->getComponent<AiMoveComponent>(entity);

			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			pair tile = vec::round(positionComponent.position);
			aiMoveComponent.moveScores.fill(0);

			for (ushort dir = 1; dir <= 8; dir++) {
				pair step = Direction::taxi[dir];
				if (realm->walkable(tile + step)) continue;

				vec offset = vec(tile + step) - positionComponent.position;
				for (ushort dir2 = 1; dir2 <= 8; dir2++) {
					float dot = vec::dot(vec::normalise(offset), Direction::unit[dir2]);
					float dis = vec::norm(offset);
					if (dot < 0 || dis > 1.5f) continue;
					aiMoveComponent.moveScores[dir2] -= dot * (1.5f - dis) * 5;
				}
			}
			
		}
	}

	void move(uint ticks) {
		for (Entity entity : entities) {
			AiMoveComponent& aiMoveComponent = ecs->getComponent<AiMoveComponent>(entity);
			if (!aiMoveComponent.move) continue;

			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);

			Direction::value direction;
			float score;
			for (int dir = 1; dir <= 8; dir++) {
				//LOG(dir, aiMoveComponent.moveScores[dir]);
				if (dir == 1 || aiMoveComponent.moveScores[dir] > score) {
					direction = Direction::from_int(dir);
					score = aiMoveComponent.moveScores[dir];
				}
			}

			if (score < 0.2) continue;

			CreatureState::value oldState = creatureStateComponent.state;
			creatureStateComponent.state = CreatureState::WALKING;
			directionComponent.direction = direction;
			//LOG(Direction::to_string(direction));

			Direction::value oldFacing = creatureStateComponent.facing;
			pair taxi = Direction::taxi[directionComponent.direction];
			if (taxi.x == 1) {
				creatureStateComponent.facing = Direction::EAST;
			} else if (taxi.x == -1) {
				creatureStateComponent.facing = Direction::WEST;
			}

			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.state != oldState) {
				creatureStateComponent.lastChange = ticks;
			}
		}
	}
};
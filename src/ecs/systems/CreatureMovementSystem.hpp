
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "Map.hpp"
#include "ECS.hpp"

class CreatureMovementSystem : public System {
public:
	void update(uint dt, RealmManager& realmManager) {
		for (Entity entity : entities) {
			DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			ColliderComponent& colliderComponent = ecs->getComponent<ColliderComponent>(entity);

			vec newPosition = positionComponent.position;
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			float speed = movementComponent.speed * TileId::speedMul(realm->map->getTileId(vec::round(positionComponent.position)));

			if (creatureStateComponent.state == CreatureState::WALKING) {
				newPosition += dt * speed * Direction::unit[directionComponent.direction] / 1000;
			} else if (creatureStateComponent.state == CreatureState::RUNNING) {
				newPosition += dt * 2 * speed * Direction::unit[directionComponent.direction] / 1000;
			}

			if (ecs->hasComponent<ForceComponent>(entity)) {
				ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
				newPosition += forceComponent.force;
				forceComponent.force *= 0.9f;
			}

			if (!isColliding(colliderComponent.collider, newPosition, realm) || isColliding(colliderComponent.collider, positionComponent.position, realm)) {
				positionComponent.position = newPosition;
			} else if (!isColliding(colliderComponent.collider, {newPosition.x, positionComponent.position.y}, realm)) {
				positionComponent.position.x = newPosition.x;
			} else if (!isColliding(colliderComponent.collider, {positionComponent.position.x, newPosition.y}, realm)) {
				positionComponent.position.y = newPosition.y;
			}
		}
	}

private:
	bool isColliding(Shape collider, vec position, Realm* realm) {
		pair topLeft = vec::round(position - collider.size / 2);
		pair bottomRight = vec::round(position + collider.size / 2);
		for (int x = topLeft.x; x <= bottomRight.x; x++) {
			for (int y = topLeft.y; y <= bottomRight.y; y++) {
				pair pos(x, y);
				if (!realm->walkable(pos)) return true;
			}
		}
		return false;
	}
};
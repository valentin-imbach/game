
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
			ColliderComponent& colliderComponent = ecs->getComponent<ColliderComponent>(entity);

			vec offset;
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			TileId::value tileId = realm->map->getTileId(vec::round(positionComponent.position));

			if (movementComponent.movementState == MovementState::WALK) {
				float speed = movementComponent.walkingSpeed * TileTemplate::templates[tileId].speed;
				offset += dt * speed * Direction::unit[directionComponent.direction] / 1000;
			} else if (movementComponent.movementState == MovementState::RUN) {
				float speed = movementComponent.runningSpeed * TileTemplate::templates[tileId].speed;
				offset += dt * speed * Direction::unit[directionComponent.direction] / 1000;
			}

			if (ecs->hasComponent<ForceComponent>(entity)) {
				ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
				offset += forceComponent.force;
				forceComponent.force *= 0.9f;
			}

			vec xOffset(offset.x, 0);
			vec yOffset(0, offset.y);

			bool collision = isColliding(colliderComponent.collider, positionComponent.position, realm);
			bool xCollision = isColliding(colliderComponent.collider, positionComponent.position + xOffset, realm);
			bool yCollision = isColliding(colliderComponent.collider, positionComponent.position + yOffset, realm);
			bool xyCollision = isColliding(colliderComponent.collider, positionComponent.position + offset, realm);
			
			vec newPosition = positionComponent.position;

			if (collision || !xyCollision) {
				newPosition += offset;
			} else if (!xCollision) {
				newPosition += xOffset;
			} else if (!yCollision) {
				newPosition += yOffset;
			}

			if (vec::dist(positionComponent.position, newPosition) > 0.01f) {
				positionComponent.position = newPosition;
			} else {
				movementComponent.movementState = MovementState::IDLE;
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
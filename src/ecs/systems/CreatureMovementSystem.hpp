
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "Map.hpp"
#include "ECS.hpp"

class CreatureMovementSystem : public System {
public:
	void update(uint dt, GridMap& gridMap, Map& map) {
		for (Entity entity : entities) {
			Direction::value direction = ecs->getComponent<DirectionComponent>(entity).direction;
			float speed = ecs->getComponent<MovementComponent>(entity).speed;
			vec& position = ecs->getComponent<PositionComponent>(entity).position;
			CreatureState::value state = ecs->getComponent<CreatureStateComponent>(entity).state;

			Collider collider = ecs->getComponent<ColliderComponent>(entity).collider;

			vec newPosition = position;
			if (state == CreatureState::WALKING) {
				newPosition += dt * speed * unitVectors[int(direction) - 1] / 1000;
			}

			if (ecs->hasComponent<ForceComponent>(entity)) {
				ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
				newPosition += forceComponent.force;
				forceComponent.force *= 0.9f;
			}

			if (!isColliding(collider, newPosition, gridMap, map)) {
				position = newPosition;
			} else if (!isColliding(collider, {newPosition.x, position.y}, gridMap, map)) {
				position.x = newPosition.x;
			} else if (!isColliding(collider, {position.x, newPosition.y}, gridMap, map)) {
				position.y = newPosition.y;
			}
		}
	}

private:
	bool isColliding(Collider collider, vec position, GridMap& gridMap, Map& map) {
		pair topLeft = round(position - collider.size / 2);
		pair bottomRight = round(position + collider.size / 2);
		for (int x = topLeft.x; x <= bottomRight.x; x++) {
			for (int y = topLeft.y; y <= bottomRight.y; y++) {
				if (!isFree(gridMap, map, {x, y})) return true;
			}
		}
		return false;
	}

	bool isFree(GridMap& gridMap, Map& map, pair position) {
		if (gridMap.find(position) != gridMap.end()) return false;
		if (map.getTileId(position) == TileId::WATER) return false;
		return true;
	}
};
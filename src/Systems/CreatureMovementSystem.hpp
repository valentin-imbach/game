
#pragma once
#include "../System.hpp"
#include "../Map.hpp"

typedef std::unordered_map<pair, Entity> GridMap;

class CreatureMovementSystem : public System {
public:
	void update(uint dt, GridMap& gridMap, Map& map) {
		for (Entity entity : entities) {
			Direction direction = componentManager->get<DirectionComponent>(entity).direction;
			float speed = componentManager->get<MovementComponent>(entity).speed;
			vec& position = componentManager->get<PositionComponent>(entity).position;
			CreatureState state = componentManager->get<CreatureStateComponent>(entity).state;

			Collider collider = componentManager->get<ColliderComponent>(entity).collider;
			std::vector<Entity>& collisions = componentManager->get<ColliderComponent>(entity).collisions;

			vec newPosition = position;
			if (state == CreatureState::WALKING) {
				newPosition += dt * speed * unitVectors[int(direction)] / 1000;
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
		pair topLeft = round(position + collider.offset);
		pair bottomRight = round(position + collider.offset + collider.size);
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
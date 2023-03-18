
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "Map.hpp"
#include "ECS.hpp"

class CreatureMovementSystem : public System {
public:
	void update(uint dt, std::unordered_set<pair>& solidMap, Map* map) {
		for (Entity entity : entities) {
			Direction::value direction = ecs->getComponent<DirectionComponent>(entity).direction;
			float speed = ecs->getComponent<MovementComponent>(entity).speed;
			vec& position = ecs->getComponent<PositionComponent>(entity).position;
			CreatureState::value state = ecs->getComponent<CreatureStateComponent>(entity).state;

			Collider collider = ecs->getComponent<ColliderComponent>(entity).collider;

			vec newPosition = position;
			if (state == CreatureState::WALKING) {
				newPosition += dt * speed * unitVectors[direction] / 1000;
			}

			if (ecs->hasComponent<ForceComponent>(entity)) {
				ForceComponent& forceComponent = ecs->getComponent<ForceComponent>(entity);
				newPosition += forceComponent.force;
				forceComponent.force *= 0.9f;
			}

			if (!isColliding(collider, newPosition, solidMap, map) || isColliding(collider, position, solidMap, map)) {
				position = newPosition;
			} else if (!isColliding(collider, {newPosition.x, position.y}, solidMap, map)) {
				position.x = newPosition.x;
			} else if (!isColliding(collider, {position.x, newPosition.y}, solidMap, map)) {
				position.y = newPosition.y;
			}
		}
	}

private:
	bool isColliding(Collider collider, vec position, std::unordered_set<pair>& solidMap, Map* map) {
		pair topLeft = round(position - collider.size / 2);
		pair bottomRight = round(position + collider.size / 2);
		for (int x = topLeft.x; x <= bottomRight.x; x++) {
			for (int y = topLeft.y; y <= bottomRight.y; y++) {
				if (!isFree(solidMap, map, {x, y})) return true;
			}
		}
		return false;
	}

	bool isFree(std::unordered_set<pair>& solidMap, Map* map, pair position) {
		if (solidMap.find(position) != solidMap.end()) return false;
		if (map->getTileId(position) == TileId::WATER) return false;
		return true;
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "TextureManager.hpp"

#define COLLIDER_DEBUG true

class CollisionSystem : public System {
public:
	void update() {
		for (Entity entityA : entities) {
			vec positionA = ecs -> getComponent<PositionComponent>(entityA).position;
			Collider colliderA = ecs -> getComponent<ColliderComponent>(entityA).collider;
			std::vector<Entity>& collisions = ecs -> getComponent<ColliderComponent>(entityA).collisions;
			collisions.clear();

			for (Entity entityB : entities) {
				if (entityB == entityA) continue;
				vec positionB = ecs -> getComponent<PositionComponent>(entityB).position;
				Collider colliderB = ecs -> getComponent<ColliderComponent>(entityB).collider;
				if (Collider::AABB(colliderA, positionA, colliderB, positionB)) {
					collisions.push_back(entityB);
				}
			}
		}
	}
};
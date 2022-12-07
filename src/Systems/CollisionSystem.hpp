
#pragma once
#include "../System.hpp"

class CollisionSystem : public System {
public:
	void update() {
		for (Entity entityA : entities) {
			vec positionA = componentManager->get<PositionComponent>(entityA).position;
			Collider colliderA = componentManager->get<ColliderComponent>(entityA).collider;
			std::vector<Entity>& collisions = componentManager->get<ColliderComponent>(entityA).collisions;
			collisions.clear();

			for (Entity entityB : entities) {
				if (entityB == entityA) continue;
				vec positionB = componentManager->get<PositionComponent>(entityB).position;
				Collider colliderB = componentManager->get<ColliderComponent>(entityB).collider;
				if (Collider::AABB(colliderA, positionA, colliderB, positionB)) {
					collisions.push_back(entityB);
				}
			}
		}
	}
};
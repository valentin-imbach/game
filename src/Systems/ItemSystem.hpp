
#pragma once
#include <vector>
#include "../System.hpp"
#include "../ECS.hpp"

class ItemSystem : public System {
public:
	void update() {
		std::vector<Entity> kill;
		for (Entity entity : entities) {
			std::vector<Entity>& collisions = ecs -> getComponent<ColliderComponent>(entity).collisions;
			if (!collisions.empty()) {
				kill.push_back(entity);
			}
		}
		for (Entity entity : kill) {
			ecs -> destroyEntity(entity);
		}
	}
};
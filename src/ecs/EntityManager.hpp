
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class EntityManager {
public:
	EntityManager() {
		for (int entity = 1; entity <= MAX_ENTITIES; entity += 1) {
			availableEntities.push(entity);
		}
	}

	Entity createEntity() {
		if (availableEntities.empty()) {
			WARNING("Entity limit reached");
			return 0;
		}
		Entity entity = availableEntities.front();
		availableEntities.pop();
		return entity;
	}

	void destroyEntity(Entity entity) {
		if (!entity) return;
		signatures[entity].reset();
		availableEntities.push(entity);
	}

	int entityCount() { return MAX_ENTITIES - availableEntities.size(); }

	std::array<Signature, MAX_ENTITIES + 1> signatures;

private:
	std::queue<Entity> availableEntities;
};
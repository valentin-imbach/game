
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class DeathSystem : public System {
public:
	void update() {
		if (entities.size() > 0) LOG(entities.size());
		std::set<Entity> copy(entities);
		for (Entity entity : copy) {
			ecs -> destroyEntity(entity);
		}
	}
};
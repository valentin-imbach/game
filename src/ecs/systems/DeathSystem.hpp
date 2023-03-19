
#pragma once
#include "System.hpp"
#include "ECS.hpp"

class DeathSystem : public System {
public:
	void update() {
		std::set<Entity> copy(entities);
		for (Entity entity : copy) ecs -> destroyEntity(entity);
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class HealthSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			if (healthComponent.health <= 0) ecs->dead.push_back(entity);
			healthComponent.damaged = false;
		}
	}
};
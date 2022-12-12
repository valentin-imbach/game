
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class ForageSystem : public System {
public:
	void update(vec position, Item& item) {
		for (Entity entity : entities) {
			ResourceComponent& resourceComponent = ecs->getComponent<ResourceComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (isInside(position, positionComponent.position, {1.0f, 1.0f})) {
				if (true) {
					healthComponent.health -= 1;
				}
			}
		}
	}
};
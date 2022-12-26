
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class ForageSystem : public System {
public:
	void update(vec position, Entity item) {
		if (!ecs->hasComponent<ToolComponent>(item)) return;
		ToolComponent& toolComponent = ecs->getComponent<ToolComponent>(item);
		for (Entity entity : entities) {
			ResourceComponent& resourceComponent = ecs->getComponent<ResourceComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (isInside(position, positionComponent.position, {1.0f, 1.0f})) {
				if (toolComponent.toolId == resourceComponent.toolId) {
					healthComponent.health -= 1;
				}
			}
		}
	}
};
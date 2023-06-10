
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class ForageSystem : public System {
public:
	void update(vec position, Entity item, uint ticks) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (!isInside(position, positionComponent.position, {1.0f, 1.0f})) continue;
			ResourceComponent& resourceComponent = ecs->getComponent<ResourceComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			if (!resourceComponent.toolId) {
				healthComponent.health -= 1;
				return;
			}

			if (!ecs->hasComponent<ToolComponent>(item)) return;
			ToolComponent& toolComponent = ecs->getComponent<ToolComponent>(item);
			if (resourceComponent.toolId == toolComponent.toolId) {
				healthComponent.health -= 1;
				spriteComponent.effects[SpriteEffectId::SHAKE] = {true, ticks};
			}
		}
	}
};
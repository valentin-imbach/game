
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

			if (hasItemKind(item, resourceComponent.toolId)) {
				if (getItemProperty(item, ItemProperty::LEVEL) >= resourceComponent.level) {
					int damage = std::min(getItemProperty(item, ItemProperty::EFFICIENCY), int(healthComponent.health));
					healthComponent.health -= damage;
					spriteComponent.effects[SpriteEffectId::SHAKE] = {true, ticks};
				}
			}
		}
	}
};
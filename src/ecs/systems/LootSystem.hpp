
#pragma once
#include "EntityFactory.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class LootSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			LootComponent& lootComponent = ecs->getComponent<LootComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (healthComponent.health <= 0) {
				EntityFactory::createItemEntity(lootComponent.loot, positionComponent.position);
			}
		}
	}
};
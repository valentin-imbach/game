
#pragma once
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
				Entity loot = ecs->createEntity();
				ecs->addComponent<PositionComponent>({positionComponent.position}, loot);
				ecs->addComponent<ItemComponent>({Item(lootComponent.itemId, lootComponent.count)}, loot);
				SpriteStack spriteStack;
				int index = int(lootComponent.itemId);
				spriteStack.addSprite({SpriteSheet::ITEMS, {index % 6, index / 6}, {1, 1}});
				ecs->addComponent<SpriteComponent>({spriteStack, 0, 0.5f}, loot);
				Collider collider = {{-0.2f, -0.2f}, {0.4f, 0.4f}};
				ecs->addComponent<ColliderComponent>({collider}, loot);
			}
		}
	}
};
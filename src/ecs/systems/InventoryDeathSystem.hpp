
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class InventoryDeathSystem : public System {
public:
	void update(uint ticks) {
		uint seed = ticks;
		for (Entity entity : entities) {
			InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);
			if (ecs->hasComponent<PositionComponent>(entity)) {
				PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
				for (int y = 0; y < inventoryComponent.inventory.size.y; y++) {
					for (int x = 0; x < inventoryComponent.inventory.size.x; x++) {
						Entity item = inventoryComponent.inventory.itemContainers[x][y].item;
						if (item) {
							vec offset;
							offset.x = noise::Float(seed++, -0.3f, 0.3f);
							offset.y = noise::Float(seed++, -0.3f, 0.3f);
							vec position = positionComponent.position + offset;
							ecs->addComponent<PositionComponent>({position}, item);
							inventoryComponent.inventory.itemContainers[x][y].clear();
						}
					}
				}

				if (ecs->hasComponent<PlayerComponent>(entity)) {
					PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
					for (int y = 0; y < playerComponent.hotbar.size.y; y++) {
						for (int x = 0; x < playerComponent.hotbar.size.x; x++) {
							Entity item = playerComponent.hotbar.itemContainers[x][y].item;
							if (item) {
								vec offset;
								offset.x = noise::Float(seed++, -0.3f, 0.3f);
								offset.y = noise::Float(seed++, -0.3f, 0.3f);
								vec position = positionComponent.position + offset;
								ecs->addComponent<PositionComponent>({position}, item);
								playerComponent.hotbar.itemContainers[x][y].clear();
							}
						}
					}
				}

			} else {
				inventoryComponent.inventory.clear(true);
			}
		}
	}
};
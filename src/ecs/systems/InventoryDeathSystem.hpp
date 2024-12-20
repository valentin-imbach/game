
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class InventoryDeathSystem : public System {

	ROSTER(INVENTORY, DEATH)

	void update(uint ticks, RealmManager& realmManager) {
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
							ecs->addComponent<PositionComponent>({position, positionComponent.realmId}, item);
							Realm* realm = realmManager.getRealm(positionComponent.realmId);
							pair chunk = realm->chunkManager.getChunk(position);
							realm->attach(item, chunk);
							inventoryComponent.inventory.itemContainers[x][y].clear();
						}
					}
				}

				if (ecs->hasComponent<PlayerComponent>(entity)) {
					PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
					for (int y = 0; y < playerComponent.equipment.size.y; y++) {
						for (int x = 0; x < playerComponent.equipment.size.x; x++) {
							Entity item = playerComponent.equipment.itemContainers[x][y].item;
							if (item) {
								vec offset;
								offset.x = noise::Float(seed++, -0.3f, 0.3f);
								offset.y = noise::Float(seed++, -0.3f, 0.3f);
								vec position = positionComponent.position + offset;
								ecs->addComponent<PositionComponent>({position, positionComponent.realmId}, item);
								playerComponent.equipment.itemContainers[x][y].clear();
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
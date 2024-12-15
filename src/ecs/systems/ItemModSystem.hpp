
#pragma once
#include "Components.hpp"
#include "System.hpp"
#include "ECS.hpp"

class ItemModSystem : public System {

	ROSTER(PLAYER, MOVEMENT)

	void update() {
		for (Entity entity : entities) {
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);

			pair size = playerComponent.equipment.size;
			for (int x = 0; x < size.x; x++) {
				for (int y = 0; y < size.y; y++) {
					Entity item = playerComponent.equipment.itemContainers[x][y].item;
					if (!item || !ecs->hasComponent<ItemModComponent>(item)) continue;
					ItemModComponent& itemModComponent = ecs->getComponent<ItemModComponent>(item);
					if (itemModComponent.mods[ItemModId::SPEED]) {
						float mult = 1.0f + float(itemModComponent.mods[ItemModId::SPEED]) / 100;
						movementComponent.speedBonus *= mult;
					}
				}
			}
		}
	}
};
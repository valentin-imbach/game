
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"

class GatherSystem : public System {
public:
	void update(Entity player, vec position, uint ticks) {
		InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(player);
		uint seed = ticks;
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (!vec::inside(position, positionComponent.position, {1.0f, 1.0f})) continue;
			LootComponent& lootComponent = ecs->getComponent<LootComponent>(entity);

			for (uint i = 0; i < lootComponent.lootTable.size; i++) {
				Entity item = lootComponent.lootTable.table[i].create(seed++);
				Entity rest = inventoryComponent.inventory.add(item);
				if (rest) {
					vec offset;
					offset.x = noise::Float(seed++, -0.3f, 0.3f);
					offset.y = noise::Float(seed++, -0.3f, 0.3f);
					ecs->addComponent<PositionComponent>({positionComponent.position + offset}, rest);
				}
			}
		}
	}
};
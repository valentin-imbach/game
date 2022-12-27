
#pragma once
#include <vector>
#include "Components.hpp"
#include "System.hpp"
#include "ECS.hpp"
#include "Item.hpp"

class ItemPickupSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& collisions) {
		for (Entity entity : entities) {
			Inventory& inventory = ecs->getComponent<InventoryComponent>(entity).inventory;
			for (Entity other : collisions[entity]) {
				if (ecs->hasComponent<ItemComponent>(other)) {
					if (!inventory.add(other)) {
						ecs->removeComponent<PositionComponent>(other);
					}
				}
			}
		}
	}
};
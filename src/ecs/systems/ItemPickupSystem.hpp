
#pragma once
#include <vector>
#include "Components.hpp"
#include "System.hpp"
#include "ECS.hpp"
#include "Item.hpp"

class ItemPickupSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			std::vector<Entity>& collisions = ecs->getComponent<ColliderComponent>(entity).collisions;
			Inventory& inventory = ecs->getComponent<InventoryComponent>(entity).inventory;
			for (Entity other : collisions) {
				if (ecs->hasComponent<ItemComponent>(other)) {
					Item& item = ecs->getComponent<ItemComponent>(other).item;
					if (item.entity) {
						if (!inventory.add(item)) {
							ecs->removeComponent<PositionComponent>(other);
						}
					} else {
						item = inventory.add(item);
						if (!item) ecs->destroyEntity(other);
					}
				}
			}
		}
	}
};
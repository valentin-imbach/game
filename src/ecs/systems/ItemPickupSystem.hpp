
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
					if (!inventory.add(other)) {
						ecs->removeComponent<PositionComponent>(other);
					}
				}
			}
		}
	}
};
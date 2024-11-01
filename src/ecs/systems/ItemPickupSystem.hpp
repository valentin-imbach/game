
#pragma once
#include <vector>
#include "Components.hpp"
#include "System.hpp"
#include "ECS.hpp"
#include "Item.hpp"

class ItemPickupSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& collisions, RealmManager& realmManager) {
		for (Entity entity : entities) {
			InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);

			for (Entity other : collisions[entity]) {
				if (ecs->hasComponent<ItemComponent>(other)) {
					Entity rest = playerComponent.hotbar.add(other);
					rest = inventoryComponent.inventory.add(rest);
					if (!rest) {
						PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(other);
						Realm* realm = realmManager.getRealm(positionComponent.realmId);
						realm->detach(other);
						ecs->removeComponent<PositionComponent>(other);
					}
				}
			}
		}
	}
};
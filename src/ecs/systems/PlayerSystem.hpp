
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class PlayerSystem : public System {
public:
	void update(Camera camera) {
		for (Entity entity : entities) {
			HandComponent& handComonent = ecs->getComponent<HandComponent>(entity);
			InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);
			// FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
			// PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			uchar sel = playerComponent.activeSlot;
			Entity item = inventoryComponent.inventory.itemContainers[sel][0].item;
			handComonent.item = item;
		}
	}
};
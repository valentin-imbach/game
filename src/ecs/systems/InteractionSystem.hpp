
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "utils.hpp"
#include "GuiElement.hpp"

class InteractionSystem : public System {
public:
	std::unique_ptr<GuiElement> update(vec position) {
		for (Entity entity : entities) {
			InteractionComponent& interactionComponent = ecs->getComponent<InteractionComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			if (isInside(position, positionComponent.position, {1.0f, 1.0f})) {
				if (interactionComponent.interactionId == InteractionId::CHEST) {
					InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);

					Sprite sprite = Sprite(SpriteSheet::CHEST, {0, 0}, {10, 10});
					std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);

					int spacing = 20;
					Inventory& inventory = inventoryComponent.inventory;

					for (int y = 0; y < inventory.size.y; y++) {
						for (int x = 0; x < inventory.size.x; x++) {
							pair position = {spacing * x - spacing * (inventory.size.x - 1) / 2, spacing * (y - 1)};
							gui->addGuiElement(std::make_unique<ItemSlot>(position, inventory.itemContainers[x][y]));
						}
					}
					return gui;
				}
			}
		}
		return nullptr;
	}
};

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
					gui->addGuiElement(std::make_unique<InventoryGui>(&inventoryComponent.inventory, 20));
					return gui;
				}
			}
		}
		return nullptr;
	}
};
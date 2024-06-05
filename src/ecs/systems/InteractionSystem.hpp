
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "utils.hpp"
#include "GuiElement.hpp"

class InteractionSystem : public System {
public:
	std::unique_ptr<GuiElement> update(vec position, EntitySet& set, uint ticks) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			StationComponent& stationComponent = ecs->getComponent<StationComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (!vec::inside(position, positionComponent.position, {1.0f, 1.0f})) continue;
	
			if (stationComponent.stationId == StationId::CHEST) {
				Sprite sprite = Sprite(SpriteSheet::CHEST, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);
				gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, 20), &inventoryComponent.inventory, 20));
				return gui;
				
			} else if (stationComponent.stationId == StationId::TOOL_STATION) {
				Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				gui->addGuiElement(std::make_unique<CraftingGui>(pair(0, 10)));
				return gui;
			} else if (stationComponent.stationId == StationId::CAMP_FIRE) {
				ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
				Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(0, 10), processingComponent.input));
				return gui;
			} else if (stationComponent.stationId == StationId::FURNACE) {
				ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
				Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(0, 10), processingComponent.input));
				return gui;
			}
		}
		return nullptr;
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "utils.hpp"
#include "GuiElement.hpp"

class InteractionSystem : public System {

	ROSTER(STATION, POSITION)

	std::unique_ptr<GuiElement> update(vec position, EntitySet& set, uint ticks, InventorySlice link = {}, InventorySlice* out = nullptr) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			StationComponent& stationComponent = ecs->getComponent<StationComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (!vec::inside(position, positionComponent.position, {1.0f, 1.0f})) continue;
	
			if (stationComponent.stationId == StationId::CHEST) {
				Sprite sprite = Sprite(SpriteSheet::CHEST, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);
				gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, 20), &inventoryComponent.inventory, 20, link));
				if (out) *out = &inventoryComponent.inventory;
				return gui;
				
			} else if (stationComponent.stationId == StationId::TOOL_STATION) {
				Sprite sprite = Sprite(SpriteSheet::TOOL_GUI, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);

				InventorySlice slice = {&inventoryComponent.inventory};
				gui->addGuiElement(std::make_unique<InventoryGui>(pair(-40, 20), slice, 20, link));

				gui->addGuiElement(std::make_unique<TextGui>(pair(-60, 0), "2 x"));
				gui->addGuiElement(std::make_unique<TextGui>(pair(-60, 20), "4 x"));
				gui->addGuiElement(std::make_unique<TextGui>(pair(-60, 40), "4 x"));
				if (out) *out = &inventoryComponent.inventory;

				Sprite buttonSprite(SpriteSheet::HAMMER, {0,0}, {1,1});
				Sprite buttonHoverSprite(SpriteSheet::HAMMER, {1,0}, {1,1});
				gui->addGuiElement(std::make_unique<Button>(pair(40, 40), pair(16, 16), nullptr, buttonSprite, buttonHoverSprite));
				
				return gui;
			} else if (stationComponent.stationId == StationId::CAMP_FIRE) {
				FuelComponent& fuelComponent = ecs->getComponent<FuelComponent>(entity);
				ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
				Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(0, -20), fuelComponent.input));
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(-20, 20), processingComponent.input));
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(20, 20), processingComponent.output));
				return gui;
			} else if (stationComponent.stationId == StationId::FURNACE) {
				FuelComponent& fuelComponent = ecs->getComponent<FuelComponent>(entity);
				ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
				Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});
				std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(0, 20), fuelComponent.input));
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(-20, -20), processingComponent.input));
				gui->addGuiElement(std::make_unique<ItemSlot>(pair(20, -20), processingComponent.output));
				Sprite base = Sprite(SpriteSheet::HEAT_BAR, {0, 0}, {1, 5});
				Sprite overlay = Sprite(SpriteSheet::HEAT_BAR, {1, 0}, {1, 5});
				gui->addGuiElement(std::make_unique<ProgressGui>(pair(-50, 0), fuelComponent.heat, base, overlay));

				Sprite flameBase = Sprite(SpriteSheet::HEAT_BAR, {0, 5}, {1, 1});
				Sprite flameOverlay = Sprite(SpriteSheet::HEAT_BAR, {1, 5}, {1, 1});
				gui->addGuiElement(std::make_unique<ProgressGui>(pair(0, 0), fuelComponent.progress, flameBase, flameOverlay));

				Sprite progressBase = Sprite(SpriteSheet::PROGRESS, {0, 0}, {1, 1});
				Sprite progressOverlay = Sprite(SpriteSheet::PROGRESS, {0, 1}, {1, 1});
				gui->addGuiElement(std::make_unique<ProgressGui>(pair(0, -20), processingComponent.process, progressBase, progressOverlay, true));
				return gui;
			}
		}
		return nullptr;
	}
};
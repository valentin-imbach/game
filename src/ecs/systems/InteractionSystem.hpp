
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
				InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(entity);


				Sprite buttonSprite(SpriteSheet::HAMMER, {0,0}, {1,1});
				Sprite buttonHoverSprite(SpriteSheet::HAMMER, {1,0}, {1,1});
				// std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);

				std::unique_ptr<TabWidget> gui = std::make_unique<TabWidget>(pair(0, 0), pair(150, 150));

				Sprite pickSprite = Sprite(SpriteSheet::PICK_AXE_GUI, {0, 0}, {10, 10});
				std::unique_ptr<Widget> pickTab = std::make_unique<Widget>(pair(0, 0), pair(150, 150), pickSprite);
				InventorySlice pickSlice(&inventoryComponent.inventory);
				pickSlice.xrange = pair(0, 1);
				pickTab->addGuiElement(std::make_unique<InventoryGui>(pair(-40, 20), pickSlice, 20, link));
				pickTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 0), "4 x"));
				pickTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 20), "3 x"));
				pickTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 40), "2 x"));
				pickTab->addGuiElement(std::make_unique<Button>(pair(40, 40), pair(16, 16), nullptr, buttonSprite, buttonHoverSprite));

				Sprite axeSprite = Sprite(SpriteSheet::AXE_GUI, {0, 0}, {10, 10});
				std::unique_ptr<Widget> axeTab = std::make_unique<Widget>(pair(0, 0), pair(150, 150), axeSprite);
				InventorySlice axeSlice(&inventoryComponent.inventory);
				axeSlice.xrange = pair(1, 2);
				axeTab->addGuiElement(std::make_unique<InventoryGui>(pair(-40, 20), axeSlice, 20, link));
				axeTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 0), "2 x"));
				axeTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 20), "3 x"));
				axeTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 40), "2 x"));
				axeTab->addGuiElement(std::make_unique<Button>(pair(40, 40), pair(16, 16), nullptr, buttonSprite, buttonHoverSprite));

				std::unique_ptr<Widget> swordTab = std::make_unique<Widget>(pair(0, 0), pair(150, 150), pickSprite);
				InventorySlice swordSlice(&inventoryComponent.inventory);
				swordSlice.xrange = pair(2, 3);
				swordTab->addGuiElement(std::make_unique<InventoryGui>(pair(-40, 20), swordSlice, 20, link));
				swordTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 0), "6 x"));
				swordTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 20), "2 x"));
				swordTab->addGuiElement(std::make_unique<TextGui>(pair(-60, 40), "4 x"));
				swordTab->addGuiElement(std::make_unique<Button>(pair(40, 40), pair(16, 16), nullptr, buttonSprite, buttonHoverSprite));

				gui->addTab(std::move(pickTab));
				gui->addTab(std::move(axeTab));
				gui->addTab(std::move(swordTab));

				if (out) *out = &inventoryComponent.inventory;
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

#pragma once
#include "Events.hpp"
#include "Item.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "GuiElement.hpp"
#include "ECS.hpp"

class GuiManager {
public:
	void update() {
		if (!primary) return;
		primary->reposition();
		primary->update();
		if (!secondary) return;
		secondary->reposition();
		secondary->update();
	}

	void draw() {
		if (!primary) return;
		primary->draw();
		if (secondary) secondary->draw();
		mouseItemContainer.item.draw(mousePosition, GUI_SCALE, ecs);
	}

	bool handleEvent(InputEvent event) {
		if (event.id == InputEventId::INVENTORY) {
			if (primary) {
				close();
				return true;
			}
		}

		if (primary && primary->handleEvent(event)) return true;
		if (secondary && secondary->handleEvent(event)) return true;
		return false;
	}

	void open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b = nullptr) {
		primary = std::move(a);
		secondary = std::move(b);
		if (!primary) return;
		primary->guiManager = this;
		primary->position.y = 0;
		primary->alignment = Direction::NONE;
		if (secondary) {
			secondary->guiManager = this;
			secondary->position.y = 0;
			secondary->alignment = Direction::NONE;

			primary->position.x = -400;
			secondary->position.x = 400;
		} else {
			primary->position.x = 0;
		}
	}

	void close() {
		primary = nullptr;
		secondary = nullptr;
	}

	bool active() {
		return bool(primary);
	}

	ECS* ecs;
	pair mousePosition;
	ItemContainer mouseItemContainer;

private:
	std::unique_ptr<GuiElement> primary;
	std::unique_ptr<GuiElement> secondary;
};
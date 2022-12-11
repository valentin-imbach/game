
#pragma once
#include "Events.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "GuiElement.hpp"

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
		if (!secondary) return;
		secondary->draw();
	}

	bool handleEvent(InputEvent event) {
		if (event == InputEvent::INVENTORY) {
			if (primary) {
				close();
				return true;
			}
		}
		return false;
	}

	void open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b = nullptr) {
		primary = std::move(a);
		secondary = std::move(b);
		if (!primary) return;
		primary->position.y = 0;
		primary->alignment = Direction::NONE;
		if (secondary) {
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

private:
	std::unique_ptr<GuiElement> primary;
	std::unique_ptr<GuiElement> secondary;
};

#include "GuiManager.hpp"

void GuiManager::update() {
	for (auto& guiElement : guiElements) guiElement->reposition();
	for (auto& guiElement : guiElements) guiElement->update(this);
	if (!primary) return;
	primary->reposition();
	primary->update(this);
	if (!secondary) return;
	secondary->reposition();
	secondary->update(this);
}

void GuiManager::draw() {
	for (auto& guiElement : guiElements) guiElement->draw();
	if (!primary) return;
	if (secondary) secondary->draw();
	primary->draw();
	mouseItemContainer.draw(mousePosition, GuiManager::scale);
}

bool GuiManager::handleEvent(InputEvent event) {
	if (event.id == InputEventId::INVENTORY) {
		if (primary) {
			close();
			return true;
		}
	}

	if (primary && primary->handleEvent(event)) return true;
	if (secondary && secondary->handleEvent(event)) return true;
	for (auto& guiElement : guiElements) {
		if (guiElement->handleEvent(event)) return true;
	}
	return false;
}

void GuiManager::open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b) {
	primary = std::move(a);
	secondary = std::move(b);
	if (!primary) return;
	primary->position.y = 0;
	primary->alignment = Direction::NONE;
	if (secondary) {
		secondary->position.y = 0;
		secondary->alignment = Direction::NONE;

		primary->position.x = -80;
		secondary->position.x = 80;
	} else {
		primary->position.x = 0;
	}
}

void GuiManager::close() {
	primary = nullptr;
	secondary = nullptr;
}

bool GuiManager::active() {
	return bool(primary);
}

void GuiManager::add(std::unique_ptr<GuiElement> guiElement) {
	guiElements.push_back(std::move(guiElement));
}
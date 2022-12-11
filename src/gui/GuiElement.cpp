
#include "GuiElement.hpp"
#include "Components.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "ECS.hpp"
#include "GuiManager.hpp"

//* GuiElement

GuiElement::GuiElement(pair position, pair size, Direction alignment) : position(position), size(size), alignment(alignment) {}

void GuiElement::reposition(GuiElement* parent) {
	screenSize = GUI_SCALE * size;
	if (parent) {
		screenPosition = parent->screenPosition + GUI_SCALE * position;
		if (alignment != Direction::NONE) {
			pair step = taxiSteps[int(alignment) - 1];
			screenPosition.x += parent->screenSize.x * step.x / 2;
			screenPosition.y += parent->screenSize.y * step.y / 2;
		}
	} else {
		screenPosition = Window::instance->size / 2;
		if (alignment != Direction::NONE) {
			pair step = taxiSteps[int(alignment) - 1];
			screenPosition.x += Window::instance->size.x * step.x / 2;
			screenPosition.y += Window::instance->size.y * step.y / 2;
		}
	}
}

bool GuiElement::handleEvent(InputEvent event) {
	return false;
}

bool GuiElement::inside(pair pos) {
	return isInside(pos, screenPosition, screenSize);
}

//* Widget

Widget::Widget(pair position, pair size, Sprite sprite) : GuiElement(position, size), sprite(sprite) {
	children = std::vector<std::unique_ptr<GuiElement>>();
}

void Widget::addGuiElement(std::unique_ptr<GuiElement> guiElement) {
	guiElement->guiManager = guiManager;
	children.push_back(std::move(guiElement));
}

void Widget::update() {
	for (auto& guiElement : children) {
		guiElement->reposition(this);
		guiElement->update();
		guiElement->guiManager = guiManager;
	}
}

void Widget::draw() {
	sprite.draw(screenPosition, GUI_SCALE);
	for (auto& guiElement : children) {
		guiElement->draw();
	}
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

bool Widget::handleEvent(InputEvent event) {
	for (auto& guiElement : children) {
		if (guiElement->handleEvent(event)) return true;
	}
	return false;
}

//* ItemSlot

ItemSlot::ItemSlot(pair position, ItemContainer& itemContainer) : GuiElement(position, {18, 18}), itemContainer(itemContainer) {
	sprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
}

void ItemSlot::update() {}

void ItemSlot::draw() {
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (itemContainer.item.entity) {
		SpriteStack& spriteStack = guiManager->ecs->getComponent<SpriteComponent>(itemContainer.item.entity).spriteStack;
		spriteStack.draw(screenPosition, GUI_SCALE);
	}
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

bool ItemSlot::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		LOG("Slot clicked");
		return true;
	}
	return false;
}
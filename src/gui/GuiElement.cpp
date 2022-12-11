
#include "GuiElement.hpp"
#include <memory>
#include <vector>
#include "Components.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "ECS.hpp"

//* GuiElement

GuiElement::GuiElement(pair position, Direction alignment) : position(position), alignment(alignment) {}

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

//* Widget

Widget::Widget(pair position, Sprite sprite) : GuiElement(position), sprite(sprite) {
	children = std::vector<std::unique_ptr<GuiElement>>();
}

void Widget::addGuiElement(std::unique_ptr<GuiElement> guiElement) {
	children.push_back(std::move(guiElement));
}

void Widget::update() {
	for (auto& guiElement : children) {
		guiElement->reposition(this);
		guiElement->update();
	}
}

void Widget::draw() {
	sprite.draw(screenPosition, GUI_SCALE);
	for (auto& guiElement : children) {
		guiElement->draw();
	}
}

//* ItemSlot

ItemSlot::ItemSlot(pair position, ItemContainer& itemContainer, ECS* ecs) : GuiElement(position), itemContainer(itemContainer), ecs(ecs) {
	sprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
}

void ItemSlot::update() {}

void ItemSlot::draw() {
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (itemContainer.item.entity) {
		SpriteStack& spriteStack = ecs->getComponent<SpriteComponent>(itemContainer.item.entity).spriteStack;
		spriteStack.draw(screenPosition, GUI_SCALE);
	}
}
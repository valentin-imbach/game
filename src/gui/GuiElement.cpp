
#include "GuiElement.hpp"
#include "Components.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "direction.hpp"
#include "pair.hpp"
#include "utils.hpp"
#include "ECS.hpp"
#include "GuiManager.hpp"
#include "World.hpp"

//* GuiElement

GuiElement::GuiElement(pair position, pair size, Direction::value alignment) : position(position), size(size), alignment(alignment) {}

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
		screenPosition = Window::instance->size / 2 + GUI_SCALE * position;
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

void ItemSlot::draw() {
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	itemContainer.item.draw(screenPosition, GUI_SCALE, guiManager->ecs);
}

bool ItemSlot::handleEvent(InputEvent event) {
	ItemContainer& mouseItemContainer = guiManager->mouseItemContainer;
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		if (itemContainer.item.itemId != ItemId::NONE && itemContainer.item.itemId == mouseItemContainer.item.itemId) {
			mouseItemContainer.item = itemContainer.add(mouseItemContainer.item);
		} else {
			std::swap(mouseItemContainer.item, itemContainer.item);
		}
		return true;
	} else if (event.id == InputEventId::SECONDARY && inside(event.mousePosition)) {
		if (!itemContainer.item) {
			mouseItemContainer.item = itemContainer.add(mouseItemContainer.item, ItemAmount::ONE);
		} else if (itemContainer.item.itemId != ItemId::NONE && itemContainer.item.itemId == mouseItemContainer.item.itemId) {
			mouseItemContainer.item = itemContainer.add(mouseItemContainer.item, ItemAmount::ONE);
		} else if (!mouseItemContainer.item) {
			itemContainer.item = mouseItemContainer.add(itemContainer.item, ItemAmount::HALF);
		}
		return true;
	}
	return false;
}

//* HotbarGui

HotbarGui::HotbarGui(Entity player) : GuiElement({0, 20}, {150, 30}, Direction::NORTH), player(player) {
	sprite = Sprite(SpriteSheet::HOTBAR, {0, 0}, {10, 2});
	slotSprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
	activeSlotSprite = Sprite(SpriteSheet::SLOT, {2, 0}, {2, 2});
}

void HotbarGui::update() {
	player = guiManager->world->player;
}

void HotbarGui::draw() {
	if (guiManager->active()) return;
	if (!player) return;
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	Inventory& inventory = guiManager->ecs->getComponent<InventoryComponent>(player).inventory;
	uint activeSlot = guiManager->ecs->getComponent<PlayerComponent>(player).activeSlot;
	int spacing = 20 * GUI_SCALE;
	for (int x = 0; x < inventory.size.x; x++) {
		Item& item = inventory.itemContainers[x][0].item;
		pair offset = {spacing * x - spacing * (inventory.size.x - 1) / 2, 0};
		if (x == activeSlot) {
			activeSlotSprite.draw(screenPosition + offset, GUI_SCALE, true);	
		} else {
			slotSprite.draw(screenPosition + offset, GUI_SCALE, true);
		}
		item.draw(screenPosition + offset, GUI_SCALE, guiManager->ecs);
	}
}

//* Health Bar

HealthBarGui::HealthBarGui(Entity player) : GuiElement({-90, 20}, {}, Direction::NORTH_EAST), player(player) {
	heartSprite = Sprite(SpriteSheet::HEART, {0, 0}, {1, 1});
	halfHeartSprite = Sprite(SpriteSheet::HEART, {1, 0}, {1, 1});
}

void HealthBarGui::update() {
	player = guiManager->world->player;
}

void HealthBarGui::draw() {
	if (!player) return;
	HealthComponent& healthComponent = guiManager->ecs->getComponent<HealthComponent>(player);
	int spacing = 9 * GUI_SCALE;
	for (int x = 0; x < healthComponent.health / 2; x++) {
		pair offset = {x * spacing, 0};
		heartSprite.draw(screenPosition + offset, GUI_SCALE, guiManager->ecs);
	}
	if (healthComponent.health % 2) {
		pair offset = {healthComponent.health / 2 * spacing, 0};
		halfHeartSprite.draw(screenPosition + offset, GUI_SCALE, guiManager->ecs);
	}
}
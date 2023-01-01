
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
		guiElement->guiManager = guiManager;
		guiElement->reposition(this);
		guiElement->update();
	}
}

void Widget::draw() {
	sprite.draw(screenPosition, GUI_SCALE);
	for (int i = children.size() - 1; i >= 0; i--) {
		children[i]->draw();
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

ItemSlot::ItemSlot(pair position, ItemContainer& itemContainer, Inventory* link) : GuiElement(position, {18, 18}), itemContainer(itemContainer), link(link) {
	sprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
}

void ItemSlot::draw() {
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	itemContainer.draw(screenPosition, GUI_SCALE);
	if (inside(guiManager->mousePosition)) {
		pair infoPosition = {screenPosition.x + 35, screenPosition.y - 30};
		itemContainer.drawInfo(infoPosition, guiManager->world->inputState[InputStateId::ALTER]);
	}
}

bool ItemSlot::handleEvent(InputEvent event) {
	ItemContainer& mouseItemContainer = guiManager->mouseItemContainer;
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		if (guiManager->world->inputState[InputStateId::ALTER] && link) {
			itemContainer.item = link->add(itemContainer.item);
			return true;
		}
		if (!itemContainer.item || !mouseItemContainer.item) {
			std::swap(mouseItemContainer.item, itemContainer.item);
			return true;
		}
		ItemComponent& itemComponent = guiManager->ecs->getComponent<ItemComponent>(itemContainer.item);
		ItemComponent& mouseItemComponent = guiManager->ecs->getComponent<ItemComponent>(mouseItemContainer.item);

		if (itemComponent.itemId != ItemId::NONE && itemComponent.itemId == mouseItemComponent.itemId) {
			mouseItemContainer.item = itemContainer.add(mouseItemContainer.item);
		} else {
			std::swap(mouseItemContainer.item, itemContainer.item);
		}
		return true;
	} else if (event.id == InputEventId::SECONDARY && inside(event.mousePosition)) {
		if (!mouseItemContainer.item) {
			itemContainer.item = mouseItemContainer.add(itemContainer.item, ItemAmount::HALF);
			return true;
		}
		mouseItemContainer.item = itemContainer.add(mouseItemContainer.item, ItemAmount::ONE);
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
	if (guiManager->active() || !player) return;
	sprite.draw(screenPosition, GUI_SCALE, true);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	Inventory& inventory = guiManager->ecs->getComponent<PlayerComponent>(player).hotbar;
	uint activeSlot = guiManager->ecs->getComponent<PlayerComponent>(player).activeSlot;
	int spacing = 20 * GUI_SCALE;
	for (int x = 0; x < inventory.size.x; x++) {
		pair offset = {spacing * x - spacing * (inventory.size.x - 1) / 2, 0};
		if (x == activeSlot) {
			activeSlotSprite.draw(screenPosition + offset, GUI_SCALE, true);
		} else {
			slotSprite.draw(screenPosition + offset, GUI_SCALE, true);
		}
		inventory.itemContainers[x][0].draw(screenPosition + offset, GUI_SCALE);
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

//* InventoryGui

InventoryGui::InventoryGui(pair position, Inventory* inventory, int spacing, Inventory* link) : Widget(position, spacing * inventory->size, Sprite()), inventory(inventory), spacing(spacing), link(link) {
	for (int x = 0; x < inventory->size.x; x++) {
		for (int y = 0; y < inventory->size.y; y++) {
			pair position(spacing * x - spacing * (inventory->size.x - 1) / 2, spacing * y - spacing * (inventory->size.y - 1) / 2);
			addGuiElement(std::make_unique<ItemSlot>(position, inventory->itemContainers[x][y], link));
		}
	}
}

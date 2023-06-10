
#include "GuiElement.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "GuiManager.hpp"
#include "Item.hpp"
#include "ResourceTemplates.hpp"
#include "Sprite.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "direction.hpp"
#include "pair.hpp"
#include "utils.hpp"
#include "TextManager.hpp"

#include "Crafting.hpp"

//* GuiElement

uint GuiManager::scale = 3;
GuiElement::GuiElement(pair position, pair size, Direction::value alignment)
	: position(position), size(size), alignment(alignment) {}

void GuiElement::reposition(GuiElement* parent) {
	screenSize = GuiManager::scale * size;
	pair parentPosition = parent ? parent->screenPosition : Window::instance->size / 2;
	pair parentSize = parent ? parent->screenSize : Window::instance->size;
	screenPosition = parentPosition + GuiManager::scale * position;
	pair step = Direction::taxi[alignment];
	screenPosition.x += parentSize.x * step.x / 2;
	screenPosition.y += parentSize.y * step.y / 2;
}

bool GuiElement::handleEvent(InputEvent event) {
	return false;
}

bool GuiElement::inside(pair pos) {
	return isInside(pos, screenPosition, screenSize);
}

//* Widget

Widget::Widget(pair position, pair size, Sprite sprite)
	: GuiElement(position, size), sprite(sprite) {
	children = std::vector<std::unique_ptr<GuiElement>>();
}

void Widget::addGuiElement(std::unique_ptr<GuiElement> guiElement) {
	children.push_back(std::move(guiElement));
}

void Widget::update(GuiManager* manager) {
	guiManager = manager;
	for (auto& guiElement : children) {
		guiElement->reposition(this);
		guiElement->update(guiManager);
	}
}

void Widget::draw() {
	sprite.draw(screenPosition, GuiManager::scale);
	for (int i = children.size() - 1; i >= 0; i--) children[i]->draw();
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

bool Widget::handleEvent(InputEvent event) {
	for (auto& guiElement : children) {
		if (guiElement->handleEvent(event)) return true;
	}
	return false;
}

//* Tab

Tab::Tab(TabWidget* parent, uint index)
	: GuiElement(pair(25 + 40 * index, 11), {24, 24}, Direction::NORTH_WEST), parent(parent), index(index) {}

bool Tab::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) parent->selectTab(index);
	return false;
}

void Tab::draw() {
	bool selected = parent->selected == index;
	Sprite sprite = Sprite(SpriteSheet::TAB, {2 * selected, 0}, {2, 2});
	sprite.draw(screenPosition, GuiManager::scale);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

//* TabWidget

TabWidget::TabWidget(pair position, pair size)
	: Widget(position, size, Sprite()) {}

void TabWidget::draw() {
	tabs[selected]->draw();
	for (int i = children.size() - 1; i >= 0; i--) children[i]->draw();
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

void TabWidget::addTab(std::unique_ptr<GuiElement> guiElement) {
	uint index = tabs.size();
	tabs.push_back(std::move(guiElement));
	pair position(index * 50, 0);
	addGuiElement(std::make_unique<Tab>(this, index));
}

void TabWidget::selectTab(uint sel) {
	if (selected < tabs.size()) selected = sel;
}

void TabWidget::update(GuiManager* manager) {
	guiManager = manager;
	for (auto& guiElement : children) {
		guiElement->reposition(this);
		guiElement->update(guiManager);
	}
	for (auto& tab : tabs) {
		tab->reposition(this);
		tab->update(guiManager);
	}
}

bool TabWidget::handleEvent(InputEvent event) {
	for (auto& guiElement : children) {
		if (guiElement->handleEvent(event)) return true;
	}
	return tabs[selected]->handleEvent(event);
}

//* ItemSlot

ItemSlot::ItemSlot(pair position, ItemContainer& itemContainer, Inventory* link)
	: GuiElement(position, {18, 18}), itemContainer(itemContainer), link(link) {
	sprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
}

void ItemSlot::draw() {
	sprite.draw(screenPosition, GuiManager::scale);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	itemContainer.draw(screenPosition, GuiManager::scale);
	if (inside(guiManager->mousePosition)) {
		pair infoPosition(screenPosition.x + 35, screenPosition.y - 30);
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
		ItemComponent& itemComponent = guiManager->world->ecs.getComponent<ItemComponent>(itemContainer.item);
		ItemComponent& mouseItemComponent = guiManager->world->ecs.getComponent<ItemComponent>(mouseItemContainer.item);

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

HotbarGui::HotbarGui(Entity player)
	: GuiElement({0, 20}, {150, 30}, Direction::NORTH), player(player) {
	sprite = Sprite(SpriteSheet::HOTBAR, {0, 0}, {10, 2});
	slotSprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
	activeSlotSprite = Sprite(SpriteSheet::SLOT, {2, 0}, {2, 2});
}

void HotbarGui::update(GuiManager* manager) {
	guiManager = manager;
	player = guiManager->world->player;
}

void HotbarGui::draw() {
	if (guiManager->active() || !player) return;
	sprite.draw(screenPosition, GuiManager::scale);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	Inventory& inventory = guiManager->world->ecs.getComponent<PlayerComponent>(player).hotbar;
	uint activeSlot = guiManager->world->ecs.getComponent<PlayerComponent>(player).activeSlot;
	int spacing = 20 * GuiManager::scale;
	for (int x = 0; x < inventory.size.x; x++) {
		pair offset(spacing * x - spacing * (inventory.size.x - 1) / 2, 0);
		if (x == activeSlot)
			activeSlotSprite.draw(screenPosition + offset, GuiManager::scale);
		else
			slotSprite.draw(screenPosition + offset, GuiManager::scale);
		inventory.itemContainers[x][0].draw(screenPosition + offset, GuiManager::scale);
	}
}

//* Health Bar

HealthBarGui::HealthBarGui(Entity player)
	: GuiElement({-90, 20}, {}, Direction::NORTH_EAST), player(player) {
	heartSprite = Sprite(SpriteSheet::HEART, {0, 0}, {1, 1});
	halfHeartSprite = Sprite(SpriteSheet::HEART, {1, 0}, {1, 1});
}

void HealthBarGui::update(GuiManager* manager) {
	guiManager = manager;
	player = guiManager->world->player;
}

void HealthBarGui::draw() {
	if (!player) return;
	HealthComponent& healthComponent = guiManager->world->ecs.getComponent<HealthComponent>(player);
	int spacing = 9 * GuiManager::scale;
	for (int x = 0; x < healthComponent.health / 2; x++) {
		pair offset(x * spacing, 0);
		heartSprite.draw(screenPosition + offset, GuiManager::scale);
	}
	if (healthComponent.health % 2) {
		pair offset(healthComponent.health / 2 * spacing, 0);
		halfHeartSprite.draw(screenPosition + offset, GuiManager::scale);
	}
}

//* InventoryGui

InventoryGui::InventoryGui(pair position, Inventory* inventory, int spacing, Inventory* link)
	: Widget(position, spacing * inventory->size, Sprite()), inventory(inventory), spacing(spacing), link(link) {
	for (int x = 0; x < inventory->size.x; x++) {
		for (int y = 0; y < inventory->size.y; y++) {
			pair position(spacing * x - spacing * (inventory->size.x - 1) / 2, spacing * y - spacing * (inventory->size.y - 1) / 2);
			addGuiElement(std::make_unique<ItemSlot>(position, inventory->itemContainers[x][y], link));
		}
	}
}

//* CraftingGui

CraftingGui::CraftingGui(pair position, Inventory* link)
	: Widget(position, {80, 20}, Sprite()), link(link) {
	addGuiElement(std::make_unique<ItemSlot>(pair(-30, 0), inputA, link));
	addGuiElement(std::make_unique<ItemSlot>(pair(-10, 0), inputB, link));
	addGuiElement(std::make_unique<ItemSlot>(pair(30, 0), output, link));
	addGuiElement(std::make_unique<Button>(pair(10, 0), pair(20, 20), std::bind(&CraftingGui::craft, this), Sprite()));
}

CraftingGui::~CraftingGui() {
	if (link) {
		inputA.item = link->add(inputA.item);
		inputB.item = link->add(inputB.item);
		output.item = link->add(output.item);
	}
	vec pos = guiManager->world->ecs.getComponent<PositionComponent>(guiManager->world->player).position;
	guiManager->world->ecs.addComponent<PositionComponent>({pos}, inputA.item);
	guiManager->world->ecs.addComponent<PositionComponent>({pos}, inputB.item);
	guiManager->world->ecs.addComponent<PositionComponent>({pos}, output.item);
}

void CraftingGui::craft() {
	for (auto& recipe : CraftingRecipe::recipes) {
		if (recipe->ingredients[0].check(inputA.item) && recipe->ingredients[1].check(inputB.item)) {
			inputA.item = recipe->ingredients[0].take(inputA.item);
			inputB.item = recipe->ingredients[0].take(inputB.item);
			output.item = EntityFactory::createItem(recipe->product.itemId, recipe->product.count);
		}
	}
}

//* BuildGui

BuildGui::BuildGui(pair position)
	: Widget(position, {144, 128}, Sprite()) {
	std::unique_ptr<Selector> selector = std::make_unique<Selector>(pair(35, 0), pair(60, 100), std::bind(&BuildGui::select, this, std::placeholders::_1), 3, Direction::WEST);
	addGuiElement(std::make_unique<Button>(pair(-20, -20), pair(20,20), std::bind(&BuildGui::build, this), Sprite(), "", Direction::SOUTH_EAST));

	for (int n = 1; n < StationId::count; n++) {
		SpriteStack sprites;
		sprites.addSprite({SpriteSheet::STATION_ICONS, {n - 1, 0}, {1, 1}}, {0, 0});
		selector->addSelection(sprites);
	}
	addGuiElement(std::move(selector));
}

void BuildGui::select(int n) {
	selected = n;
}

void BuildGui::build() {
	ECS& ecs = guiManager->world->ecs;
	guiManager->buildMode = EntityFactory::createStation(StationId::from_int(selected + 1), {0, 0});
	ecs.getComponent<SpriteComponent>(guiManager->buildMode).z = 0.5f;
	guiManager->close();
}

//* Button

Button::Button(pair position, pair size, std::function<void()> callback, Sprite sprite, std::string text, Direction::value alignment)
	: GuiElement(position, size, alignment), sprite(sprite), callback(callback), text(text) {}

bool Button::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		callback();
		return true;
	}
	return false;
}

void Button::draw() {
	TextManager::drawText(text, screenPosition, true);
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

//* Selector

Selector::Selector(pair position, pair size, std::function<void(int)> callback, int columns, Direction::value alignment)
	: GuiElement(position, size, alignment), callback(callback), columns(columns) {}

void Selector::addSelection(SpriteStack sprite) {
	sprites.push_back(sprite);
}

bool Selector::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		int offset = screenSize.x / columns;
		pair index = (event.mousePosition - screenPosition + screenSize / 2) / offset;
		int n = index.x + columns * index.y;
		if (n < sprites.size()) {
			callback(n);
			selected = n;
			return true;
		}
	}
	return false;
}

void Selector::draw() {
	int offset = screenSize.x / columns;
	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	Sprite slot(SpriteSheet::SLOT, {0, 2}, {2,2});
	Sprite slot2(SpriteSheet::SLOT, {2, 2}, {2,2});
	for (int i = 0; i < sprites.size(); i++) {
		pair index = pair(i % columns, i / columns);
		pair pos = screenPosition - screenSize / 2 + offset * index + pair(offset / 2, offset / 2);
		if (selected == i) {
			slot2.draw(pos, GuiManager::scale);
		} else {
			slot.draw(pos, GuiManager::scale);
		}
		sprites[i].draw(pos, GuiManager::scale);
		if (GUI_BOX) TextureManager::drawRect(pos, {offset, offset});
	}
}

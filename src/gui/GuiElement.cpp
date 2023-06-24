
#include "GuiElement.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "GuiManager.hpp"
#include "Item.hpp"
#include "ResourceTemplates.hpp"
#include "Sprite.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "direction.hpp"
#include "pair.hpp"
#include "utils.hpp"

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

void GuiElement::update(GuiManager *manager) {
	guiManager = manager;
}

void GuiElement::draw() {
	if (hover && hoverSprite) {
		hoverSprite.draw(screenPosition, GuiManager::scale);
	} else if (sprite) {
		sprite.draw(screenPosition, GuiManager::scale);
	}

	if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
}

bool GuiElement::handleEvent(InputEvent event) {
	if (event.id == InputEventId::RESET) {
		hover = false;
		keys = false;
		return false;
	} else if (event.id == InputEventId::HOVER && inside(event.mousePosition)) {
		hover = true;
		return absorbHover;
	} else if (event.id == InputEventId::STATE) {
		keys = true;
		return absorbKeys;
	}
	return false;
}

bool GuiElement::inside(pair pos) {
	return pair::inside(pos, screenPosition, screenSize);
}

//* Widget

Widget::Widget(pair position, pair size, Sprite sprite) : GuiElement(position, size) {
	children = std::vector<std::unique_ptr<GuiElement>>();
	this->sprite = sprite;
}

void Widget::addGuiElement(std::unique_ptr<GuiElement> guiElement) {
	children.push_back(std::move(guiElement));
}

void Widget::removeGuiElement() {
	children.pop_back();
}

void Widget::update(GuiManager* manager) {
	GuiElement::update(manager);
	for (auto& guiElement : children) {
		guiElement->reposition(this);
		guiElement->update(guiManager);
	}
}

void Widget::draw() {
	GuiElement::draw();
	for (int i = children.size() - 1; i >= 0; i--) children[i]->draw();
}

bool Widget::handleEvent(InputEvent event) {
	for (auto& guiElement : children) {
		if (guiElement->handleEvent(event)) return true;
	}
	return (GuiElement::handleEvent(event));
}

//* Tab

Tab::Tab(TabWidget* parent, uint index)
	: GuiElement(pair(25 + 40 * index, 11), pair(24, 24), Direction::NORTH_WEST), parent(parent), index(index) {}

bool Tab::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		parent->selectTab(index);
		return true;
	}
	return GuiElement::handleEvent(event);
}

void Tab::draw() {
	bool selected = parent->selected == index;
	sprite = Sprite(SpriteSheet::TAB, pair(2 * selected, 0), pair(2, 2));
	GuiElement::draw();
}

//* TabWidget

TabWidget::TabWidget(pair position, pair size)
	: Widget(position, size) {}

void TabWidget::draw() {
	tabs[selected]->draw();
	Widget::draw();
}

void TabWidget::addTab(std::unique_ptr<GuiElement> guiElement) {
	uint index = tabs.size();
	tabs.push_back(std::move(guiElement));
	addGuiElement(std::make_unique<Tab>(this, index));
}

void TabWidget::selectTab(uint n) {
	if (selected < tabs.size()) selected = n;
}

void TabWidget::update(GuiManager* manager) {
	Widget::update(manager);
	for (auto& tab : tabs) {
		tab->reposition(this);
		tab->update(guiManager);
	}
}

bool TabWidget::handleEvent(InputEvent event) {
	if (tabs[selected]->handleEvent(event)) return true;
	return Widget::handleEvent(event);
}

//* ItemSlot

ItemSlot::ItemSlot(pair position, ItemContainer& itemContainer, Inventory* link)
	: GuiElement(position, pair(18, 18)), link(link), itemContainer(itemContainer) {
	sprite = Sprite(SpriteSheet::SLOT, pair(0, 0), pair(2, 2));
	hoverSprite = Sprite(SpriteSheet::SLOT, pair(2, 0), pair(2, 2));
}

void ItemSlot::draw() {
	GuiElement::draw();
	itemContainer.draw(screenPosition, GuiManager::scale);
	if (hover) {
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
		if (itemContainer.itemKind && mouseItemContainer.item && !hasItemKind(mouseItemContainer.item, itemContainer.itemKind)) return false;
		if (itemContainer.output && mouseItemContainer.item) return false;

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
		if (itemContainer.itemKind && mouseItemContainer.item && !hasItemKind(mouseItemContainer.item, itemContainer.itemKind)) return false;

		if (!mouseItemContainer.item) {
			itemContainer.item = mouseItemContainer.add(itemContainer.item, ItemAmount::HALF);
			return true;
		}
		mouseItemContainer.item = itemContainer.add(mouseItemContainer.item, ItemAmount::ONE);
		return true;
	}
	return GuiElement::handleEvent(event);
}

//* HotbarGui

HotbarGui::HotbarGui(Entity player)
	: GuiElement(pair(0, 20), pair(150, 30), Direction::NORTH), player(player) {
	sprite = Sprite(SpriteSheet::HOTBAR, {0, 0}, {10, 2});
	slotSprite = Sprite(SpriteSheet::SLOT, {0, 0}, {2, 2});
	activeSlotSprite = Sprite(SpriteSheet::SLOT, {2, 0}, {2, 2});
}

void HotbarGui::update(GuiManager* manager) {
	GuiElement::update(manager);
	player = guiManager->world->player;
}

void HotbarGui::draw() {
	if (guiManager->active() || !player) return;
	GuiElement::draw();
	Inventory& inventory = guiManager->world->ecs.getComponent<PlayerComponent>(player).hotbar;
	uint activeSlot = guiManager->world->ecs.getComponent<PlayerComponent>(player).activeSlot;
	int spacing = 20 * GuiManager::scale;
	for (uint x = 0; x < inventory.size.x; x++) {
		pair offset(spacing * x - spacing * (inventory.size.x - 1) / 2, 0);
		if (x == activeSlot) {
			activeSlotSprite.draw(screenPosition + offset, GuiManager::scale);
		} else {
			slotSprite.draw(screenPosition + offset, GuiManager::scale);
		}
		inventory.itemContainers[x][0].draw(screenPosition + offset, GuiManager::scale);
	}
}

//* Health Bar

HealthBarGui::HealthBarGui(Entity player)
	: GuiElement(pair(-90, 20), pair(0, 0), Direction::NORTH_EAST), player(player) {
	heartSprite = Sprite(SpriteSheet::HEART, {0, 0}, {1, 1});
	halfHeartSprite = Sprite(SpriteSheet::HEART, {1, 0}, {1, 1});
}

void HealthBarGui::update(GuiManager* manager) {
	GuiElement::update(manager);
	player = guiManager->world->player;
}

void HealthBarGui::draw() {
	if (!player) return;
	GuiElement::draw();
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
	: Widget(position, spacing * inventory->size), inventory(inventory), link(link), spacing(spacing) {
	for (int x = 0; x < inventory->size.x; x++) {
		for (int y = 0; y < inventory->size.y; y++) {
			pair position(spacing * x - spacing * (inventory->size.x - 1) / 2, spacing * y - spacing * (inventory->size.y - 1) / 2);
			addGuiElement(std::make_unique<ItemSlot>(position, inventory->itemContainers[x][y], link));
		}
	}
}

//* CraftingGui

CraftingGui::CraftingGui(pair position, Inventory* link)
	: Widget(position, {144, 128}), link(link) {
	std::unique_ptr<Selector> selector = std::make_unique<Selector>(pair(35, 0), pair(60, 100), std::bind(&CraftingGui::select, this, std::placeholders::_1), 3, Direction::WEST);
	for (uint n = 1; n < CraftingRecipeId::count; n++) {
		SpriteStack sprites;
		sprites.addSprite({SpriteSheet::CRAFTING_ICONS, pair(n - 1, 0), pair(1, 1)}, pair(0, 0));
		selector->addSelection(sprites);
	}
	addGuiElement(std::move(selector));
}

void CraftingGui::select(int n) {
	if (craftingGrid) removeGuiElement();
	std::unique_ptr<CraftingGrid> grid = std::make_unique<CraftingGrid>(pair(40, 0), CraftingRecipeId::from_int(n+1), link);
	craftingGrid = grid.get();
	addGuiElement(std::move(grid));
}

//* CraftingGrid

CraftingGrid::CraftingGrid(pair position, CraftingRecipeId::value recipeId, Inventory* link)
	: Widget(position, {40, 0}), recipeId(recipeId), link(link) {

	CraftingKindRecipe& recipe = CraftingKindRecipe::recipes[recipeId];
	arity = recipe.ingredients.size();
	size.y = 20 * (2 + arity);
	inputs = std::vector<ItemContainer>(arity);
	output.output = true;

	for (uint i = 0; i < arity; i++) {
		inputs[i].itemKind = recipe.ingredients[i].itemKind;
		int y = i * 20 - 10 * (1 + arity);
		addGuiElement(std::make_unique<ItemSlot>(pair(10, y), inputs[i], link));
		std::string text = std::to_string(recipe.ingredients[i].count) + " x";
		addGuiElement(std::make_unique<TextGui>(pair(-10, y), text));
	}

	addGuiElement(std::make_unique<ItemSlot>(pair(10, 10 * (arity + 1)), output, link));
	Sprite buttonSprite(SpriteSheet::HAMMER, {0,0}, {1,1});
	Sprite buttonHoverSprite(SpriteSheet::HAMMER, {1,0}, {1,1});
	addGuiElement(std::make_unique<Button>(pair(10, 10 * (arity - 1)), pair(16, 16), std::bind(&CraftingGrid::craft, this), buttonSprite, buttonHoverSprite));
}

CraftingGrid::~CraftingGrid() {
	if (link) {
		for (uint i = 0; i < inputs.size(); i++) inputs[i].item = link->add(inputs[i].item);
		output.item = link->add(output.item);
	}
	vec pos = guiManager->world->ecs.getComponent<PositionComponent>(guiManager->world->player).position;
	for (uint i = 0; i < inputs.size(); i++) guiManager->world->ecs.addComponent<PositionComponent>({pos}, inputs[i].item);
	guiManager->world->ecs.addComponent<PositionComponent>({pos}, output.item);
}

void CraftingGrid::craft() {
	CraftingKindRecipe recipe = CraftingKindRecipe::recipes[recipeId];
	for (int i = 0; i < arity; i++) {
		if (!recipe.ingredients[i].check(inputs[i].item)) return;
	}

	for (int i = 0; i < arity; i++) inputs[i].item = recipe.ingredients[i].take(inputs[i].item);
	
	Entity item = EntityFactory::world->ecs.createEntity();
	SpriteStack spriteStack;
	uint seed = 123;
	for (SpriteTemplate& sprite : recipe.product.spriteTemplates) {
		uint var = noise::Int(seed++, 0, sprite.variations);
		pair spritePosition(sprite.anker.x + var * sprite.size.x, sprite.anker.y);
		spriteStack.addSprite({SpriteSheet::ITEMS, spritePosition, sprite.size}, sprite.offset);
	}
	EntityFactory::world->ecs.addComponent<SpriteComponent>({spriteStack, 0.5f}, item);
	EntityFactory::world->ecs.getComponent<SpriteComponent>(item).effects[SpriteEffectId::BOUNCE] = {true, 0};
	EntityFactory::world->ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, item);
	ItemKindComponent itemKindComponent = {};

	for (auto& productKind : recipe.product.productKinds) itemKindComponent.itemKinds[productKind] = true;
	for (auto& productProperty : recipe.product.productProperties) {
		assert(productProperty.factors.size() == arity);

		int total = 0;
		int weight = 0;

		for (int i = 0; i < arity; i++) {
			int w = productProperty.factors[i].second;
			int stat = getItemProperty(inputs[i].item, productProperty.factors[i].first);
			if (w < 0) stat = 10 - stat;
			total += stat * std::abs(w);
			weight += std::abs(w);
		}

		assert(weight != 0);
		itemKindComponent.itemProperties[productProperty.property] = total / weight;
	}

	EntityFactory::world->ecs.addComponent<ItemKindComponent>(itemKindComponent, item);
	Collider itemCollider(vec(0, 0), vec(0.4f, 0.4f));
	EntityFactory::world->ecs.addComponent<ColliderComponent>({itemCollider}, item);
	EntityFactory::world->ecs.addComponent<NameComponent>({Textblock(recipe.product.name)}, item);

	output.item = item;
	return;
}

//* BuildGui

BuildGui::BuildGui(pair position)
	: Widget(position, pair(144, 128)) {
	std::unique_ptr<Selector> selector = std::make_unique<Selector>(pair(35, 0), pair(60, 100), std::bind(&BuildGui::select, this, std::placeholders::_1), 3, Direction::WEST);
	addGuiElement(std::make_unique<Button>(pair(-20, -20), pair(20, 20), std::bind(&BuildGui::build, this), Sprite(), Sprite(), "", Direction::SOUTH_EAST));

	for (uint n = 1; n < StationId::count; n++) {
		SpriteStack sprites;
		sprites.addSprite({SpriteSheet::STATION_ICONS, pair(n - 1, 0), pair(1, 1)}, pair(0, 0));
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

Button::Button(pair position, pair size, std::function<void()> callback, Sprite sprite, Sprite hoverSprite, std::string text, Direction::value alignment)
	: GuiElement(position, size, alignment), text(text), callback(callback) {
		this->sprite = sprite;
		this->hoverSprite = hoverSprite;
}

bool Button::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		callback();
		return true;
	}
	return GuiElement::handleEvent(event);
}

void Button::draw() {
	GuiElement::draw();
	if (!text.empty()) {
		if (hover) {
			TextManager::drawText(text, screenPosition, true, {150,150,255,255});
		} else {
			TextManager::drawText(text, screenPosition, true);
		}
	}
}

//* Selector

Selector::Selector(pair position, pair size, std::function<void(int)> callback, int columns, Direction::value alignment)
	: GuiElement(position, size, alignment), columns(columns), callback(callback) {}

void Selector::addSelection(SpriteStack sprite) {
	sprites.push_back(sprite);
}

bool Selector::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
		int offset = screenSize.x / columns;
		pair index = (event.mousePosition - screenPosition + screenSize / 2) / offset;
		uint n = index.x + columns * index.y;
		if (n < sprites.size()) {
			callback(n);
			selected = n;
			return true;
		}
	}
	return GuiElement::handleEvent(event);
}

void Selector::draw() {
	int offset = screenSize.x / columns;
	GuiElement::draw();
	Sprite slot(SpriteSheet::SLOT, {0, 2}, {2, 2});
	Sprite slot2(SpriteSheet::SLOT, {2, 2}, {2, 2});
	for (uint i = 0; i < sprites.size(); i++) {
		pair index = pair(i % columns, i / columns);
		pair pos = screenPosition - screenSize / 2 + offset * index + pair(offset / 2, offset / 2);
		(selected == i ? slot2 : slot).draw(pos, GuiManager::scale);
		sprites[i].draw(pos, GuiManager::scale);
		if (GUI_BOX) TextureManager::drawRect(pos, {offset, offset});
	}
}

//* TextGui

TextGui::TextGui(pair position, std::string text, Direction::value alignment) : GuiElement(position, {0,0}, alignment), text(text) {
	size = TextManager::textSize(text) / GuiManager::scale;
}

void TextGui::draw() {
	GuiElement::draw();
	TextManager::drawText(text, screenPosition, true);
}

//* TextField


TextField::TextField(pair position, pair size, Direction::value alignment) : GuiElement(position, size, alignment) {}

void TextField::draw() {
	GuiElement::draw();
    TextManager::drawText(text, screenPosition, true);
}

bool TextField::handleEvent(InputEvent event) {
	if (event.id == InputEventId::PRIMARY) {
		if (inside(event.mousePosition)) {
			active = true;
			return true;
		} else {
			active = false;
			return false;
		}
	} else if (event.id == InputEventId::ESCAPE) {
		active = false;
		return false;
	} else if (event.id == InputEventId::TEXT) {
		if (active) {
			text += event.text;
			return true;
		}
	} else if (event.id == InputEventId::BACKSPACE) {
		if (active && text.length() > 0) {
			text = text.substr(0, text.length() - 1);
			return true;
		}
	}
	return GuiElement::handleEvent(event);
}

std::string TextField::getText() {
	return text;
}
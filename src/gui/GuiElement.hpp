
#pragma once
#include <memory>
#include "Events.hpp"
#include "Item.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "ECS_types.hpp"

class GuiManager;

#define GUI_SCALE 3
#define GUI_BOX true

class GuiElement {
public:
	GuiElement(pair position, pair size, Direction::value alignment = Direction::NONE);
	virtual ~GuiElement() = default;
	void reposition(GuiElement* parent = nullptr);
	virtual void update() {};
	virtual void draw() = 0;
	virtual bool handleEvent(InputEvent event);

protected:
	pair size;
	pair screenSize;
	pair position;
	pair screenPosition;
	Direction::value alignment;
	GuiManager* guiManager;
	bool inside(pair position);

	friend class Widget;
	friend class GuiManager;
};

class Widget : public GuiElement {
public:
	Widget(pair position, pair size, Sprite sprite);
	~Widget() override = default;
	void update() override;
	void draw() override;
	void addGuiElement(std::unique_ptr<GuiElement> guiElement);
	bool handleEvent(InputEvent event) override;

protected:
	Sprite sprite;
	std::vector<std::unique_ptr<GuiElement>> children;
};

class ECS;

class ItemSlot : public GuiElement {
public:
	ItemSlot(pair position, ItemContainer& itemContainer, Inventory* link = nullptr);
	~ItemSlot() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
	Inventory* link;

private:
	Sprite sprite;
	ItemContainer& itemContainer;
};

class HotbarGui : public GuiElement {
public:
	HotbarGui(Entity player);
	~HotbarGui() override = default;
	void update() override;
	void draw() override;

private:
	Entity player;
	Sprite sprite;
	Sprite slotSprite;
	Sprite activeSlotSprite;
};

class HealthBarGui : public GuiElement {
public:
	HealthBarGui(Entity player);
	~HealthBarGui() override = default;
	void update() override;
	void draw() override;

private:
	Entity player;
	Sprite heartSprite;
	Sprite halfHeartSprite;
};

class InventoryGui : public Widget {
public:
	InventoryGui(pair position, Inventory* inventory, int spacing, Inventory* link = nullptr);
	~InventoryGui() override = default;
	Inventory* link;
private:
	Inventory* inventory;
	int spacing;
};
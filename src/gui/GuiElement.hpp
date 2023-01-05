
#pragma once
#include "ECS_types.hpp"
#include "Events.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "utils.hpp"
#include <memory>

class GuiManager;

#define GUI_BOX true

class GuiElement {
public:
	GuiElement(pair position, pair size, Direction::value alignment = Direction::NONE);
	virtual ~GuiElement() = default;
	void reposition(GuiElement *parent = nullptr);
	virtual void update(GuiManager *manager) { guiManager = manager; };
	virtual void draw() = 0;
	virtual bool handleEvent(InputEvent event);

protected:
	pair size;
	pair screenSize;
	pair position;
	pair screenPosition;
	Direction::value alignment;
	GuiManager *guiManager;
	bool inside(pair position);

	friend class Widget;
	friend class GuiManager;
};

class Widget : public GuiElement {
public:
	Widget(pair position, pair size, Sprite sprite);
	~Widget() override = default;
	void update(GuiManager *manager) override;
	void draw() override;
	void addGuiElement(std::unique_ptr<GuiElement> guiElement);
	bool handleEvent(InputEvent event) override;

protected:
	Sprite sprite;
	std::vector<std::unique_ptr<GuiElement>> children;
};

class TabWidget;

class Tab : public GuiElement {
public:
	Tab(TabWidget *parent, uint index);
	~Tab() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;

private:
	TabWidget *parent;
	uint index;
};

class TabWidget : public Widget {
public:
	TabWidget(pair position, pair size);
	~TabWidget() override = default;
	void draw() override;
	void update(GuiManager *manager) override;
	void addTab(std::unique_ptr<GuiElement> guiElement);
	void selectTab(uint selected);
	bool handleEvent(InputEvent event) override;

private:
	uint selected = 0;
	std::vector<std::unique_ptr<GuiElement>> tabs;
	friend class Tab;
};

class ECS;

class ItemSlot : public GuiElement {
public:
	ItemSlot(pair position, ItemContainer &itemContainer,
			 Inventory *link = nullptr);
	~ItemSlot() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
	Inventory *link;

private:
	Sprite sprite;
	ItemContainer &itemContainer;
};

class HotbarGui : public GuiElement {
public:
	HotbarGui(Entity player);
	~HotbarGui() override = default;
	void update(GuiManager *manager) override;
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
	void update(GuiManager *manager) override;
	void draw() override;

private:
	Entity player;
	Sprite heartSprite;
	Sprite halfHeartSprite;
};

class InventoryGui : public Widget {
public:
	InventoryGui(pair position, Inventory *inventory, int spacing, Inventory *link = nullptr);
	~InventoryGui() override = default;
	Inventory *link;

private:
	Inventory *inventory;
	int spacing;
};

template <typename T>
class Button : public GuiElement {
public:
	Button(pair position, pair size, void (T::*callback)(), T *object, Sprite sprite)
		: GuiElement(position, size), sprite(sprite), callback(callback), object(object) {}
	~Button() override = default;
	bool handleEvent(InputEvent event) override {
		if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
			(object->*callback)();
		}
	}

	void draw() override {
		if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	}

private:
	Sprite sprite;
	void (T::*callback)();
	T *object;
};

class CraftingGui : public Widget {
public:
	CraftingGui(pair position, Inventory *link = nullptr);
	~CraftingGui() override;
	void craft();

private:
	Inventory *link;
	ItemContainer inputA;
	ItemContainer inputB;
	ItemContainer output;
};
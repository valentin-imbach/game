
#pragma once
#include "ECS_types.hpp"
#include "Events.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "utils.hpp"
#include "GuiManager.hpp"

#define GUI_BOX true

class GuiElement {
public:
	GuiElement(pair position, pair size, Direction::value alignment = Direction::NONE);
	virtual ~GuiElement() = default;
	void reposition(GuiElement* parent = nullptr);
	virtual void update(GuiManager* manager) { guiManager = manager; };
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
	void update(GuiManager* manager) override;
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
	Tab(TabWidget* parent, uint index);
	~Tab() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;

private:
	TabWidget* parent;
	uint index;
};

class TabWidget : public Widget {
public:
	TabWidget(pair position, pair size);
	~TabWidget() override = default;
	void draw() override;
	void update(GuiManager* manager) override;
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
	ItemSlot(pair position, ItemContainer& itemContainer,
		Inventory* link = nullptr);
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
	void update(GuiManager* manager) override;
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
	void update(GuiManager* manager) override;
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

template <typename T>
class Button : public GuiElement {
public:
	Button(pair position, pair size, void (T::*callback)(), T* object, Sprite sprite)
		: GuiElement(position, size), sprite(sprite), callback(callback), object(object) {}
	~Button() override = default;
	bool handleEvent(InputEvent event) override {
		if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
			(object->*callback)();
			return true;
		}
		return false;
	}

	void draw() override {
		if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
	}

private:
	Sprite sprite;
	void (T::*callback)();
	T* object;
};

class CraftingGui : public Widget {
public:
	CraftingGui(pair position, Inventory* link = nullptr);
	~CraftingGui() override;
	void craft();

private:
	Inventory* link;
	ItemContainer inputA;
	ItemContainer inputB;
	ItemContainer output;
};

template <typename T>
class Selector : public GuiElement {
public:
	Selector(pair position, pair size, void (T::*callback)(int n), T* object, int columns = 1)
		: GuiElement(position, size), callback(callback), object(object), columns(columns) {}
	~Selector() = default;

	void addSelection(SpriteStack sprite) {
		sprites.push_back(sprite);
	}

	bool handleEvent(InputEvent event) override {
		if (event.id == InputEventId::PRIMARY && inside(event.mousePosition)) {
			int offset = screenSize.x / columns;
			pair index = (event.mousePosition - screenPosition + screenSize/2) / offset;
			int n = index.x + columns * index.y;
			if (n < sprites.size()) {
				(object->*callback)(n);
				return true;
			}
		}
		return false;
	}

	void draw() override {
		int offset = screenSize.x / columns;
		if (GUI_BOX) TextureManager::drawRect(screenPosition, screenSize);
		for (int i = 0; i < sprites.size(); i++) {
			pair index = pair(i % columns, i / columns);
			pair pos = screenPosition - screenSize/2 + offset * index + pair(offset/2, offset/2);
			sprites[i].draw(pos, GuiManager::scale);
			if (GUI_BOX) TextureManager::drawRect(pos, {offset, offset});
		}
	}

private:
	int columns;
	void (T::*callback)(int n);
	T* object;
	std::vector<SpriteStack> sprites;
};

class BuildGui : public Widget {
public:
	BuildGui(pair position) : Widget(position, {80, 80}, Sprite()) {
		std::unique_ptr<Selector<BuildGui>> selector = std::make_unique<Selector<BuildGui>>(pair(0, 0), pair(48, 48), &BuildGui::select, this, 3);
		SpriteStack sprites;
		sprites.addSprite(Sprite(SpriteSheet::STATIONS, {0,1}));
		selector->addSelection(sprites);
		selector->addSelection(sprites);
		selector->addSelection(sprites);
		selector->addSelection(sprites);
		addGuiElement(std::move(selector));
	}
	~BuildGui() = default;
	void select(int n) {
		LOG("Selected", n);
	}

private:

};
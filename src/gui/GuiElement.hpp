
#pragma once
#include "Crafting.hpp"
#include "ECS_types.hpp"
#include "Events.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "utils.hpp"

class GuiManager;

#define GUI_BOX true

class GuiElement {
public:
	GuiElement(pair position, pair size, Direction::value alignment = Direction::NONE);
	virtual ~GuiElement() = default;
	void reposition(GuiElement* parent = nullptr);
	virtual void update(GuiManager* manager);
	virtual void draw();
	virtual bool handleEvent(InputEvent event);

protected:
	pair position;
	pair screenPosition;
	pair size;
	pair screenSize;

	Sprite sprite;
	Sprite hoverSprite;
	Direction::value alignment;
	GuiManager* guiManager;

	bool hover;
	bool keys;
	bool absorbHover = true;
	bool absorbKeys = false;
	bool inside(pair position);

	friend class GuiManager;
};

class Widget : public GuiElement {
public:
	Widget(pair position, pair size, Sprite sprite = {});
	~Widget() override = default;
	void update(GuiManager* manager) override;
	void draw() override;
	void addGuiElement(std::unique_ptr<GuiElement> guiElement);
	void removeGuiElement();
	bool handleEvent(InputEvent event) override;

protected:
	std::vector<std::unique_ptr<GuiElement>> children;
};

class TabWidget : public Widget {
public:
	TabWidget(pair position, pair size);
	~TabWidget() override = default;
	void draw() override;
	void update(GuiManager* manager) override;
	void addTab(std::unique_ptr<GuiElement> guiElement);
	void selectTab(uint n);
	bool handleEvent(InputEvent event) override;

private:
	uint selected = 0;
	std::vector<std::unique_ptr<GuiElement>> tabs;
	friend class Tab;
};

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

class ItemSlot : public GuiElement {
public:
	ItemSlot(pair position, ItemContainer& itemContainer, Inventory* link = nullptr);
	~ItemSlot() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
	Inventory* link;

private:
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

class Button : public GuiElement {
public:
	Button(pair position, pair size, std::function<void()> callback, Sprite sprite = {}, Sprite hoverSprite = {}, std::string text = "", Direction::value alignment = Direction::NONE);
	~Button() override = default;
	bool handleEvent(InputEvent event) override;
	void draw() override;

private:
	std::string text;
	std::function<void()> callback;
};

class CraftingGrid : public Widget {
public:
	CraftingGrid(pair position, CraftingRecipeId::value recipeId, Inventory* link = nullptr);
	~CraftingGrid() override;
	void craft();

private:
	CraftingRecipeId::value recipeId;
	uint arity;
	Inventory* link;
	std::vector<ItemContainer> inputs;
	ItemContainer output;
};

class CraftingGui : public Widget {
public:
	CraftingGui(pair position, Inventory* link = nullptr);
	~CraftingGui() = default;
	void select(int n);

private:
	CraftingGrid* craftingGrid = nullptr;
	Inventory* link;
};

class Selector : public GuiElement {
public:
	Selector(pair position, pair size, std::function<void(int)> callback, int columns = 1, Direction::value alignment = Direction::NONE);
	~Selector() = default;

	void addSelection(SpriteStack sprite);
	bool handleEvent(InputEvent event) override;
	void draw() override;

private:
	int columns;
	int selected = -1;
	std::function<void(int)> callback;
	std::vector<SpriteStack> sprites;
};

class BuildGrid : public Widget {
public:
	BuildGrid(pair position, BuildRecipeId::value recipeId, Inventory* link = nullptr);
	~BuildGrid() override;
	void build();

private:
	BuildRecipeId::value recipeId;
	uint arity;
	Inventory* link;
	std::vector<ItemContainer> inputs;
};

class BuildGui : public Widget {
public:
	BuildGui(pair position, Inventory* link = nullptr);
	~BuildGui() = default;
	void select(int n);
	void build();

private:
	BuildGrid* buildGrid = nullptr;
	Inventory* link;
};

class TextGui : public GuiElement {
public:
	TextGui(pair position, std::string text, Direction::value alignment = Direction::NONE);
	~TextGui() = default;
	void draw() override;
private:
	std::string text;
};

class TextField : public GuiElement {
public:
    TextField(pair position, pair size, Direction::value alignment = Direction::NONE);
	std::string getText();
private:
	std::string text;
	bool active;
	void draw() override;
    bool handleEvent(InputEvent event) override;
};
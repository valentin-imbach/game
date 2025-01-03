
#pragma once
#include "Crafting.hpp"

#include "Events.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "utils.hpp"

class GuiManager;

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
	bool inside(vec position);

	friend class GuiManager;
};

class Widget : public GuiElement {
public:
	Widget(pair position, pair size, Sprite sprite = {}, Direction::value alignment = Direction::NONE);
	~Widget() override = default;
	void update(GuiManager* manager) override;
	void draw() override;
	void addGuiElement(std::unique_ptr<GuiElement> guiElement);
	void removeGuiElement();
	bool handleEvent(InputEvent event) override;

	template<typename T, typename... Ts>
	T* emplaceGuiElement(Ts... args) {
		std::unique_ptr<T> guiElement = std::make_unique<T>(std::forward<Ts>(args)...);
		T* res = guiElement.get();
		addGuiElement(std::move(guiElement));
		return res;
	}

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

struct InventorySlice {
	InventorySlice(Inventory* inventory = nullptr) : inventory(inventory) {}
	Inventory* inventory = nullptr;
	pair xrange = pair(0, INV_WIDTH);
	pair yrange = pair(0, INV_HEIGHT);
};

class ItemSlot : public GuiElement {
public:
	ItemSlot(pair position, ItemContainer& itemContainer, InventorySlice link = {});
	~ItemSlot() override = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
	InventorySlice link;

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
	InventoryGui(pair position, InventorySlice slice, int spacing, InventorySlice link = {});
	~InventoryGui() override = default;
	InventorySlice link;

private:
	InventorySlice slice;
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

// class CraftingGrid : public Widget {
// public:
// 	CraftingGrid(pair position, CraftingRecipeId::value recipeId, InventorySlice link = {});
// 	~CraftingGrid() override;
// 	void craft();

// private:
// 	CraftingRecipeId::value recipeId;
// 	uint arity;
// 	InventorySlice link;
// 	std::vector<ItemContainer> inputs;
// 	ItemContainer output;
// };

// class CraftingGui : public Widget {
// public:
// 	CraftingGui(pair position, InventorySlice link = {});
// 	~CraftingGui() = default;
// 	void select(int n);

// private:
// 	CraftingGrid* craftingGrid = nullptr;
// 	InventorySlice link;
// };

// class ToolGui : public Widget {
// public:
// 	ToolGui(pair position, InventorySlice link = {});
// 	~ToolGui() = default;

// private:
// 	InventorySlice link;
// };

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
	BuildGrid(pair position, BuildRecipeId::value recipeId, InventorySlice link = {});
	~BuildGrid() override;
	void build();

private:
	BuildRecipeId::value recipeId;
	uint arity;
	InventorySlice link;
	std::vector<ItemContainer> inputs;
};

class BuildGui : public Widget {
public:
	BuildGui(pair position, InventorySlice link = {});
	~BuildGui() = default;
	void select(int n);
	void build();

private:
	BuildGrid* buildGrid = nullptr;
	InventorySlice link;
};

class TextGui : public GuiElement {
public:
	TextGui(pair position, std::string text, bool centered = true, Direction::value alignment = Direction::NONE);
	~TextGui() = default;
	void draw() override;
private:
	std::string text;
};

class TextField : public GuiElement {
public:
    TextField(pair position, pair size, std::string* text, std::string preview = "", Direction::value alignment = Direction::NONE);
	std::string getText();
private:
	std::string* text;
	std::string preview;
	bool active;
	void draw() override;
    bool handleEvent(InputEvent event) override;
};

class ProgressGui : public GuiElement {
public:
	ProgressGui(pair position, float& value, Sprite base, Sprite overlay, bool verticle = false, Direction::value alignment = Direction::NONE);
	~ProgressGui() = default;
	void draw() override;
private:
	bool verticle;
	Sprite overlay;
	float& value;
};

class SliderHeadGui : public GuiElement {
public:
	SliderHeadGui(pair position, pair size, Sprite sprite);
	~SliderHeadGui() = default;
friend class SliderGui;
friend class RangeSliderGui;
};

class SliderGui : public Widget {
public:
	SliderGui(pair position, int* value, int min, int max, Sprite sprite, Sprite slider, Direction::value alignment = Direction::NONE);
	~SliderGui() = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
private:
	SliderHeadGui* slider;
	int* value;
	int min;
	int max;
	bool active = false;
};

class RangeSliderGui : public Widget {
public:
	RangeSliderGui(pair position, pair* value, pair range, Sprite sprite, Direction::value alignment = Direction::NONE);
	~RangeSliderGui() = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
private:
	SliderHeadGui* leftHead;
	SliderHeadGui* rightHead;
	pair* value;
	pair range;
	bool leftActive = false;
	bool rightActive = false;
};

class ValueGui : public GuiElement {
public:
	ValueGui(pair position, int* value, Direction::value alignment = Direction::NONE);
	~ValueGui() = default;
	void draw() override;
private:
	int* value;
};

class SettingRangeSlider : public Widget {
public:
	SettingRangeSlider(pair position, pair* value, pair range, std::string text, Direction::value alignment = Direction::NONE);
	~SettingRangeSlider() = default;
};

class CheckboxGui : public GuiElement {
public:
	CheckboxGui(pair position, bool* value, Direction::value alignment = Direction::NONE);
	~CheckboxGui() = default;
	void draw() override;
	bool handleEvent(InputEvent event) override;
private:
	bool* value;
	Sprite checkedSprite;
};

class SettingCheckbox : public Widget {
public:
	SettingCheckbox(pair position, bool* value, std::string text, Direction::value alignment = Direction::NONE);
	~SettingCheckbox() = default;
};
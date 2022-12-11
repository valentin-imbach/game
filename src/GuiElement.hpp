
#pragma once
#include "utils/direction.hpp"
#include "utils/utils.hpp"
#include "Sprite.hpp"

#define GUI_SCALE 3

class GuiElement {
public:
	GuiElement(pair position, Direction alignment = Direction::NONE);
	virtual ~GuiElement();
	void reposition(GuiElement* parent = nullptr);
	virtual void update() = 0;
	virtual void draw() = 0;

protected:
	pair size;
	pair screenSize;
	pair position;
	pair screenPosition;
	Direction alignment;

	friend class Widget;
	friend class GuiManager;
};

class Widget : public GuiElement {
public:
	Widget(pair position, Sprite sprite);
	~Widget();
	void update() override;
	void draw() override;
	void addGuiElement(GuiElement guiElement);

private:
	Sprite sprite;
	std::vector<GuiElement> children;
};
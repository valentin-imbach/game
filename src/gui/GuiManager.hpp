
#pragma once
#include <memory>
#include <vector>
#include "Events.hpp"
#include "Item.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "ECS.hpp"

class World;
class GuiElement;

class GuiManager {
public:
	void update();
	void draw();

	bool handleEvent(InputEvent event);
	void open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b = nullptr);
	void close();
	bool active();
	void add(std::unique_ptr<GuiElement> guiElement);

	World* world;
	pair mousePosition;
	ItemContainer mouseItemContainer;

	static uint scale;

private:
	std::unique_ptr<GuiElement> primary;
	std::unique_ptr<GuiElement> secondary;
	std::vector<std::unique_ptr<GuiElement>> guiElements;
};
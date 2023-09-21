
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

struct GuiBanner {
	std::string text;
	uint timeLeft;
};

class GuiManager {
public:
	void update(uint dt);
	void draw();

	bool handleEvent(InputEvent event);
	void open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b = nullptr);
	void close();
	bool active();
	void add(std::unique_ptr<GuiElement> guiElement);

	void addBanner(std::string text);

	World* world;
	pair mousePosition;
	ItemContainer mouseItemContainer;

	Entity buildMode = 0;

	static bool box;
	static uint scale;

private:
	std::unique_ptr<GuiElement> primary;
	std::unique_ptr<GuiElement> secondary;
	std::vector<std::unique_ptr<GuiElement>> guiElements;
	std::queue<GuiBanner> banners;
};
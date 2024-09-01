
#include "Menu.hpp"
#include "Window.hpp"
#include "Game.hpp"

void Menu::draw(uint ticks) {
	widget->draw();
};

bool Menu::handleEvent(InputEvent event) {
	return widget->handleEvent(event);
}

void Menu::update() {
	widget->reposition();
	widget->update(nullptr);
}

MainMenu::MainMenu() {}

void MainMenu::build(Game* game) {
	std::vector<std::string> worldNames;
	auto path = Window::instance->root / "saves";
	for (const auto& dir : std::filesystem::directory_iterator(path)) {
		if (dir.is_directory()) worldNames.push_back(dir.path().filename().string());
	}

	auto createCallback = [game]() -> void {
		game->gameState = GameState::CREATE;
	};

	auto settingsCallback = [game]() -> void {
		game->gameState = GameState::SETTINGS;
	};

	parallax[0] = TextureManager::loadTexture("background/1.png");
	parallax[1] = TextureManager::loadTexture("background/2.png");
	parallax[2] = TextureManager::loadTexture("background/3.png");
	parallax[3] = TextureManager::loadTexture("background/4.png");
	widget = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	widget->addGuiElement(std::make_unique<Button>(pair(-30, -70), pair(50, 20), createCallback, Sprite(), Sprite(), "New World"));
	widget->addGuiElement(std::make_unique<Button>(pair(30, -70), pair(50, 20), settingsCallback, Sprite(), Sprite(), "Settings"));

	int offset = -40;
	for (auto& name : worldNames) {
		widget->addGuiElement(std::make_unique<Button>(pair(0, offset), pair(50, 20), std::bind(&Game::loadWorld, game, name), Sprite(), Sprite(), name));
		widget->addGuiElement(std::make_unique<Button>(pair(50, offset), pair(20, 20), std::bind(&Game::removeWorld, game, name), Sprite(), Sprite(), "del"));
		offset += 25;
	}
}

void MainMenu::draw(uint ticks) {
	TextureStyle style;
	style.centered = false;
	TextureManager::drawTexture(parallax[0], nullptr, {0, 0}, {576, 324}, {0, 0}, Window::instance->size, style);
	TextureManager::drawTexture(parallax[1], nullptr, {int(ticks / 140) % 576, 0}, {576, 324}, {0, 0}, Window::instance->size, style);
	TextureManager::drawTexture(parallax[2], nullptr, {int(ticks / 60) % 576, 0}, {576, 324}, {0, 0}, Window::instance->size, style);
	TextureManager::drawTexture(parallax[3], nullptr, {int(ticks/ 20) % 576, 0}, {576, 324}, {0, 0}, Window::instance->size, style);
	Menu::draw(ticks);
}

void PauseMenu::build(Game* game) {
	widget = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	widget->addGuiElement(std::make_unique<Button>(pair(0, 0), pair(50, 20), std::bind(&Game::saveWorld, game), Sprite(), Sprite(), "Save and Quit"));
}

void CreateMenu::build(Game* game) {

	auto createCallback = [this, game]() -> void {
		uint seed = arc4random();
		if (!seedInput.empty() && string::is_int(seedInput)) seed = std::stoi(seedInput);
		game->createWorld(nameInput, seed, params);
	};

	auto cancelCallback = [game]() -> void {
		game->gameState = GameState::MENU;
	};

	widget = std::make_unique<Widget>(pair(0, 0), pair(240, 160), Sprite());
	widget->emplaceGuiElement<TextField>(pair(-60, -65), pair(80, 15), &nameInput, "World Name");
	widget->emplaceGuiElement<TextField>(pair(60, -65), pair(80, 15), &seedInput, "World Seed");

	widget->emplaceGuiElement<SettingRangeSlider>(pair(0, -40), &params.temperature, pair(-20, 40), "Temerature");
	widget->emplaceGuiElement<SettingRangeSlider>(pair(0, -25), &params.elevation, pair(-500, 1500), "Elevation");
	widget->emplaceGuiElement<SettingRangeSlider>(pair(0, -10), &params.percipitation, pair(0, 300), "Perculation");

	widget->emplaceGuiElement<SettingCheckbox>(pair(0, 10), &params.cliffs, "Cliffs");
	widget->emplaceGuiElement<SettingCheckbox>(pair(0, 30), &params.rivers, "Rivers");
	widget->emplaceGuiElement<SettingCheckbox>(pair(0, 50), &params.structures, "Structures");

	widget->emplaceGuiElement<SettingRangeSlider>(pair(0, 70), &params.variation, pair(0, 100), "Variation");
	widget->emplaceGuiElement<SettingRangeSlider>(pair(0, 85), &params.vegetation, pair(0, 100), "Vegetation");

	widget->emplaceGuiElement<Button>(pair(35, -15), pair(60, 20), cancelCallback, Sprite(), Sprite(), "Main Menu", Direction::SOUTH_WEST);
	widget->emplaceGuiElement<Button>(pair(-35, -15), pair(60, 20), createCallback, Sprite(), Sprite(), "Create World", Direction::SOUTH_EAST);

	
}
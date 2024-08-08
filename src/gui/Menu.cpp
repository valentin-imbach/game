
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
		if (!nameField || !seedField) return;
		uint seed = arc4random();
		std::string seedString = seedField->getText();
		if (!seedString.empty() && string::is_int(seedString)) {
			seed = std::stoi(seedString);
		}
		game->createWorld(nameField->getText(), seed);
	};

	auto cancelCallback = [game]() -> void {
		game->gameState = GameState::MENU;
	};

	widget = std::make_unique<Widget>(pair(0, 0), pair(240, 160), Sprite());

	seedField = widget->emplaceGuiElement<TextField>(pair(60, -65), pair(80, 15), "World Seed");
	nameField = widget->emplaceGuiElement<TextField>(pair(-60, -65), pair(80, 15), "World Name");

	// widget->addGuiElement(std::make_unique<SettingSlider>(pair(-60, -30), s1, 0, 100, "Slider 1"));
	widget->emplaceGuiElement<SettingSlider>(pair(-60, -30), &s1, 0, 100, "Slider 1");
	widget->emplaceGuiElement<SettingSlider>(pair(60, -30), &s2, 0, 100, "Slider 2");
	widget->emplaceGuiElement<SettingSlider>(pair(-60, 20), &s3, 0, 100, "Slider 3");
	widget->emplaceGuiElement<SettingSlider>(pair(60, 20), &s4, 0, 100, "Slider 4");

	widget->emplaceGuiElement<Button>(pair(35, -15), pair(60, 20), cancelCallback, Sprite(), Sprite(), "Main Menu", Direction::SOUTH_WEST);
	widget->emplaceGuiElement<Button>(pair(-35, -15), pair(60, 20), createCallback, Sprite(), Sprite(), "Create World", Direction::SOUTH_EAST);
}
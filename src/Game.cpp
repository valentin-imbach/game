
#include "Game.hpp"
#include "Events.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_scancode.h"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include <filesystem>

#define DEBUG_MODE true

Game::Game() : trackMix(1), console(this) {
	framesPerSecond = 0;
	Sprite::loadSpriteSheets();
	TextManager::Init();
	SoundManager::Init();

	gameState = GameState::MENU;
	buildMenu();

	if (DEBUG_MODE) {
		createWorld("Test World", arc4random(), true);
		return;
	}
}

Game::~Game() {
	TextManager::cleanup();
	SoundManager::cleanup();
}

void Game::buildMenu() {
	std::vector<std::string> worldNames;
	auto path = Window::instance->root / "saves";
	for (const auto& dir : std::filesystem::directory_iterator(path)) {
		if (dir.is_directory()) worldNames.push_back(dir.path().filename().string());
	}

	mainMenu = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	mainMenu->addGuiElement(std::make_unique<Button>(pair(0, -70), pair(50, 20), std::bind(&Game::createButton, this), Sprite(), Sprite(), "New World"));
	std::unique_ptr<TextField> nameTextField = std::make_unique<TextField>(pair(-50, -90), pair(80, 15));
	std::unique_ptr<TextField> seedTextField = std::make_unique<TextField>(pair(50, -90), pair(80, 15));
	nameField = nameTextField.get();
	seedField = seedTextField.get();
	mainMenu->addGuiElement(std::move(nameTextField));
	mainMenu->addGuiElement(std::move(seedTextField));

	int offset = -40;
	for (auto& name : worldNames) {
		mainMenu->addGuiElement(std::make_unique<Button>(pair(0, offset), pair(50, 20), std::bind(&Game::loadWorld, this, name), Sprite(), Sprite(), name));
		mainMenu->addGuiElement(std::make_unique<Button>(pair(50, offset), pair(20, 20), std::bind(&Game::removeWorld, this, name), Sprite(), Sprite(), "del"));
		offset += 25;
	}

	pauseMenu = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	pauseMenu->addGuiElement(std::make_unique<Button>(pair(0, 0), pair(50, 20), std::bind(&Game::saveWorld, this), Sprite(), Sprite(), "Save and Quit"));
}

void Game::createButton() {
	if (!nameField || !seedField) return;
	uint seed;
	std::string seedString = seedField->getText();
	if (!seedString.empty() && string::is_int(seedString)) {
		seed = std::stoi(seedString);
	} else {
		seed = arc4random();
	}
	createWorld(nameField->getText(), seed);
}

void Game::createWorld(std::string name, uint seed, bool debug) {
	if (name.empty()) {
		WARNING("World name can't be empty");
		return;
	}
	auto path = Window::instance->root / "saves" / name;
	if (std::filesystem::exists(path)) {
		WARNING("Trying to create duplicate world", name);
		return;
	}

	world = std::make_unique<World>(name, seed, debug);
	LOG("World", name, "created");
	gameState = GameState::RUNNING;
}

void Game::removeWorld(std::string name) {
	auto path = Window::instance->root / "saves" / name;
	if (!std::filesystem::exists(path)) {
		WARNING("Trying to remove non-existent world", name);
		return;
	}
	std::filesystem::remove_all(path);
	buildMenu();
}

void Game::loadWorld(std::string name) {
	auto path = Window::instance->root / "saves" / name / "save.binary";
	std::fstream file = std::fstream(path, std::ios::in | std::ios::binary);
	if (!file) ERROR("No save file found for world", name);
	world = std::make_unique<World>(file);
	file.close();
	world->name = name;
	gameState = GameState::RUNNING;
}

void Game::saveWorld() {
	if (!world) return;
	auto path = Window::instance->root / "saves" / world->name;
	std::filesystem::create_directory(path);
	std::fstream file = std::fstream(path / "save.binary", std::ios::out | std::ios::binary);
	if (!file) ERROR("Failed to create save file for world", world->name);
	world->serialise(file);
	file.close();
	buildMenu();
	world = nullptr;
	gameState = GameState::MENU;
}

void Game::update(uint dt) {
	Window::instance->update();
	//trackMix.update();
	if (gameState == GameState::MENU) {
		mainMenu->reposition();
		mainMenu->update(nullptr);
	} else if (gameState == GameState::PAUSED) {
		pauseMenu->reposition();
		pauseMenu->update(nullptr);
		debugScreen.update(world.get(), framesPerSecond, updatesPerSecond);
	} else if (gameState == GameState::RUNNING) {
		world->update(dt);
		debugScreen.update(world.get(), framesPerSecond, updatesPerSecond);
	}
}

void Game::draw() {
	Window::instance->clear();
	if (gameState == GameState::MENU) {
		mainMenu->draw();
	} else if (gameState == GameState::RUNNING) {
		world->draw();
		debugScreen.draw();
	} else if (gameState == GameState::PAUSED) {
		world->draw();
		debugScreen.draw();
		pauseMenu->draw();
	}
	console.draw();
	Window::instance->draw();
}

bool Game::handleEvent(InputEvent event, uint dt) {
	if (event.id == InputEventId::QUIT) {
		LOG("Window closed");
		gameState = GameState::NONE;
		return true;
	}

	if (console.handleEvent(event)) return true;

	if (gameState == GameState::MENU) {
		return mainMenu->handleEvent(event);
	} else if (gameState == GameState::PAUSED) {
		if (pauseMenu->handleEvent(event)) return true;
		if (event.id == InputEventId::ESCAPE) {
			gameState = GameState::RUNNING;
			return true;
		}
	} else if (gameState == GameState::RUNNING) {
		if (debugScreen.handleEvent(event)) return true;
		if (event.id == InputEventId::ESCAPE) {
			gameState = GameState::PAUSED;
			return true;
		}
		return world->handleEvent(event, dt);
	}
	return false;
}

void Game::pollEvents(uint dt) {
	pair mousePosition;
	const uchar* keyState = SDL_GetKeyboardState(NULL);
	const uint mouseState = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

	if (world) {
		world->guiManager.mousePosition = mousePosition;

		world->inputState.set(InputStateId::MOVE_EAST, keyState[SDL_SCANCODE_D]);
		world->inputState.set(InputStateId::MOVE_NORTH, keyState[SDL_SCANCODE_W]);
		world->inputState.set(InputStateId::MOVE_WEST, keyState[SDL_SCANCODE_A]);
		world->inputState.set(InputStateId::MOVE_SOUTH, keyState[SDL_SCANCODE_S]);

		world->inputState.set(InputStateId::INFO, keyState[SDL_SCANCODE_TAB]);
		world->inputState.set(InputStateId::ALTER, keyState[SDL_SCANCODE_LSHIFT]);

		world->inputState.set(InputStateId::PRIMARY, mouseState & SDL_BUTTON_LMASK);
		world->inputState.set(InputStateId::SECONDARY, mouseState & SDL_BUTTON_RMASK);
	}

	handleEvent({InputEventId::RESET, mousePosition}, dt);
	handleEvent({InputEventId::STATE, mousePosition}, dt);
	handleEvent({InputEventId::HOVER, mousePosition}, dt);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		InputEvent inputEvent;
		inputEvent.mousePosition = mousePosition;
		if (event.type == SDL_QUIT) {
			inputEvent.id = InputEventId::QUIT;
		} else if (event.type == SDL_TEXTINPUT) {
			inputEvent.id = InputEventId::TEXT;
			inputEvent.text = event.text.text;
		} else if (event.type == SDL_MOUSEWHEEL) {
			if (event.wheel.y > 0) {
				inputEvent.id = InputEventId::ROTATE_LEFT;
			} else if (event.wheel.y < 0) {
				inputEvent.id = InputEventId::ROTATE_RIGHT;
			} else {
				continue;
			}
		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.repeat) continue;
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				inputEvent.id = InputEventId::ESCAPE;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
				inputEvent.id = InputEventId::INVENTORY;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
				inputEvent.id = InputEventId::THROW;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_1) {
				inputEvent.id = InputEventId::SELECT_1;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
				inputEvent.id = InputEventId::SELECT_2;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
				inputEvent.id = InputEventId::SELECT_3;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_4) {
				inputEvent.id = InputEventId::SELECT_4;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_5) {
				inputEvent.id = InputEventId::SELECT_5;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_6) {
				inputEvent.id = InputEventId::SELECT_6;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_7) {
				inputEvent.id = InputEventId::SELECT_7;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_TAB) {
				inputEvent.id = InputEventId::CONSOLE;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				inputEvent.id = InputEventId::RETURN;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
				inputEvent.id = InputEventId::BACKSPACE;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_F1) {
				inputEvent.id = InputEventId::DEBUG;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				inputEvent.id = InputEventId::UP;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				inputEvent.id = InputEventId::DOWN;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_M) {
				inputEvent.id = InputEventId::MAP;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_COMMA) {
				inputEvent.id = InputEventId::ZOOM_OUT;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_PERIOD) {
				inputEvent.id = InputEventId::ZOOM_IN;
			} else {
				continue;
			}
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				inputEvent.id = InputEventId::PRIMARY;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				inputEvent.id = InputEventId::SECONDARY;
			} else {
				continue;
			}
		} else {
			continue;
		}
		handleEvent(inputEvent, dt);
	}
}
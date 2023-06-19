
#include "Game.hpp"
#include "Events.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_scancode.h"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include <filesystem>

Game::Game()
	: trackMix(1), console(this) {
	lastFrameTicks = SDL_GetTicks();
	sample = std::queue<uint>();
	sampleSum = 0;

	framesPerSecond = 0;
	Sprite::loadSpriteSheets();
	TextManager::Init();
	SoundManager::Init();

	gameState = GameState::MENU;
	buildMenu();
}

void Game::buildMenu() {
	std::vector<std::string> worldNames;
	auto path = Window::instance->root / "saves";
	for (const auto& dir : std::filesystem::directory_iterator(path)) {
		if (dir.is_directory()) worldNames.push_back(dir.path().filename().string());
	}

	mainMenu = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	mainMenu->addGuiElement(std::make_unique<Button>(pair(0, -70), pair(50, 20), std::bind(&Game::create, this), Sprite(), "New World"));

	int offset = -40;
	for (auto& name : worldNames) {
		mainMenu->addGuiElement(std::make_unique<Button>(pair(0, offset), pair(50, 20), std::bind(&Game::load, this, name), Sprite(), name));
		mainMenu->addGuiElement(std::make_unique<Button>(pair(50, offset), pair(20, 20), std::bind(&Game::remove, this, name), Sprite(), "del"));
		offset += 25;
	}

	pauseMenu = std::make_unique<Widget>(pair(0, 0), pair(160, 160), Sprite(SpriteSheet::MENU, {0, 0}, {10, 10}));
	pauseMenu->addGuiElement(std::make_unique<Button>(pair(0, 0), pair(50, 20), std::bind(&Game::save, this), Sprite(), "Save and Quit"));
}

void Game::create() {
	std::string name = "world" + std::to_string(number);
	auto path = Window::instance->root / "saves";
	for (const auto& dir : std::filesystem::directory_iterator(path)) {
		if (dir.path().filename().string() == name) return;
	}
	number += 1;

	uint seed = arc4random();
	world = std::make_unique<World>(name, seed);
	LOG("World", name, "created");
	gameState = GameState::RUNNING;
}

void Game::remove(std::string name) {
	auto path = Window::instance->root / "saves" / name;
	std::filesystem::remove_all(path);
	buildMenu();
}

void Game::load(std::string name) {
	auto path = Window::instance->root / "saves" / name / "save.binary";
	std::fstream file = std::fstream(path, std::ios::in | std::ios::binary);
	if (!file) ERROR("No save for world", name);
	world = std::make_unique<World>(file);
	file.close();
	world->name = name;
	gameState = GameState::RUNNING;
}

void Game::save() {
	if (!world) return;
	auto path = Window::instance->root / "saves" / world->name;
	std::filesystem::create_directory(path);
	std::fstream file = std::fstream(path / "save.binary", std::ios::out | std::ios::binary);
	if (!file) ERROR("No save for world");
	world->serialise(file);
	file.close();
	buildMenu();
	world = nullptr;
	gameState = GameState::MENU;
}

void Game::update() {
	Window::instance->update();
	//trackMix.update();
	if (gameState == GameState::MENU) {
		mainMenu->reposition();
		mainMenu->update(nullptr);
	} else if (gameState == GameState::PAUSED) {
		pauseMenu->reposition();
		pauseMenu->update(nullptr);
		debugScreen.update(world.get(), framesPerSecond);
	} else if (gameState == GameState::RUNNING) {
		world->update(dt);
		debugScreen.update(world.get(), framesPerSecond);
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

void Game::handleEvents() {
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

	InputEvent stateEvent;
	stateEvent.mousePosition = mousePosition;
	stateEvent.id = InputEventId::STATE;
	if (gameState == GameState::RUNNING) world->handleEvent(stateEvent, dt);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			LOG("Window closed");
			gameState = GameState::NONE;
			continue;
		}

		if (console.handleEvent(event)) continue;
		if (event.key.repeat) continue;

		InputEvent inputEvent;
		inputEvent.mousePosition = mousePosition;
		if (event.type == SDL_KEYDOWN) {
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

		if (gameState == GameState::MENU) {
			mainMenu->handleEvent(inputEvent);
		} else if (gameState == GameState::PAUSED) {
			pauseMenu->handleEvent(inputEvent);
			if (inputEvent.id == InputEventId::ESCAPE) gameState = GameState::RUNNING;
		} else if (gameState == GameState::RUNNING) {
			if (debugScreen.handleEvent(event)) continue;
			if (inputEvent.id == InputEventId::ESCAPE) gameState = GameState::PAUSED;
			world->handleEvent(inputEvent, dt);
		}
	}

	if (world && console.active) world->inputState = 0;
}

void Game::limitFrameRate(int fps) {
	uint delay = std::ceil(1000.0f / fps);
	uint ticks = SDL_GetTicks();
	uint past = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	if (past < delay) {
		SDL_Delay(delay - past);
		past = delay;
	}

	dt = past;

	sample.push(past);
	sampleSum += past;
	if (sample.size() > SAMPLE_SIZE) {
		sampleSum -= sample.front();
		sample.pop();
		framesPerSecond = std::floor(SAMPLE_SIZE * 1000.0f / sampleSum);
	}
}

#include "Game.hpp"
#include "Events.hpp"
#include <SDL3/SDL.h>
// #include "SDL2/SDL_scancode.h"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include <filesystem>
#include "Dungeon3.hpp"
#include "SoundManager.hpp"
#include "FileWatcher.hpp"

#define DEBUG_MODE false

GameSettings Game::settings = {};

Game::Game() : console(this) {
	framesPerSecond = 0;
	Sprite::loadSpriteSheets();
	TextManager::Init();
	SoundManager::Init();

	gameState = GameState::MENU;
	buildMenus();

	// if (DEBUG_MODE) {
	// 	createWorld("Test World", arc4random(), true);
	// 	return;
	// }

}

void Game::cleanup() {
	TextManager::cleanup();
	SoundManager::cleanup();
}

void Game::buildMenus() {
	mainMenu.build(this);
	pauseMenu.build(this);
	createMenu.build(this);
}

void Game::createWorld(std::string name, uint seed, WorldParameters params) {
	if (name.empty()) {
		WARNING("World name can't be empty");
		return;
	}
	auto path = Window::instance->root / "saves" / name;
	if (std::filesystem::exists(path)) {
		WARNING("Trying to create duplicate world", name);
		return;
	}

	world = std::make_unique<World>(name, seed, params);
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
	buildMenus();
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
	buildMenus();
	world = nullptr;
	gameState = GameState::MENU;
}

void Game::update(uint dt) {
	ticks += dt;
	Window::instance->update();
	FileWatcher::update(ticks);
	console.update(ticks);
	//trackMix.update();
	if (gameState == GameState::MENU) {
		mainMenu.update();
	} else if (gameState == GameState::CREATE) {
		createMenu.update();
	} else if (gameState == GameState::PAUSED) {
		pauseMenu.update();
		debugScreen.update(world.get(), framesPerSecond, updatesPerSecond);
		debugScreen.entity = world->inspect;
	} else if (gameState == GameState::RUNNING) {
		world->update(dt);
		debugScreen.update(world.get(), framesPerSecond, updatesPerSecond);
		debugScreen.entity = world->inspect;
	}
}

void Game::draw() {
	Window::instance->clear();
	if (gameState == GameState::MENU) {
		mainMenu.draw(ticks);
	} else if (gameState == GameState::CREATE) {
		createMenu.draw(ticks);
	} else if (gameState == GameState::RUNNING) {
		world->draw();
		debugScreen.draw();
	} else if (gameState == GameState::PAUSED) {
		world->draw();
		debugScreen.draw();
		pauseMenu.draw(ticks);
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
		return mainMenu.handleEvent(event);
	} else if (gameState == GameState::CREATE) {
		return createMenu.handleEvent(event);
	} else if (gameState == GameState::PAUSED) {
		if (pauseMenu.handleEvent(event)) return true;
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
	vec mousePosition = Window::instance->mousePosition;
	const bool* keyState = Window::instance->keyState;

	InputState inputState;

	inputState.set(InputStateId::MOVE_EAST, keyState[SDL_SCANCODE_D]);
	inputState.set(InputStateId::MOVE_NORTH, keyState[SDL_SCANCODE_W]);
	inputState.set(InputStateId::MOVE_WEST, keyState[SDL_SCANCODE_A]);
	inputState.set(InputStateId::MOVE_SOUTH, keyState[SDL_SCANCODE_S]);

	inputState.set(InputStateId::INFO, keyState[SDL_SCANCODE_TAB]);
	inputState.set(InputStateId::ALTER, keyState[SDL_SCANCODE_LSHIFT]);

	inputState.set(InputStateId::PRIMARY, Window::instance->mouseState & SDL_BUTTON_LMASK);
	inputState.set(InputStateId::SECONDARY, Window::instance->mouseState & SDL_BUTTON_RMASK);

	if (world) {
		world->guiManager.mousePosition = mousePosition;
		world->inputState = inputState;
	}

	handleEvent({InputEventId::RESET, mousePosition, inputState}, dt);
	handleEvent({InputEventId::STATE, mousePosition, inputState}, dt);
	handleEvent({InputEventId::HOVER, mousePosition, inputState}, dt);

	SDL_Event event;
	while (SDL_PollEvent(&event) && gameState) {
		InputEvent inputEvent = {InputEventId::NONE, mousePosition, inputState};
		if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
			inputEvent.id = InputEventId::QUIT;
		} else if (event.type == SDL_EVENT_TEXT_INPUT) {
			inputEvent.id = InputEventId::TEXT;
			inputEvent.text = event.text.text;
		} else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
			if (event.wheel.y > 0) {
				inputEvent.id = InputEventId::ROTATE_LEFT;
			} else if (event.wheel.y < 0) {
				inputEvent.id = InputEventId::ROTATE_RIGHT;
			} else {
				continue;
			}
		} else if (event.type == SDL_EVENT_KEY_DOWN) {

			if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
				inputEvent.id = InputEventId::BACKSPACE;
			} else if (event.key.repeat) continue;

			if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
				inputEvent.id = InputEventId::ESCAPE;
			} else if (event.key.scancode == SDL_SCANCODE_E) {
				inputEvent.id = InputEventId::INVENTORY;
			} else if (event.key.scancode == SDL_SCANCODE_Q) {
				inputEvent.id = InputEventId::THROW;
			} else if (event.key.scancode == SDL_SCANCODE_1) {
				inputEvent.id = InputEventId::SELECT_1;
			} else if (event.key.scancode == SDL_SCANCODE_2) {
				inputEvent.id = InputEventId::SELECT_2;
			} else if (event.key.scancode == SDL_SCANCODE_3) {
				inputEvent.id = InputEventId::SELECT_3;
			} else if (event.key.scancode == SDL_SCANCODE_4) {
				inputEvent.id = InputEventId::SELECT_4;
			} else if (event.key.scancode == SDL_SCANCODE_5) {
				inputEvent.id = InputEventId::SELECT_5;
			} else if (event.key.scancode == SDL_SCANCODE_6) {
				inputEvent.id = InputEventId::SELECT_6;
			} else if (event.key.scancode == SDL_SCANCODE_7) {
				inputEvent.id = InputEventId::SELECT_7;
			} else if (event.key.scancode == SDL_SCANCODE_TAB) {
				inputEvent.id = InputEventId::CONSOLE;
			} else if (event.key.scancode == SDL_SCANCODE_RETURN) {
				inputEvent.id = InputEventId::RETURN;
			} else if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
				inputEvent.id = InputEventId::BACKSPACE;
			} else if (event.key.scancode == SDL_SCANCODE_F1) {
				inputEvent.id = InputEventId::DEBUG;
			} else if (event.key.scancode == SDL_SCANCODE_I) {
				inputEvent.id = InputEventId::INSPECT;
			} else if (event.key.scancode == SDL_SCANCODE_UP) {
				inputEvent.id = InputEventId::UP;
			} else if (event.key.scancode == SDL_SCANCODE_DOWN) {
				inputEvent.id = InputEventId::DOWN;
			} else if (event.key.scancode == SDL_SCANCODE_M) {
				inputEvent.id = InputEventId::MAP;
			} else if (event.key.scancode == SDL_SCANCODE_COMMA) {
				inputEvent.id = InputEventId::ZOOM_OUT;
			} else if (event.key.scancode == SDL_SCANCODE_PERIOD) {
				inputEvent.id = InputEventId::ZOOM_IN;
			} else {
				continue;
			}
		} else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
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
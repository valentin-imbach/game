
#include "Game.hpp"
#include "Events.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_scancode.h"
#include "utils.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

Game::Game() : console(this) {
	gameState = GameState::LOADING;

	lastFrameTicks = SDL_GetTicks();
	sample = std::queue<uint>();
	sampleSum = 0;

	framesPerSecond = 0;
	Sprite::loadSpriteSheets();
	TextManager::Init();
	world = std::make_unique<World>("World");
	gameState = GameState::RUNNING;
}

void Game::update() {
	Window::instance->update();
	debugScreen.update(world.get(), framesPerSecond);
	world->update(dt);	
}

void Game::draw() {
	Window::instance->clear();
	world->draw();
	debugScreen.draw();
	console.draw();
	Window::instance->draw();
}

void Game::handleEvents() {
	pair mousePosition;
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	const Uint32 mouseState = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	world->guiManager.mousePosition = mousePosition;

	world->inputState.set(InputStateId::MOVE_EAST, keyState[SDL_SCANCODE_D]);
	world->inputState.set(InputStateId::MOVE_NORTH, keyState[SDL_SCANCODE_W]);
	world->inputState.set(InputStateId::MOVE_WEST, keyState[SDL_SCANCODE_A]);
	world->inputState.set(InputStateId::MOVE_SOUTH, keyState[SDL_SCANCODE_S]);

	world->inputState.set(InputStateId::INFO, keyState[SDL_SCANCODE_TAB]);
	world->inputState.set(InputStateId::ALTER, keyState[SDL_SCANCODE_LSHIFT]);

	world->inputState.set(InputStateId::PRIMARY, mouseState & SDL_BUTTON_LMASK);
	world->inputState.set(InputStateId::SECONDARY, mouseState & SDL_BUTTON_RMASK);

	SDL_Event event;
	world->inputEvents.clear();
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			LOG("Window closed");
			gameState = GameState::NONE;
			continue;
		}
		if (console.handleEvent(event)) continue;
		if (event.key.repeat) continue;
		if (debugScreen.handleEvent(event)) continue;
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				world->inputEvents.push_back({InputEventId::PRIMARY, mousePosition});
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				world->inputEvents.push_back({InputEventId::SECONDARY, mousePosition});
			}
		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				world->inputEvents.push_back({InputEventId::ESCAPE, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
				world->inputEvents.push_back({InputEventId::INVENTORY, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
				world->inputEvents.push_back({InputEventId::THROW, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_1) {
				world->inputEvents.push_back({InputEventId::SELECT_1, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
				world->inputEvents.push_back({InputEventId::SELECT_2, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
				world->inputEvents.push_back({InputEventId::SELECT_3, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_4) {
				world->inputEvents.push_back({InputEventId::SELECT_4, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_5) {
				world->inputEvents.push_back({InputEventId::SELECT_5, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_6) {
				world->inputEvents.push_back({InputEventId::SELECT_6, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_7) {
				world->inputEvents.push_back({InputEventId::SELECT_7, mousePosition});
			} else if (event.key.keysym.scancode == SDL_SCANCODE_TAB) {
				world->inputEvents.push_back({InputEventId::CONSOLE, mousePosition});
			}
		}
	}

	if (console.active) world->inputState = 0;
}

void Game::limitFrameRate(int fps) {
	uint delay = ceil(1000.0f / fps);
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
		framesPerSecond = floor(SAMPLE_SIZE * 1000.0f / sampleSum);
	}
}

#include "Game.hpp"
#include "Events.hpp"
#include "SDL2/SDL.h"
#include "utils/utils.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

Game::Game() {
	running = true;

	lastFrameTicks = SDL_GetTicks();
	sample = std::queue<uint>();
	sampleSum = 0;

	framesPerSecond = 0;
	Sprite::loadSpriteSheets();
	TextManager::Init();
	world = std::make_unique<World>("World");
}

void Game::update() {
	Window::instance->clear();
	Window::instance->update();
	world->update(dt);
	std::string text = "FPS: " + std::to_string(framesPerSecond);
	TextManager::drawText(text, {20, 20});
	Window::instance->draw();
}

void Game::handleEvents() {
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	const Uint32 mouseState = SDL_GetMouseState(NULL, NULL);

	world -> inputStates.set(size_t(InputState::MOVE_EAST), keyState[SDL_SCANCODE_D]);
	world -> inputStates.set(size_t(InputState::MOVE_NORTH), keyState[SDL_SCANCODE_W]);
	world -> inputStates.set(size_t(InputState::MOVE_WEST), keyState[SDL_SCANCODE_A]);
	world -> inputStates.set(size_t(InputState::MOVE_SOUTH), keyState[SDL_SCANCODE_S]);

	world -> inputStates.set(size_t(InputState::INFO), keyState[SDL_SCANCODE_TAB]);
	world -> inputStates.set(size_t(InputState::ALTER), keyState[SDL_SCANCODE_LSHIFT]);

	world -> inputStates.set(size_t(InputState::PRIMARY), mouseState & SDL_BUTTON_LMASK);
	world -> inputStates.set(size_t(InputState::SECONDARY), mouseState & SDL_BUTTON_RMASK);

	SDL_Event event;
	world -> inputEvents.clear();
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			LOG("Window closed");
			running = false;
			break;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				world -> inputEvents.push_back(InputEvent::PRIMARY);
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				world -> inputEvents.push_back(InputEvent::SECONDARY);
			}
		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				world -> inputEvents.push_back(InputEvent::ESCAPE);
			} else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
				world -> inputEvents.push_back(InputEvent::INVENTORY);
			} else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
				world -> inputEvents.push_back(InputEvent::THROW);
			}
		}
		
	}
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
	
	sample.push(dt);
	sampleSum += dt;
	if (sample.size() > SAMPLE_SIZE) {
		sampleSum -= sample.front();
		sample.pop();
		framesPerSecond = floor(SAMPLE_SIZE * 1000.0f / sampleSum);
	}
}
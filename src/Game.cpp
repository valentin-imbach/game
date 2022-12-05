
#include "Game.hpp"
#include "utils/utils.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

Game::Game() {
	running = true;
	lastFrameTicks = 0;
	lastSecondTicks = 0;
	frameCounter = 0;
	framesPerSecond = 0;
	TextureManager::Init();
	TextManager::Init();
	world = std::make_unique<World>("World");
}

void Game::update() {
	Window::instance->clear();
	world->update();
	std::string text = "FPS: " + std::to_string(framesPerSecond);
	TextManager::drawText(text, {20, 20});
	Window::instance->update();
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			LOG("Window closed");
			running = false;
			break;
		}
	}
}

void Game::limitFrameRate(int fps) {
	uint delay = ceil(1000.0f / fps);
	uint ticks = SDL_GetTicks();
	uint past = ticks - lastFrameTicks;
	if (past < delay) {
		SDL_Delay(delay - past);
		past = delay;
	}
	lastFrameTicks = ticks;
	frameCounter += 1;
	if (ticks - lastSecondTicks > 1000) {
		framesPerSecond = frameCounter;
		frameCounter = 0;
		lastSecondTicks = ticks;
	}
}
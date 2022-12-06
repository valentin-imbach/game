
#include "Game.hpp"
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
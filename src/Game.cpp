
#include "Game.hpp"
#include "utils/logger.hpp"

Game::Game(Window window) : window(window) {
    running = true;
}

void Game::update() {
    window.update();
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
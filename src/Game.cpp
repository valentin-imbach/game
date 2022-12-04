
#include "Game.hpp"
#include "utils/logger.hpp"
#include "TextureManager.hpp"

Game::Game(Window window) : window(window) {
    running = true;
    TextureManager::loadSpriteSheets();
}

void Game::update() {
    window.clear();
    TextureManager::drawTexture(SpriteSheet::HOLE, {0,0}, {16,16}, {100,100}, 5);
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
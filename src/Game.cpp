
#include "Game.hpp"
#include "utils/logger.hpp"
#include "TextureManager.hpp"

Game::Game() {
    running = true;
    TextureManager::loadSpriteSheets();
}

void Game::update() {
    Window::instance -> clear();
    pair pos =  Window::instance -> mousePosition;
    TextureManager::drawTexture(SpriteSheet::HOLE, {0,0}, {16,16}, pos, 5, true);
    Window::instance -> update();
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
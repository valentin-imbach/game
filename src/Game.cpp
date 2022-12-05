
#include "Game.hpp"
#include "utils/utils.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

Game::Game() {
    running = true;
    TextureManager::loadSpriteSheets();
    world = std::make_unique<World>("World");
}

void Game::update() {
    Window::instance -> clear();
    world -> update();
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
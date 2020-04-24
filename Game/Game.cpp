//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"
#include "Camera.hpp"

Game::Game() {
    
    LayerManager::addLayer(&tileLayer);
    LayerManager::addLayer(&entityLayer);
    LayerManager::addLayer(&debugLayer);
    LayerManager::addLayer(&guiLayer);
    
    LOG("Game initialized");
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (e.key.repeat) {
            continue;
        }
        if (LayerManager::handleEvent(e)) {
            continue;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
            
        }
    }
}

void Game::update() {
    LayerManager::update();
    Camera::update(entityLayer.player -> getComponent<PositionComponent>().pos);
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    LayerManager::render();
    SDL_RenderPresent(Window::renderer);
}

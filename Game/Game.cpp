//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"
#include "Camera.hpp"
#include "LayerSystem/Layers.h"

GameState Game::state = LOADING;
EntityLayer* Game::entityLayer = nullptr;
DebugLayer* Game::debugLayer = nullptr;
GuiManager Game::guiManager = GuiManager();

void Game::Init() {
    LayerManager::addLayer(entityLayer = new EntityLayer());
    LayerManager::addLayer(debugLayer = new DebugLayer());
    state = RUNNING;
    
    LOG("Game initialized");
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (e.key.repeat) { continue; }
        if (state == RUNNING) {
            if ((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEWHEEL) && guiManager.handleEvent(e)) {
                continue;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = PAUSED;
                return;
            }
            if (LayerManager::handleEvent(e)) {
                continue;
            }
        } else if (state == PAUSED) {
            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = RUNNING;
                return;
            }
        }
    }
}

void Game::update() {
    if (state == RUNNING) {
        LayerManager::update();
        guiManager.update();
        Camera::update(entityLayer -> player -> getComponent<PositionComponent>() -> position);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    LayerManager::render();
    guiManager.render();
    SDL_RenderPresent(Window::renderer);
}

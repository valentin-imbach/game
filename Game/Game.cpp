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
TileLayer* Game::tileLayer = nullptr;
EntityLayer* Game::entityLayer = nullptr;
DebugLayer* Game::debugLayer = nullptr;
GuiLayer* Game::guiLayer = nullptr;

void Game::Init() {
    LayerManager::addLayer(tileLayer = new TileLayer());
    LayerManager::addLayer(entityLayer = new EntityLayer());
    LayerManager::addLayer(debugLayer = new DebugLayer());
    LayerManager::addLayer(guiLayer = new GuiLayer());
    state = RUNNING;
    LOG("Game initialized");
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (e.key.repeat) { continue; }
        if (state == RUNNING) {
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
        Camera::update(entityLayer -> player -> getComponent<PositionComponent>().position);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    LayerManager::render();
    SDL_RenderPresent(Window::renderer);
}

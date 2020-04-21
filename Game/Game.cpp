//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    
    layerManager.addLayer(new TileLayer());
    EntityLayer* entityLayer = new EntityLayer();
    layerManager.addLayer(entityLayer);
    player = entityLayer -> player;
    
}

Game::~Game() {
    
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (layerManager.handleEvent(e)) {
            continue;
        }
        //Leftover events
    }
}

void Game::update() {
    layerManager.update();
    Camera::update();
    Camera::pos = {player -> getComponent<PositionComponent>().x,player -> getComponent<PositionComponent>().y};
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    layerManager.render();
    Camera::render();
    TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
    SDL_RenderPresent(Window::renderer);
}

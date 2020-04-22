//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    
    //Timer("Game Constructor");
    layerManager.addLayer(&tileLayer);
    layerManager.addLayer(&entityLayer);
    layerManager.addLayer(&debugLayer);
    
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
    Camera::update(entityLayer.player -> getComponent<PositionComponent>().pos);
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    layerManager.render();
    SDL_RenderPresent(Window::renderer);
}

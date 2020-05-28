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
    
    world = new World();
    
    pauseMenu = new PauseMenu();
    console = new Console(&(world -> entityLayer));
    
    controller.state = RUNNING;
    
    LOG("Game initialized");
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (controller.handleEvent(e)) { continue; }
        if (controller.state == RUNNING) {
            if (console -> handleEvent(e)) { continue; }
            if (world -> handleEvent(e)) { continue; }
        } else if (controller.state == PAUSED) {
            if (pauseMenu -> handleEvent(e)) { continue; }
        }
    }
}

void Game::update() {
    if (controller.state == RUNNING) {
        world -> update();
        Camera::update(world -> entityLayer.player -> getComponent<PositionComponent>() -> position);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    
    console -> render();
    world -> render();
    
    if (controller.state == RUNNING) {
        //Camera::render();
    } else if (controller.state == PAUSED) {
        pauseMenu -> render();
    }
    
    SDL_RenderPresent(Window::renderer);
}

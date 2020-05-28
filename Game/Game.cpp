//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"
#include "Camera.hpp"

World* Game::world = nullptr;
StateController Game::controller = StateController();

Game::Game() {
    controller.state = MAIN_MENU;
    LOG("Game initialized");
}

void Game::loadWorld() {
    world = new World();
    controller.state = RUNNING;
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (console.handleEvent(e)) { continue; }
        if (controller.state == MAIN_MENU) {
            if (mainMenu.handleEvent(e)) { continue; }
        } else if (controller.state == RUNNING) {
            if (controller.handleEvent(e)) { continue; }
            if (world -> handleEvent(e)) { continue; }
        } else if (controller.state == PAUSED) {
            if (controller.handleEvent(e)) { continue; }
            if (pauseMenu.handleEvent(e)) { continue; }
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
    
    if (controller.state == MAIN_MENU) {
        mainMenu.render();
    }
    
    if (controller.state == RUNNING || controller.state == PAUSED) {
        world -> render();
        //Camera::render();
    }
    
    if (controller.state == PAUSED) {
        pauseMenu.render();
    }
    
    console.render();
    
    SDL_RenderPresent(Window::renderer);
}

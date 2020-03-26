//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    Tile::loadTypes();
    world = new Map(50,50);
    player = new Player(3,3,3);
    
    newPlayer.addComponent<PositionComponent>();
}

Game::~Game() {
    delete world;
    delete player;
}

void Game::update() {
    manager.update();
    world -> update();
    player -> update(world);
    Camera::update();
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    world -> render();
    player -> render();
    player -> renderHotbar(3);
    TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
    SDL_RenderPresent(Window::renderer);
}

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

    player.addComponent<PositionComponent>(5,5);
    player.addComponent<SpriteComponent>();
    player.addComponent<DirectionComponent>(SOUTH);
    player.addComponent<PlayerInputComponent>();
    player.addComponent<PlayerAnimationComponent>();
    player.addComponent<CollisionComponent>(1,1);
    
    
}

Game::~Game() {
    delete world;
}

void Game::update() {
    world -> update();
    manager.refresh();
    manager.update();
    Camera::update();

}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    world -> render();
    manager.render();
    TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
    SDL_RenderPresent(Window::renderer);
}

//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    world = new Map(50,50);

    player.addComponent<PlayerAnimationComponent>();
    player.addComponent<CollisionComponent>(0.4,0.3,0.4,0.2);
    
    wall.addComponent<GridComponent>(3,3,world);
    wall.addComponent<SpriteComponent>();
    
}

Game::~Game() {
    delete world;
}

void Game::update() {
    world -> update();
    manager.refresh();
    manager.update();
    Camera::update();
    Camera::pos = {player.getComponent<PositionComponent>().x*64,player.getComponent<PositionComponent>().y*64};
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    world -> render();
    manager.render();
    TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
    SDL_RenderPresent(Window::renderer);
}

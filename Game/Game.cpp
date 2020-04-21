//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"

Game::Game() {
    world = new Map("map.txt");

    player.addComponent<PositionComponent>(50,50);
    player.addComponent<PlayerAnimationComponent>();
    player.addComponent<CollisionComponent>(0.4,0.3,0.4,0.2);
    player.addComponent<InventoryComponent>(9,5);
    
    player.getComponent<InventoryComponent>().items[0][0] = new ItemStack(0,5);
    player.getComponent<InventoryComponent>().items[1][0] = new ItemStack(1,2);
    player.getComponent<InventoryComponent>().items[1][1] = new ItemStack(2,77);
    player.getComponent<InventoryComponent>().items[0][1] = new ItemStack(3,5);
    
    guiManager.activeGui = &player.addComponent<PlayerGuiComponent>();
    
    //wall.addComponent<GridComponent>(3,3);
    //wall.addComponent<SpriteComponent>();
    
}

Game::~Game() {
    delete world;
}

void Game::update() {
    world -> update();
    entityManager.refresh();
    entityManager.update();
    Camera::update();
    Camera::pos = {player.getComponent<PositionComponent>().x,player.getComponent<PositionComponent>().y};
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 255, 255, 255, 255);
    SDL_RenderClear(Window::renderer);
    world -> render();
    entityManager.render();
    entityManager.debugRender();
    guiManager.update();
    Camera::render();
    TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
    SDL_RenderPresent(Window::renderer);
}

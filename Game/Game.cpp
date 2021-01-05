//
//  Game.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Game.hpp"
#include "Camera.hpp"
#include "SoundManager.hpp"

World* Game::world = nullptr;
StateController Game::controller = StateController();

Game::Game() {
    controller.state = GameState::MAIN_MENU;
    LOG("Game initialized");
}

void Game::createWorld() {
    
    world = new World("World");
    
    Entity* player = world -> entityLayer.entityManager.addEntity();
    
    world -> entityLayer.player = player;
    world -> entityLayer.entityManager.player = player;
    
    player -> addTag(EntityTag::PLAYER);
    
    player -> addComponent<PositionComponent>(pair<float>(50,50));
    player -> addComponent<DirectionComponent>();
    player -> addComponent<HealthComponent>(10);
    
    InventoryComponent* inv = player -> addComponent<InventoryComponent>(7,4);
    inv -> addItem(new ItemStack(ItemID::PICK,1));
    inv -> addItem(new ItemStack(ItemID::HAMMER,1));
    inv -> addItem(new ItemStack(ItemID::SHOVEL,1));
    inv -> addItem(new ItemStack(ItemID::BRANCH,15));
    
    player -> addComponent<CollisionComponent>(0.375,0.25,0.375,0.25);
    player -> addComponent<SpriteComponent>();
    player -> addComponent<PlayerGuiComponent>();
    player -> addComponent<PlayerInputComponent>();
    player -> addComponent<PlayerAnimationComponent>();
    
    controller.state = GameState::RUNNING;
    SoundManager::setVolume(SoundManager::volume);
    //SoundManager::play();
    
}

void Game::loadWorld() {
    
    std::string name = "World";
    world = new World(name);
    
    std::fstream file = std::fstream("saves/"+name, std::ios::in | std::ios::binary);
    world -> deserialize(file);
    file.close();
    
    controller.state = GameState::RUNNING;
    SoundManager::setVolume(SoundManager::volume);
    //SoundManager::play();
}

void Game::handleEvents() {
    for (auto e : Window::events) {
        if (console.handleEvent(e)) continue;
        if (controller.state == GameState::MAIN_MENU) {
            if (mainMenu.handleEvent(e)) continue;
        } else if (controller.state == GameState::RUNNING) {
            if (controller.handleEvent(e)) continue;
            if (world -> handleEvent(e)) continue;
        } else if (controller.state == GameState::PAUSED) {
            if (controller.handleEvent(e)) continue;
            if (pauseMenu.handleEvent(e)) continue;
        }
    }
}

void Game::update() {
    if (controller.state == GameState::RUNNING) {
        world -> update();
        Camera::update(world -> entityLayer.player -> getComponent<PositionComponent>() -> position);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Window::renderer, 200, 200, 200, 255);
    SDL_RenderClear(Window::renderer);
    
    if (controller.state == GameState::MAIN_MENU) mainMenu.render();
    if (controller.state == GameState::RUNNING || controller.state == GameState::PAUSED) world -> render();
    if (controller.state == GameState::PAUSED) pauseMenu.render();
    console.render();
    
    SDL_RenderPresent(Window::renderer);
}

void Game::serialize(std::fstream& stream) {
    serialize_(stream, SoundManager::volume, Camera::ZOOM, Window::MAX_FPS);
}

void Game::deserialize(std::fstream& stream) {
    deserialize_(stream, SoundManager::volume, Camera::ZOOM, Window::MAX_FPS);
}

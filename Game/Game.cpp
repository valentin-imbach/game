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
    TextManager::Init();
    SoundManager::Init();
    TextureManager::Init();
    LOG("Game initialized");
}

void Game::createWorld() {
    world = new World("World");
    
    Entity* house = world -> entityLayer.entityManager.addEntity();
    
    house -> addComponent<PositionComponent>(pair<float>(63,41));
    house -> addComponent<SizeComponent>(pair<float>(7,3));
    house -> addComponent<SpriteComponent>(Sprite(TextureManager::getTexture("house.png"),{0,0},{7,7}), 4);
    house -> addComponent<GridComponent>(pair<float>(60,40),pair<int>(7,3));
    
    world -> populate();
    
    Entity* player = world -> entityLayer.entityManager.addEntity();
    
    world -> entityLayer.player = player;
    world -> entityLayer.entityManager.player = player;
    
    player -> addTag(EntityTag::PLAYER);
    player -> addComponent<PositionComponent>(pair<float>(50,50));
    player -> addComponent<SizeComponent>();
    player -> addComponent<DirectionComponent>();
    player -> addComponent<HealthComponent>(10);
    
    InventoryComponent* inv = player -> addComponent<InventoryComponent>(7,4);
    inv -> addItem(new Tool("Sword", ItemType::SWORD, {0,0}));
    inv -> addItem(new Tool("Pick Axe", ItemType::PICK_AXE, {1,0}));
    inv -> addItem(new Tool("Hammer", ItemType::AXE, {2,0}));
    inv -> addItem(new Tool("Shovel" ,ItemType::SHOVEL, {3,0}));
    inv -> addItem(new ItemStack(ItemID::ASHWOOD_PLANK, 15));
    
    
    player -> addComponent<CollisionComponent>(0.375,0.25,0.375,0.25);
    player -> addComponent<SpriteComponent>();
    player -> addComponent<PlayerGuiComponent>();
    player -> addComponent<PlayerInputComponent>();
    player -> addComponent<PlayerAnimationComponent>();
    
    Entity* cow = world -> entityLayer.entityManager.addEntity();
    cow -> addComponent<PositionComponent>(pair<float>(53,50));
    cow -> addComponent<SizeComponent>();
    cow -> addComponent<DirectionComponent>();
    cow -> addComponent<HealthComponent>(10);
    cow -> addComponent<CollisionComponent>(0.375,0.25,0.375,0.25);
    cow -> addComponent<SpriteComponent>(Sprite(TextureManager::getTexture("cow.png"),{0,0},{1,1},4));
    
    
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

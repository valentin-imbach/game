
#include "Game.hpp"
#include "utils/utils.hpp"
#include "TextureManager.hpp"
#include "Components.hpp"
#include "Window.hpp"

Game::Game() {
    running = true;
    TextureManager::loadSpriteSheets();
    rosterComponents();
    rosterSystems();
    
    Entity player = ecs.createEntity();

    PositionComponent positionComponent = {{200,100}};
    //ecs.addComponent(positionComponent, player);

    SpriteComponent spriteComponent = {SpriteSheet::HOLE};
    ecs.addComponent(spriteComponent, player);
}

void Game::rosterComponents() {
    ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
    ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
}

void Game::rosterSystems() {
    Signature signature = ECS::makeSiganture({ComponentId::SPRITE, ComponentId::POSITION});
    spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, signature);
    gravitySystem = ecs.rosterSystem<GravitySystem>(SystemId::GRAVITY, ECS::makeSiganture({ComponentId::POSITION}));
}

void Game::update() {
    Window::instance -> clear();
    spriteSystem -> update();
    gravitySystem -> update();
    Window::instance -> update();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            LOG("Window closed");
            running = false;
            break;
        }
    }
}
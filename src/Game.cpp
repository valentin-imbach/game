
#include "Game.hpp"
#include "utils/logger.hpp"
#include "TextureManager.hpp"
#include "Components.hpp"

Game::Game() {
    running = true;
    TextureManager::loadSpriteSheets();

    Signature signature = ECS::makeSiganture({ComponentId::SPRITE, ComponentId::POSITION});
    spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, signature);

    Entity player = ecs.createEntity();

    ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
    PositionComponent positionComponent = {{200,100}};
    ecs.addComponent(positionComponent, player);
   
    ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
    SpriteComponent spriteComponent = {SpriteSheet::HOLE};
    ecs.addComponent(spriteComponent, player);
}

void Game::update() {
    Window::instance -> clear();
    //pair pos =  Window::instance -> mousePosition;
    //TextureManager::drawTexture(SpriteSheet::HOLE, {0,0}, {16,16}, pos, 5, true);
    spriteSystem -> update(ecs.componentManager);
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

#include "World.hpp"
#include "Components.hpp"

World::World(std::string name) : name(name) {
    rosterComponents();
    rosterSystems();

    Entity player = ecs.createEntity();

    PositionComponent positionComponent = {{200,100}};
    ecs.addComponent(positionComponent, player);

    SpriteComponent spriteComponent = {SpriteSheet::HOLE};
    ecs.addComponent(spriteComponent, player);
}

void World::rosterComponents() {
    ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
    ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
}

void World::rosterSystems() {
    Signature signature = ECS::makeSiganture({ComponentId::SPRITE, ComponentId::POSITION});
    spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, signature);
    gravitySystem = ecs.rosterSystem<GravitySystem>(SystemId::GRAVITY, ECS::makeSiganture({ComponentId::POSITION}));
}

void World::update() {
    gravitySystem -> update();
    spriteSystem -> update();
}
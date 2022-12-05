
#pragma once
#include "utils/utils.hpp"
#include "ECS.hpp"
#include "Systems/SpriteSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Map.hpp"

class World {
public:
    World(std::string name);
    void update();
private:
    void rosterComponents();
    void rosterSystems();

    ECS ecs;
    SpriteSystem* spriteSystem;
    GravitySystem* gravitySystem;

    std::string name;

    Entity player;
    Entity camera;

    Map map;
    void renderMap();
};
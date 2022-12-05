
#pragma once
#include "utils/utils.hpp"
#include "ECS.hpp"
#include "Systems/SpriteSystem.hpp"
#include "Systems/GravitySystem.hpp"

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
};
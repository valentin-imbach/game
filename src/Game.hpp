
#pragma once
#include "ECS.hpp"
#include "Systems/SpriteSystem.hpp"
#include "Systems/GravitySystem.hpp"

class Game {
public:
    Game();

    void update();
    void handleEvents();

    void rosterComponents();
    void rosterSystems();

    bool running;

    ECS ecs;
    SpriteSystem* spriteSystem;
    GravitySystem* gravitySystem;
};
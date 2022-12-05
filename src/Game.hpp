
#pragma once
#include "Window.hpp"
#include "ECS.hpp"
#include "Systems/SpriteSystem.hpp"

class Game {
public:
    Game();

    void update();
    void handleEvents();

    bool running;

    ECS ecs;
    SpriteSystem* spriteSystem;
};
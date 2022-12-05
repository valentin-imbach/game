
#pragma once
#include "World.hpp"
#include <memory>

class Game {
public:
    Game();

    void update();
    void handleEvents();

    bool running;
    std::unique_ptr<World> world;
};

#pragma once
#include "Window.hpp"

class Game {
public:
    Game();

    void update();
    void handleEvents();
    bool running;    
};
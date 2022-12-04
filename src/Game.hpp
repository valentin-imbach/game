
#pragma once
#include "Window.hpp"

class Game {
public:
    Game(Window window);

    void update();
    void handleEvents();
    bool running;
    Window window;
    
};
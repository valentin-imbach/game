//
//  main.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include <iostream>
#include "tools.h"
#include "Window.hpp"
#include "Game.hpp"
#include "Camera.hpp"

int main() {
    
    std::srand(1);
    
    Window window("Game",1024,640);
    Game game;
    
    while (window.isRunning()) {
        window.update();
        game.update();
        game.render();
    }
    
    return 0;
}

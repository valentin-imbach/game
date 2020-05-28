//
//  main.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "Window.hpp"
#include "Game.hpp"


int main() {
    
    std::srand((int)time(0));
    
    Window window("Game",1224,840);
    Game game;
    
    while (Window::running) {
        window.update();
        game.handleEvents();
        game.update();
        game.render();
    }
    
    std::fstream file = std::fstream("save.binary", std::ios::out | std::ios::binary);
    game.world -> entityLayer.player -> serialize(file);
    file.close();
    
    return 0;
}

//
//  main.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Window.hpp"
#include "Game.hpp"

int main() {
    
    std::srand((int)time(0));
    
    Window window("Game",1224,840);
    Game game;
    
    std::fstream file = std::fstream("config.binary", std::ios::in | std::ios::binary);
    game.deserialize(file);
    file.close();
    
    while (Window::running) {
        window.update();
        game.handleEvents();
        game.update();
        game.render();
    }
    
    if (game.world != nullptr) {
        file = std::fstream("saves/" + game.world -> name, std::ios::out | std::ios::binary);
        game.world -> serialize(file);
        file.close();
    }
    
    file = std::fstream("config.binary", std::ios::out | std::ios::binary);
    game.serialize(file);
    file.close();
    
    return 0;
}

//
//  Console.hpp
//  Game
//
//  Created by Valentin Imbach on 26.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "LayerSystem/Layers.h"
#include "MessagingSystem.hpp"

class Console : public Observer {
private:
    EntityLayer* entityLayer;
    
    bool active = false;
    
    int memory = 15;
    int memIt = -1;
    v(std::string) history;
    std::string text;
    
    bool execute(std::string);
    
public:
    
    Console() {
        subscribe(MessageType::PRINT);
    }
    
    void render();
    void write(std::string text);
    bool handleEvent(SDL_Event event);
    bool onMessage(const Message& message) override;
};

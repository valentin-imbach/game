//
//  GuiLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../GuiSystem/GuiManager.hpp"

class GuiLayer : public Layer {
public:
    
    GuiLayer() {
        LOG("Gui Layer constructed");
    }
    
    void update() override {
        GuiManager::update();
    }
    
    void render() override {
        GuiManager::render();
    }
    
    bool handleEvent(SDL_Event event) override {
        return GuiManager::handleEvent(event);
    }
};
